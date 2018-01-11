all:
        gcc webserver.c -o webserver.o
        gcc mosquitto_sub.c -o sub.o -lmosquitto
        gcc mosquitto_pub2.c -o pub2.o -lmosquitto
        gcc mosquitto_pub1.c -o pub1.o -lmosquitto
