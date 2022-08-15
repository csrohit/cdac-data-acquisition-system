/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7


#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

	/* Data of ADC io-channels specified in devicetree. */
static struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};



void blink(void *, void *);
const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(DT_ALIAS(led_built_in), gpios);
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led_red), gpios);

void main(void)
{
	
		int err;
	int16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

	/* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!device_is_ready(adc_channels[i].dev)) {
			printk("ADC controller device not ready\n");
			return;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);
			return;
		}
	}

	while (1) {
		printk("ADC reading:\n");
		for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
			int32_t val_mv;

			printk("- %s, channel %d: ",
			       adc_channels[i].dev->name,
			       adc_channels[i].channel_id);

			(void)adc_sequence_init_dt(&adc_channels[i], &sequence);

			err = adc_read(adc_channels[i].dev, &sequence);
			if (err < 0) {
				printk("Could not read (%d)\n", err);
				continue;
			} else {
				printk("%"PRId16, buf);
			}

			/* conversion to mV may not be supported, skip if not */
			val_mv = buf;
			err = adc_raw_to_millivolts_dt(&adc_channels[i],
						       &val_mv);
			if (err < 0) {
				printk(" (value in mV not available)\n");
			} else {
				printk(" = %"PRId32" mV\n", val_mv);
			}
		}

		k_sleep(K_MSEC(1000));
	}

}



/**
 * @brief Blink task to verfy that scheduler is working
 * 
 */
K_THREAD_DEFINE(blink0_id, STACKSIZE, blink, &led_blue, 1000, NULL, PRIORITY, 0, 0);

void blink(void * p1, void *p2)
{
	int ret;
	struct gpio_dt_spec *spec = (struct gpio_dt_spec *)p1;
	int sleep_ms = (int)p2;
	if (!device_is_ready(spec->port))
	{
		printk("Error: %s device is not ready\n", spec->port->name);
		return;
	}

	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	if (ret != 0)
	{
		printk("Error %d: failed to configure pin %d \n",
			   ret, spec->pin);
		return;
	}

	while (1)
	{
		gpio_pin_toggle_dt(spec);
		k_msleep(sleep_ms);
	}
}
