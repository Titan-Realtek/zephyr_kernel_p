/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com>
 */

#define DT_DRV_COMPAT realtek_rts5918_rtmr

#include <stdint.h>

#include <zephyr/init.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys_clock.h>
#include <zephyr/irq.h>
#include <zephyr/spinlock.h>
#include <zephyr/drivers/timer/system_timer.h>

#include <reg/reg_rtmr.h>
#include <reg/reg_system.h>

#define RTS5918_SCCON_REG_BASE ((SYSTEM_Type *)(DT_REG_ADDR(DT_NODELABEL(sccon))))

#define CYCLES_PER_TICK (CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC / CONFIG_SYS_CLOCK_TICKS_PER_SEC)

BUILD_ASSERT(DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT) == 1,
	     "Realtek RTOS timer is not supported multiple instances");

#define RTMR_REG ((RTOSTMR_Type *)DT_INST_REG_ADDR(0))

#define SLWTMR_REG ((RTOSTMR_Type *)(DT_REG_ADDR(DT_NODELABEL(slwtmr0))))

#define SSCON_REG ((SYSTEM_Type *)(DT_REG_ADDR(DT_NODELABEL(sccon))))

#define RTMR_COUNTER_MAX   0x0ffffffful
#define RTMR_TIMER_STOPPED 0xf0000000ul

#define MAX_TICKS (RTMR_COUNTER_MAX / CYCLES_PER_TICK)

/* Adjust cycle count programmed into timer for HW restart latency */
#define RTMR_ADJUST_LIMIT  2
#define RTMR_ADJUST_CYCLES 1

static struct k_spinlock lock;

/*
 * Total HW cycles elapsed since boot.  Updated in the ISR (when the
 * timer expires) and in sys_clock_set_timeout() (to capture partial
 * cycles from a preempted timer period).
 *
 * At any point the current cycle count can be computed as:
 *
 *     current = cycle_count + elapsed_cycles()
 */
static uint32_t cycle_count;

/*
 * Total cycles announced to the kernel via sys_clock_announce().
 * The difference (cycle_count - announced_cycles) represents
 * elapsed but unannounced cycles.
 */
static uint32_t announced_cycles;

/*
 * Last load value programmed into the RTMR hardware counter.
 * The counter counts down from last_load to 0.
 * May be set to RTMR_TIMER_STOPPED when the timer is disabled.
 */
static uint32_t last_load;

/*
 * Last value returned by sys_clock_cycle_get_32().  Used as a
 * monotonicity guard to suppress spurious rewinds that arise
 * because the RTMR hardware cannot reliably distinguish "CNT
 * reached 0 (timer expired)" from "LDCNT just written (timer
 * restarted)" within a handful of APB clock cycles.
 */
static uint32_t last_cycle_ret;

static void rtmr_restart(uint32_t counter)
{
	RTMR_REG->CTRL = 0ul;
	RTMR_REG->LDCNT = counter;
	RTMR_REG->CTRL = RTOSTMR_CTRL_INTEN_Msk | RTOSTMR_CTRL_EN_Msk;
}

static uint32_t rtmr_get_counter(void)
{
	uint32_t counter = RTMR_REG->CNT;

	if ((counter == 0) && (RTMR_REG->CTRL & RTOSTMR_CTRL_EN_Msk)) {
		/*
		 * CNT=0 with EN=1: the timer has expired (or, far less
		 * commonly, was just restarted but the counter hasn't
		 * loaded yet).  Return last_load so that elapsed_cycles()
		 * computes last_load - last_load = 0.  The ISR will
		 * correctly add last_load into cycle_count.
		 */
		counter = last_load;
	}

	return counter;
}

/*
 * HW cycles elapsed since the last cycle_count update.
 * Must be called with lock held.
 *
 * Returns the number of cycles the counter has counted down since
 * it was last programmed via rtmr_restart(), or 0 if the timer is
 * stopped.
 */
