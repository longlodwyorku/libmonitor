FLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic $(NIX_CFLAGS_COMPILE)
NCC = c++
CC = g++

nixmonitor.o: src/monitor.cpp include/monitor.hpp
	$(NCC) -c -fPIC -o obj/monitor.o src/monitor.cpp

libnixmonitor.so: nixmonitor.o include/monitor.hpp
	$(NCC) -shared -o shared/libmonitor.so obj/monitor.o

monitor.o: src/monitor.cpp include/monitor.hpp
	$(CC) -c -fPIC -o obj/monitor.o src/monitor.cpp

libmonitor.so: nixmonitor.o include/monitor.hpp
	$(CC) -shared -o shared/libmonitor.so obj/monitor.o

sample: examples/sample.cpp libnixmonitor.so
	$(CC) $(FLAGS) -o bin/sample examples/sample.cpp -L ./shared -lmonitor -Wl,-rpath=./shared/

clean:
	rm -rf obj/* bin/* shared/*
