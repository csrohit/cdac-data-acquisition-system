BUILD_DIR:= rohit


source:
	cmake -Brohit

build: source
	${MAKE} -C ${BUILD_DIR} -j11

flash:
	bash flash.sh

flash-link:
	st-flash write ./${BUILD_DIR}/zephyr/zephyr.bin 0x08000000 

flash-serial:
	stm32flash -g 0x08000000 -b 115200 -w ./${BUILD_DIR}/zephyr/zephyr.bin "/dev/ttyUSB0"

monitor:
	minicom --device /dev/ttyUSB0

clean:
	rm -rf ${BUILD_DIR}

.phony: flash clean