static uint32_t elapsed_cycles(void)
{
	uint32_t cur_cnt = rtmr_get_counter();

	if ((last_load == RTMR_TIMER_STOPPED) || (last_load == 0)) {
		/*
		 * last_load == RTMR_TIMER_STOPPED: timer disabled (idle forever).
		 * last_load == 0: timer expired and ISR has already accounted
		 * for all elapsed cycles via cycle_count (see rtmr_isr).
		 * In either case, no cycles have elapsed since the last
		 * cycle_count update.
		 */
		return 0;
	}

	if (cur_cnt > last_load) {
		/*
		 * Counter has wrapped past 0 (underflowed) after expiring
		 * but before the ISR could service the interrupt.  In this
		 * window the raw (last_load - cur_cnt) would underflow to
		 * a spurious huge value, causing sys_clock_cycle_get_32()
		 * to spike and then appear to rewind after the ISR runs.
		 *
		 * Return last_load (the full period); the ISR will consume
		 * this same value into cycle_count, keeping monotonicity.
		 */
		return last_load;
	}

	return last_load - cur_cnt;
}

static void rtmr_isr(const void *arg)
{
	ARG_UNUSED(arg);

	uint32_t dcycles;
	int32_t ticks;

	k_spinlock_key_t key = k_spin_lock(&lock);

	RTMR_REG->INTSTS = RTOSTMR_INTSTS_STS_Msk;

	/* One-shot mode: the timer has counted down to 0.
	 * Account for all elapsed cycles: the LDCNT value (last_load) plus
	 * the fixed HW restart latency.  Do NOT restart the timer here --
	 * sys_clock_set_timeout() called from within sys_clock_announce()
	 * will reprogram it.
	 */
	cycle_count += last_load + RTMR_ADJUST_CYCLES;
	last_load = 0;

	dcycles = cycle_count - announced_cycles;
	ticks = dcycles / CYCLES_PER_TICK;

	announced_cycles += ticks * CYCLES_PER_TICK;

	k_spin_unlock(&lock, key);

	sys_clock_announce(ticks);
}

void sys_clock_set_timeout(int32_t ticks, bool idle)
{
	ARG_UNUSED(idle);

	uint32_t cur_cnt;
	int full_ticks;
	uint32_t full_cycles;
	uint32_t partial_cycles;

	if (idle && (ticks == K_TICKS_FOREVER)) {
		RTMR_REG->CTRL = 0U;
		last_load = RTMR_TIMER_STOPPED;
		return;
	}

	if (ticks <= 0) {
		full_ticks = 0;
	} else if ((ticks == K_TICKS_FOREVER) || (ticks > MAX_TICKS)) {
		full_ticks = MAX_TICKS - 1;
	} else {
		full_ticks = ticks - 1;
	}

	full_cycles = full_ticks * CYCLES_PER_TICK;

	k_spinlock_key_t key = k_spin_lock(&lock);

	/* Detect whether the timer was actively running.  A running
	 * timer incurred a restart latency (RTMR_ADJUST_CYCLES) when
	 * rtmr_restart() was called.  The ISR normally accounts for
	 * this (see rtmr_isr), but if we reprogram the timer before
	 * the current period completes, that latency would be lost.
	 */
	bool timer_was_running = (last_load != 0) &&
				(last_load != RTMR_TIMER_STOPPED) &&
				(RTMR_REG->CTRL & RTOSTMR_CTRL_EN_Msk);

	cur_cnt = rtmr_get_counter();

	RTMR_REG->CTRL = 0U;

	/*
	 * Account for partial cycles elapsed since the last update.
	 *
	 * When called after the ISR (last_load == 0), the ISR has already
	 * accounted for all cycles via cycle_count.  The HW counter may
	 * have underflowed past 0, so 0 - cur_cnt would add a spurious
	 * positive value that corrupts cycle_count.  Skip the update.
	 */
	if (last_load != 0) {
		if (cur_cnt > last_load) {
			cycle_count += last_load;
		} else {
			cycle_count += last_load - cur_cnt;
		}
	}

	/* Compensate for the restart latency if we're interrupting
	 * a running period (the ISR won't get to add it).
	 */
	if (timer_was_running) {
		cycle_count += RTMR_ADJUST_CYCLES;
	}

	/* Align to the next tick boundary */
	partial_cycles = CYCLES_PER_TICK - (cycle_count % CYCLES_PER_TICK);
	last_load = full_cycles + partial_cycles;

	rtmr_restart(last_load);

	k_spin_unlock(&lock, key);
}

