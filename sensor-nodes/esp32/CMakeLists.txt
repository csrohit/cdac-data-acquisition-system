# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(BOARD esp32)

set($ENV{ZEPHYR_TOOLCHAIN_VARIANT} espressif)
set($ENV{ESPRESSIF_TOOLCHAIN_PATH} /home/rohit/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf)

set($ENV{ZEPHYR_BASE} /home/rohit/code/zephyrproject/zephyr)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE src/main.c)
