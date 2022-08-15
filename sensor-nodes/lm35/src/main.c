/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

void blink(void *, void *);
const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(DT_ALIAS(led_built_in), gpios);
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led_red), gpios);

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

	while (1)
	{
		gpio_pin_toggle_dt(&led);

		k_msleep(500);
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