uint32_t sys_clock_elapsed(void)
{
	uint32_t cur_cycles;
	uint32_t delta;

	k_spinlock_key_t key = k_spin_lock(&lock);

	cur_cycles = cycle_count + elapsed_cycles();
	delta = cur_cycles - announced_cycles;

	k_spin_unlock(&lock, key);

	return delta / CYCLES_PER_TICK;
}

void sys_clock_idle_exit(void)
{
	if (last_load == RTMR_TIMER_STOPPED) {
		last_load = CYCLES_PER_TICK;
		rtmr_restart(last_load);
	}
}

void sys_clock_disable(void)
{
	/* Disable RTMR. */
	RTMR_REG->CTRL = 0ul;
}

uint32_t sys_clock_cycle_get_32(void)
{
	uint32_t ret;

	k_spinlock_key_t key = k_spin_lock(&lock);

	ret = cycle_count + elapsed_cycles();

	/*
	 * Monotonicity guard: the RTMR counter at 0 with EN=1 is
	 * indistinguishable between "just expired" and "just restarted"
	 * for a few APB clock cycles.  If we misclassify expired as
	 * restarted, elapsed_cycles() returns 0 instead of last_load,
	 * making ret temporarily dip by last_load cycles.  When the
	 * next call correctly accounts for the expired period, the
	 * value jumps back up, creating a spurious rewind from the
	 * caller's perspective.  Clamp ret to never go backwards.
	 */
	if (ret < last_cycle_ret) {
		if ((last_cycle_ret - ret) < (uint32_t)(UINT32_MAX / 2)) {
			ret = last_cycle_ret;
		}
	}
	last_cycle_ret = ret;

	k_spin_unlock(&lock, key);

	return ret;
}

#ifdef CONFIG_ARCH_HAS_CUSTOM_BUSY_WAIT

void arch_busy_wait(uint32_t n_usec)
{
	if (n_usec == 0) {
		return;
	}

	uint32_t start = SLWTMR_REG->CNT;

	for (;;) {
		uint32_t curr = SLWTMR_REG->CNT;

		if ((start - curr) >= n_usec) {
			break;
		}
	}
}
#endif

static int sys_clock_driver_init(void)
{
	/* Enable RTMR clock power */
	RTMR_REG->INTSTS = RTOSTMR_INTSTS_STS_Msk;
	NVIC_ClearPendingIRQ(DT_INST_IRQN(0));

	SYSTEM_Type *sys_reg = RTS5918_SCCON_REG_BASE;
	sys_reg->IPCLK3 |= SYSTEM_IPCLK3__RTMR_Msk;
	sys_reg->APBCLK1 |= SYSTEM_APBCLK1__RTMR_Msk;

	sys_reg->CLKGATING3 |= SYSTEM_CLKGATING3__RTMR_Msk;
	sys_reg->IPRST3 &= ~SYSTEM_IPRST3__RTMR_Msk;
	sys_reg->IPRST3 |= SYSTEM_IPRST3__RTMR_Msk;
	sys_reg->CLKGATING3 &= ~SYSTEM_CLKGATING3__RTMR_Msk;

	/* Enable RTMR interrupt. */
	IRQ_CONNECT(DT_INST_IRQN(0), DT_INST_IRQ(0, priority), rtmr_isr, 0, 0);
	irq_enable(DT_INST_IRQN(0));

	/* Trigger RTMR and wait it start to counting */
	last_load = RTMR_COUNTER_MAX;

	rtmr_restart(last_load);
	while (RTMR_REG->CNT == 0) {
	};

#ifdef CONFIG_ARCH_HAS_CUSTOM_BUSY_WAIT
#if CONFIG_SOC_RTS5918
	/* Enable SLWTMR0 clock power */
	sys_reg->IPCLK2 |= SYSTEM_IPCLK2__SLWTMR0_Msk;
#endif
	/* Enable SLWTMR0 */
	SLWTMR_REG->LDCNT = 0xfffffffful;
	SLWTMR_REG->CTRL = (RTOSTMR_CTRL_MDSEL_Msk | RTOSTMR_CTRL_EN_Msk);
#endif

	return 0;
}

SYS_INIT(sys_clock_driver_init, PRE_KERNEL_2, CONFIG_SYSTEM_CLOCK_INIT_PRIORITY);
