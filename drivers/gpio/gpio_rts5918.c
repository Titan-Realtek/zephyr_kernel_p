/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2024 Realtek Semiconductor Corporation, SIBG-SD7
 * Author: Lin Yu-Cheng <lin_yu_cheng@realtek.com>
 */

#define DT_DRV_COMPAT realtek_rts5918_gpio

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/irq.h>
#include "zephyr/drivers/gpio/gpio_utils.h"
#include <zephyr/logging/log.h>
#include <zephyr/dt-bindings/gpio/realtek-gpio.h>
/*
 * Phase 9.1: input_kbd_matrix.h is only included when the kbd matrix
 * subsystem is enabled. Without this guard, the header pulls in
 * CONFIG_INPUT_KBD_MATRIX_THREAD_STACK_SIZE which is only declared
 * inside `if INPUT_KBD_MATRIX` and breaks the build on boards (like
 * Bison) that haven't enabled the keyboard matrix yet.
 */
// #if defined(CONFIG_INPUT_KBD_MATRIX) && \
//     DT_NODE_HAS_STATUS(DT_NODELABEL(kbd), okay)
// #include <zephyr/input/input_kbd_matrix.h>
// #define RTS5918_GPIO_HAS_KBD 1
// #else
// #define RTS5918_GPIO_HAS_KBD 0
// #endif
#include <reg/reg_gpio.h>

LOG_MODULE_REGISTER(gpio_rts5918, CONFIG_GPIO_LOG_LEVEL);

#define RTS5918_GPIOA_REG_BASE ((GPIO_Type *)(DT_REG_ADDR(DT_NODELABEL(gpioa))))

struct gpio_rts5918_config {
	struct gpio_driver_config common;
	volatile uint32_t *reg_base;
	uint8_t num_pins;
	uint32_t irq_num;
};

struct gpio_rts5918_data {
	struct gpio_driver_data common;
	sys_slist_t callbacks;
};

static int pin_is_valid(const struct gpio_rts5918_config *config, gpio_pin_t pin)
{
	if (pin >= config->num_pins) {
		return -EINVAL;
	}

	return 0;
}

static int pin_output_high(const struct device *port, gpio_pin_t pin)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = &config->reg_base[pin];

	int err = pin_is_valid(config, pin);

	if (err) {
		return err;
	}

	if (*gcr & GPIO_GCR_OUTMD_Msk) {
		/* Switch I/O mode to input mode when configuration is open-drain with output high
		 */
		*gcr = (*gcr & ~(GPIO_GCR_DIR_Msk | GPIO_GCR_OUTCTRL_Msk));// | GPIO_GCR_OUTCTRL_Msk);
	} else {
		*gcr |= GPIO_GCR_OUTCTRL_Msk | GPIO_GCR_DIR_Msk;
	}

	return 0;
}

static int pin_output_low(const struct device *port, gpio_pin_t pin)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = &config->reg_base[pin];

	int err = pin_is_valid(config, pin);

	if (err) {
		return err;
	}

	*gcr = (*gcr & ~GPIO_GCR_OUTCTRL_Msk) | GPIO_GCR_DIR_Msk;

	return 0;
}

