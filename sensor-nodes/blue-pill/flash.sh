#!/bin/sh

	openocd -d2 -s /opt/openocd/scripts -f interface/stlink.cfg -c "transport select hla_swd" -f board/stm32f103c8_blue_pill.cfg -c "program {./rohit/zephyr/zephyr.elf}  verify reset; shutdown;"