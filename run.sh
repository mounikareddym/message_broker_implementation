#!/bin/bash
rm sub.o pub1.o pub2.o webserver.o
make
./sub.o &
./pub1.o &
./pub2.o &
./webserver.o &