static int gpio_rts5918_configuration(const struct device *port, gpio_pin_t pin, gpio_flags_t flags)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = &config->reg_base[pin];
	uint32_t cfg_val = *gcr;

	int err = pin_is_valid(config, pin);

	if (err) {
		return err;
	}

	if (flags & GPIO_INPUT) {
		cfg_val &= ~GPIO_GCR_DIR_Msk;
		cfg_val &= ~GPIO_GCR_OUTCTRL_Msk;
		/* enable input detect */
		cfg_val |= GPIO_GCR_INDETEN_Msk;
	}

	if (flags & GPIO_DISCONNECTED) {
		cfg_val &= ~GPIO_GCR_DIR_Msk;
		cfg_val &= ~GPIO_GCR_OUTCTRL_Msk;
		/* disable input detect */
		cfg_val &= ~GPIO_GCR_INDETEN_Msk;
	}

	if (flags & GPIO_OPEN_DRAIN) {
		cfg_val |= GPIO_GCR_OUTMD_Msk;
	} else {
		cfg_val &= ~GPIO_GCR_OUTMD_Msk;
	}

	switch (flags & (GPIO_PULL_UP | GPIO_PULL_DOWN)) {
	case GPIO_PULL_UP:
		cfg_val &= ~GPIO_GCR_PULLDWEN_Msk;
		cfg_val |= GPIO_GCR_PULLUPEN_Msk;
		break;
	case GPIO_PULL_DOWN:
		cfg_val &= ~GPIO_GCR_PULLUPEN_Msk;
		cfg_val |= GPIO_GCR_PULLDWEN_Msk;
		break;
	default:
		break;
	}

	switch (flags & RTS5918_GPIO_VOLTAGE_MASK) {
	case RTS5918_GPIO_VOLTAGE_1V8:
		cfg_val |= GPIO_GCR_INVOLMD_Msk;
		break;
	case RTS5918_GPIO_VOLTAGE_DEFAULT:
	case RTS5918_GPIO_VOLTAGE_3V3:
		cfg_val &= ~GPIO_GCR_INVOLMD_Msk;
		break;
	case RTS5918_GPIO_VOLTAGE_5V0:
		return -ENOTSUP;
	default:
		break;
	}

	if (flags & RTS5918_GPIO_OUTDRV) {
		cfg_val |= GPIO_GCR_OUTDRV_Msk;
	} else {
		cfg_val &= ~GPIO_GCR_OUTDRV_Msk;
	}

	if (flags & RTS5918_GPIO_SLEWRATE) {
		cfg_val |= GPIO_GCR_SLEWRATE_Msk;
	} else {
		cfg_val &= ~GPIO_GCR_SLEWRATE_Msk;
	}

	if (flags & RTS5918_GPIO_SCHEN) {
		cfg_val |= GPIO_GCR_SCHEN_Msk;
	} else {
		cfg_val &= ~GPIO_GCR_SCHEN_Msk;
	}

#if 0
	/*
	 * Only update MFCTRL when the caller explicitly selected a function
	 * via the RTS5918_GPIO_MFCTRL_x flags. RTS5918_GPIO_MFCTRL_0 == 0,
	 * so a plain `gpio_pin_configure(..., GPIO_INPUT)` would otherwise
	 * fall into "case MFCTRL_0" and reset MFCTRL to plain GPIO,
	 * clobbering any pinctrl alt-function previously set on the pad
	 * (e.g. by drv_pinctrl_rtk_set_func() or pinctrl-0 in DT).
	 *
	 * Preserving the existing MFCTRL when no MFCTRL_x bit is in flags
	 * lets pinctrl drivers keep ownership of the alt-function field.
	 */
	if (flags & RTS5918_GPIO_MFCTRL_MASK) {
		cfg_val &= ~GPIO_GCR_MFCTRL_Msk;
		switch (flags & RTS5918_GPIO_MFCTRL_MASK) {
		case RTS5918_GPIO_MFCTRL_0:
			cfg_val |= (0U << GPIO_GCR_MFCTRL_Pos);
			break;
		case RTS5918_GPIO_MFCTRL_1:
			cfg_val |= (1U << GPIO_GCR_MFCTRL_Pos);
			break;
		case RTS5918_GPIO_MFCTRL_2:
			cfg_val |= (2U << GPIO_GCR_MFCTRL_Pos);
			break;
		case RTS5918_GPIO_MFCTRL_3:
			cfg_val |= (3U << GPIO_GCR_MFCTRL_Pos);
			break;
		default:
			return -EINVAL;
		}
	}
#endif

	*gcr = cfg_val;

	if (flags & GPIO_OUTPUT) {
		if (flags & GPIO_OUTPUT_INIT_HIGH) {
			pin_output_high(port, pin);
		} else if (flags & GPIO_OUTPUT_INIT_LOW) {
			pin_output_low(port, pin);
		}
	}

	return 0;
}

