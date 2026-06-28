/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Flashtool application — launches the flashtool housekeeper thread.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "Flashtool.h"

LOG_MODULE_REGISTER(app, CONFIG_EC_LOG_LEVEL);

/* ── Thread resources ──────────────────────────────────────────── */

#define FLASHTOOL_STACK_SIZE  4096
#define FLASHTOOL_PRIORITY    5

/*
 * Poll interval passed to flashtool_thread() via p1.
 * The thread reads *(uint32_t *)p1 once at startup and then
 * k_msleep(same_value) at the bottom of every loop iteration.
 */
static uint32_t flashtool_period_ms = 100;

K_THREAD_DEFINE(flashtool_tid, FLASHTOOL_STACK_SIZE,S84
                flashtool_thread, &flashtool_period_ms, NULL, NULL,
                FLASHTOOL_PRIORITY, 0, 0);

/* ── main() ────────────────────────────────────────────────────── */

int main(void)
{
	LOG_INF("Flashtool app started, thread prio=%d period=%ums",
		FLASHTOOL_PRIORITY, flashtool_period_ms);

	/*
	 * The flashtool thread was created by K_THREAD_DEFINE with
	 * delay=0, so it is already running.  main() can idle here
	 * or perform other tasks.
	 */
	return 0;
}
