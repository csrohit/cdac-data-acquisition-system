/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <zephyr/drivers/gpio.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static struct adc_dt_spec lm35 = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led_yellow)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void main(void)
{
	int ret;

	if (!device_is_ready(led.port))
	{
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return;
	}

	int err;
	int16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

	if (!device_is_ready(lm35.dev))
	{
		printk("ADC controller device not ready\n");
		return;
	}

	err = adc_channel_setup_dt(&lm35);
	if (err < 0)
	{
		printk("Could not setup channel  (%d)\n", err);
		return;
	}

	while (1)
	{
		printk("ADC reading:\n");
		int32_t val_mv;

		printk("- %s, channel %d: ",
			   lm35.dev->name,
			   lm35.channel_id);

		(void)adc_sequence_init_dt(&lm35, &sequence);

		err = adc_read(lm35.dev, &sequence);
		if (err < 0)
		{
			printk("Could not read (%d)\n", err);
			continue;
		}
		else
		{
			printk("%" PRId16, buf);
		}

		/* conversion to mV may not be supported, skip if not */
		val_mv = buf;
		err = adc_raw_to_millivolts_dt(&lm35,
									   &val_mv);
		if (err < 0)
		{
			printk(" (value in mV not available)\n");
		}
		else
		{
			printk(" = %" PRId32 " mV\n", val_mv);
		}
		k_sleep(K_MSEC(1000));
	}

	while (1)
	{
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0)
		{
			return;
		}
		k_msleep(SLEEP_TIME_MS);
	}
}