#ifdef CONFIG_GPIO_GET_CONFIG
static int gpio_rts5918_get_configuration(const struct device *port, gpio_pin_t pin,
					  gpio_flags_t *flags)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = &config->reg_base[pin];
	gpio_flags_t cfg_flag = 0x0UL;

	int err = pin_is_valid(config, pin);

	if (err) {
		return err;
	}

	if (*gcr & GPIO_GCR_OUTCTRL_Msk) {
		cfg_flag |= GPIO_OUTPUT | GPIO_OUTPUT_INIT_HIGH;
	} else {
		if (*gcr & GPIO_GCR_DIR_Msk) {
			cfg_flag |= GPIO_OUTPUT | GPIO_OUTPUT_INIT_LOW;
		} else {
			cfg_flag |= GPIO_INPUT;
			if (*gcr & GPIO_GCR_INVOLMD_Msk) {
				cfg_flag |= RTS5918_GPIO_VOLTAGE_1V8;
			} else {
				cfg_flag |= RTS5918_GPIO_VOLTAGE_3V3;
			}
		}
	}

	if (*gcr & GPIO_GCR_OUTMD_Msk) {
		cfg_flag |= GPIO_OPEN_DRAIN;
	}

	if (*gcr & GPIO_GCR_PULLUPEN_Msk) {
		cfg_flag |= GPIO_PULL_UP;
	} else if (*gcr & GPIO_GCR_PULLDWEN_Msk) {
		cfg_flag |= GPIO_PULL_DOWN;
	}

	if (*gcr & GPIO_GCR_INDETEN_Msk) {
		cfg_flag |= RTS5918_GPIO_INDETEN;
	} else {
		cfg_flag &= ~RTS5918_GPIO_INDETEN;
	}

	if (*gcr & GPIO_GCR_OUTDRV_Msk) {
		cfg_flag |= RTS5918_GPIO_OUTDRV;
	} else {
		cfg_flag &= ~RTS5918_GPIO_OUTDRV;
	}

	if (*gcr & GPIO_GCR_SLEWRATE_Msk) {
		cfg_flag |= RTS5918_GPIO_SLEWRATE;
	} else {
		cfg_flag &= ~RTS5918_GPIO_SLEWRATE;
	}

	if (*gcr & GPIO_GCR_SCHEN_Msk) {
		cfg_flag |= RTS5918_GPIO_SCHEN;
	} else {
		cfg_flag &= ~RTS5918_GPIO_SCHEN;
	}

	switch ((*gcr & GPIO_GCR_MFCTRL_Msk) >> GPIO_GCR_MFCTRL_Pos) {
	case 0:
		cfg_flag |= RTS5918_GPIO_MFCTRL_0;
		break;
	case 1:
		cfg_flag |= RTS5918_GPIO_MFCTRL_1;
		break;
	case 2:
		cfg_flag |= RTS5918_GPIO_MFCTRL_2;
		break;
	case 3:
		cfg_flag |= RTS5918_GPIO_MFCTRL_3;
		break;
	default:
		cfg_flag |= RTS5918_GPIO_MFCTRL_0;
		break;
	}

	if (*gcr & GPIO_GCR_INTEN_Msk) {
		switch (*gcr & GPIO_GCR_INTCTRL_Msk) {
		case GPIO_GCR_INTCTRL_TRIG_EDGE_HIGH:
			cfg_flag |= GPIO_INT_EDGE_RISING;
			break;
		case GPIO_GCR_INTCTRL_TRIG_EDGE_LOW:
			cfg_flag |= GPIO_INT_EDGE_FALLING;
			break;
		case GPIO_GCR_INTCTRL_TRIG_EDGE_BOTH:
			cfg_flag |= GPIO_INT_EDGE_BOTH;
			break;
		case GPIO_GCR_INTCTRL_TRIG_LEVEL_LOW:
			cfg_flag |= GPIO_INT_LEVEL_LOW;
			break;
		case GPIO_GCR_INTCTRL_TRIG_LEVEL_HIGH:
			cfg_flag |= GPIO_INT_LEVEL_HIGH;
			break;
		default:
			cfg_flag |= GPIO_INT_LEVEL_LOW;
			break;
		}
	} else {
		cfg_flag |= GPIO_INT_DISABLE;
	}

	*flags = cfg_flag;

	return 0;
}
#endif

