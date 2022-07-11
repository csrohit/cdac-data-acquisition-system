

target:= subscriber
CC:= gcc
LDFLAGS:= -lmosquitto

${target}.o: ${target}.c
	${CC} $^ -o $@ ${LDFLAGS}


clean: 
	rm -rf *.o


.phony: clean

run: ${target}.o
	./$^