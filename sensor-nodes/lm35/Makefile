source:
	cmake -Brohit

build: source
	${MAKE} -C rohit -j11

only:
	${MAKE} -C rohit -j11


uf2:
	cp rohit/zephyr/zephyr.uf2 /media/chin2/RPI-RP2/	

flash:
	openocd -d2 -s /usr/local/bin/openocd/scripts -f interface/stlink.cfg -c "transport select hla_swd" -f target/stm32f1x.cfg -c "program {./rohit/zephyr/zephyr.elf}  verify reset; shutdown;"

clean:
	rm -rf rohit

.phony: flash clean