static int gpio_rts5918_port_get_raw(const struct device *port, gpio_port_value_t *value)
{
	const struct gpio_rts5918_config *config = port->config;
	gpio_port_value_t ret_val = 0;
	uint16_t mask = 0x1U;

	for (gpio_pin_t i = 0; i < config->num_pins; i++) {
		if (config->reg_base[i] & GPIO_GCR_PINSTS_Msk) {
			ret_val |= (gpio_port_value_t)mask;
		}
		mask <<= 1;
	}

	*value = ret_val;

	return 0;
}

static int gpio_rts5918_port_set_masked_raw(const struct device *port, gpio_port_pins_t mask,
					    gpio_port_value_t value)
{
	const struct gpio_rts5918_config *config = port->config;
	uint32_t pin;

	mask &= 0x0000FFFF;
	for (; mask; mask &= ~BIT(pin)) {
		pin = find_lsb_set(mask) - 1;
		if (pin >= config->num_pins) {
			break;
		}

		if (value & BIT(pin)) {
			pin_output_high(port, pin);
		} else {
			pin_output_low(port, pin);
		}
	}

	return 0;
}

static int gpio_rts5918_port_set_bits_raw(const struct device *port, gpio_port_pins_t pins)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = config->reg_base;
	uint32_t pin = 0;

	pins &= 0x0000FFFF;
	gpio_port_pins_t sel_pin = 1;

	for (; pins;) {
		if (pins & sel_pin) {
			pin_output_high(port, pin);
		}
		pins &= ~sel_pin;
		sel_pin <<= 1;
		gcr++;
		pin++;
	}

	return 0;
}

static int gpio_rts5918_port_clear_bits_raw(const struct device *port, gpio_port_pins_t pins)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = config->reg_base;
	uint32_t pin = 0;

	pins &= 0x0000FFFF;
	gpio_port_pins_t sel_pin = 1;

	for (; pins;) {
		if (pins & sel_pin) {
			pin_output_low(port, pin);
		}
		pins &= ~sel_pin;
		sel_pin <<= 1;
		gcr++;
		pin++;
	}

	return 0;
}

static int gpio_rts5918_port_toggle_bits(const struct device *port, gpio_port_pins_t pins)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = config->reg_base;
	uint32_t pin = 0;

	pins &= 0x0000FFFF;
	gpio_port_pins_t sel_pin = 0x1UL;

	for (; pins;) {
		if (pins & sel_pin) {
			if (*gcr & GPIO_GCR_OUTCTRL_Msk) {
				pin_output_low(port, pin);
			} else {
				pin_output_high(port, pin);
			}
		}

		pins &= ~sel_pin;
		sel_pin <<= 1;
		gcr++;
		pin++;
	}

	return 0;
}

static gpio_pin_t gpio_rts5918_get_intr_pin(volatile uint32_t *reg_base)
{
	gpio_pin_t pin = 0U;

	for (; pin < 16; pin++) {
		if (reg_base[pin] & GPIO_GCR_INTSTS_Msk) {
			break;
		}
	}

	return pin;
}

static void gpio_rts5918_isr(const void *arg)
{
// #if RTS5918_GPIO_HAS_KBD
// 	const struct device *kbd_dev = DEVICE_DT_GET(DT_NODELABEL(kbd));
// #endif
	const struct device *port = arg;
	const struct gpio_rts5918_config *config = port->config;
	struct gpio_rts5918_data *data = port->data;
	volatile uint32_t *gcr = config->reg_base;
	unsigned int key = irq_lock();

	for (int pin = 0; pin < config->num_pins ; pin++){
		if (gcr[pin] & GPIO_GCR_INTSTS_Msk) {
			gcr[pin] |= GPIO_GCR_INTSTS_Msk;
// #if RTS5918_GPIO_HAS_KBD
// 			if (((uintptr_t)&gcr[pin] == 0x40230170UL) ||
// 			    ((uintptr_t)&gcr[pin] == 0x40230174UL)) {
// 				input_kbd_matrix_poll_start(kbd_dev);
// 			}
// #endif
			gpio_fire_callbacks(&data->callbacks, port, BIT(pin));
		}
	}

	irq_unlock(key);
}

