

all: subscriber.o
subscriber.o: subscriber.c
	gcc $^ paho/lib/libpaho-mqtt3c.a -o $@ -I./paho/include -lpthread


clean:
	rm main.o