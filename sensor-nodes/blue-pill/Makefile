BUILD_DIR:= rohit


source:
	cmake -Brohit

only:
	${MAKE} -C ${BUILD_DIR} -j11

build: source
	${MAKE} -C ${BUILD_DIR} -j11

flash:
		openocd -d2 -s /usr/local/bin/openocd/scripts -f interface/stlink.cfg -c "transport select hla_swd" -f target/stm32f4x.cfg -c "program {./rohit/zephyr/zephyr.elf}  verify reset; shutdown;"


flash-link:
	st-flash write ./${BUILD_DIR}/zephyr/zephyr.bin 0x08000000 

flash-serial:
	stm32flash -g 0x08000000 -b 115200 -w ./${BUILD_DIR}/zephyr/zephyr.bin "/dev/ttyUSB0"

monitor:
	minicom --device /dev/ttyUSB0

clean:
	rm -rf ${BUILD_DIR}

.phony: flash clean