static int gpio_rts5918_intr_config(const struct device *port, gpio_pin_t pin,
				    enum gpio_int_mode mode, enum gpio_int_trig trig)
{
	const struct gpio_rts5918_config *config = port->config;
	volatile uint32_t *gcr = &config->reg_base[pin];
	uint32_t cfg_val = *gcr;
	uint32_t pin_index = config->irq_num;
	uint32_t interrupt_en_check = 0;

	int err = pin_is_valid(config, pin);

	if (err) {
		return err;
	}

	switch (mode) {
	case GPIO_INT_MODE_DISABLED:
#ifdef CONFIG_GPIO_ENABLE_DISABLE_INTERRUPT
	case GPIO_INT_MODE_DISABLE_ONLY:
		/* DISABLE_ONLY clears INTEN without altering INTCTRL, so a
		 * later ENABLE_ONLY can resume with the previously configured
		 * trigger. Behaviour is identical to DISABLED on this driver
		 * because DISABLED also leaves INTCTRL untouched.
		 */
#endif
		{
			/* Race fix: clearing this pin's INTEN, scanning the bank's
			 * remaining INTENs, and conditionally calling irq_disable()
			 * must be one critical section. Otherwise a concurrent caller
			 * enabling INTEN on a different pin between the scan and the
			 * NVIC disable would have its enable silently undone (the
			 * vector gets masked even though that other pin still wants
			 * it). Holding irq_lock here also stops the GPIO ISR from
			 * firing mid-update on this same bank.
			 */
			unsigned int key = irq_lock();
			cfg_val &= ~GPIO_GCR_INTEN_Msk;
			*gcr = cfg_val;
			for (int i = 0; i < config->num_pins; i++) {
				interrupt_en_check |= config->reg_base[i];
			}
			if ((interrupt_en_check & GPIO_GCR_INTEN_Msk) == 0x0) {
				irq_disable(pin_index);
			}
			irq_unlock(key);
		}
		return 0;
#ifdef CONFIG_GPIO_ENABLE_DISABLE_INTERRUPT
	case GPIO_INT_MODE_ENABLE_ONLY:
		/* Re-enable a previously configured interrupt without touching
		 * INTCTRL. Caller is expected to have configured the trigger
		 * earlier via GPIO_INT_MODE_LEVEL / _EDGE.
		 *
		 * Also W1C any stale INTSTS in the same write — between an
		 * earlier DISABLE_ONLY and this ENABLE_ONLY the pin level may
		 * have transitioned and latched a pending bit, which would
		 * otherwise fire a spurious ISR the moment INTEN goes back
		 * high. Setting INTSTS here writes 1 to clear (W1C); INTEN is
		 * RW so the same write asserts the enable.
		 */
		cfg_val |= GPIO_GCR_INTSTS_Msk | GPIO_GCR_INTEN_Msk;
		*gcr = cfg_val;
		irq_enable(pin_index);
		return 0;
#endif
	case GPIO_INT_MODE_LEVEL:
		switch (trig) {
		case GPIO_INT_TRIG_LOW:
			cfg_val &= ~GPIO_GCR_INTCTRL_Msk;
			cfg_val |= GPIO_GCR_INTCTRL_TRIG_LEVEL_LOW;
			break;
		case GPIO_INT_TRIG_HIGH:
			cfg_val &= ~GPIO_GCR_INTCTRL_Msk;
			cfg_val |= GPIO_GCR_INTCTRL_TRIG_LEVEL_HIGH;
			break;
		default:
			return -EINVAL;
		}
		break;
	case GPIO_INT_MODE_EDGE:
		switch (trig) {
		case GPIO_INT_TRIG_LOW:
			cfg_val &= ~GPIO_GCR_INTCTRL_Msk;
			cfg_val |= GPIO_GCR_INTCTRL_TRIG_EDGE_LOW;
			break;
		case GPIO_INT_TRIG_HIGH:
			cfg_val &= ~GPIO_GCR_INTCTRL_Msk;
			cfg_val |= GPIO_GCR_INTCTRL_TRIG_EDGE_HIGH;
			break;
		case GPIO_INT_TRIG_BOTH:
			cfg_val &= ~GPIO_GCR_INTCTRL_Msk;
			cfg_val |= GPIO_GCR_INTCTRL_TRIG_EDGE_BOTH;
			break;
		default:
			return -EINVAL;
		}
		break;
	default:
		return -EINVAL;
	}

	/* enable interrupt */
	cfg_val |= GPIO_GCR_INTEN_Msk;
	/* set value to GPIO register */
	*gcr = cfg_val;

	irq_enable(pin_index);

	return 0;
}

static int gpio_rts5918_manage_cb(const struct device *port, struct gpio_callback *cb, bool set)
{
	struct gpio_rts5918_data *data = port->data;

	return gpio_manage_callback(&data->callbacks, cb, set);
}

//static DEVICE_API(gpio, gpio_rts5918_driver_api) = {
static const struct gpio_driver_api gpio_rts5918_driver_api = {
	.pin_configure = gpio_rts5918_configuration,
#ifdef CONFIG_GPIO_GET_CONFIG
	.pin_get_config = gpio_rts5918_get_configuration,
#endif
	.port_get_raw = gpio_rts5918_port_get_raw,
	.port_set_masked_raw = gpio_rts5918_port_set_masked_raw,
	.port_set_bits_raw = gpio_rts5918_port_set_bits_raw,
	.port_clear_bits_raw = gpio_rts5918_port_clear_bits_raw,
	.port_toggle_bits = gpio_rts5918_port_toggle_bits,
	.pin_interrupt_configure = gpio_rts5918_intr_config,
	.manage_callback = gpio_rts5918_manage_cb,
};


#define RTS5918_GPIO_DTNAMIC_IRQ(id)                                                               \
	IRQ_CONNECT(DT_INST_IRQN(id), DT_INST_IRQ(id, priority), gpio_rts5918_isr,                 \
		    DEVICE_DT_INST_GET(id), 0U);

#define GPIO_RTS5918_INIT(id)                                                                      \
	static int gpio_rts5918_init_##id(const struct device *dev)                                \
	{                                                                                          \
		if (!(DT_INST_IRQ_HAS_CELL(id, irq))) {                                            \
			return 0;                                                                  \
		}                                                                                  \
                                                                                                   \
		RTS5918_GPIO_DTNAMIC_IRQ(id)                                                       \
                                                                                                   \
		return 0;                                                                          \
	}                                                                                          \
                                                                                                   \
	static struct gpio_rts5918_data gpio_rts5918_data_##id;                                    \
                                                                                                   \
	static const struct gpio_rts5918_config gpio_rts5918_config_##id = {                       \
		.common = {.port_pin_mask = GPIO_PORT_PIN_MASK_FROM_DT_INST(id)},                  \
		.reg_base = (volatile uint32_t *)DT_INST_REG_ADDR(id),                             \
		.num_pins = DT_INST_PROP(id, ngpios),                                              \
		.irq_num = DT_INST_IRQN(id),                                               \
	};                                                                                         \
	DEVICE_DT_INST_DEFINE(id, gpio_rts5918_init_##id, NULL, &gpio_rts5918_data_##id,           \
			      &gpio_rts5918_config_##id, POST_KERNEL, CONFIG_GPIO_INIT_PRIORITY,   \
			      &gpio_rts5918_driver_api);

DT_INST_FOREACH_STATUS_OKAY(GPIO_RTS5918_INIT)
