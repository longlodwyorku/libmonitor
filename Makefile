FLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic $(NIX_CFLAGS_COMPILE)
NCC = c++
CC = g++

libmonitor.so: monitor.o include/monitor.hpp
	mkdir -p shared
	$(CC) -shared -o shared/libmonitor.so obj/monitor.o

nixmonitor.o: src/monitor.cpp include/monitor.hpp
	mkdir -p obj
	$(NCC) -c -fPIC -o obj/monitor.o src/monitor.cpp

libnixmonitor.so: nixmonitor.o include/monitor.hpp
	mkdir -p shared
	$(NCC) -shared -o shared/libmonitor.so obj/monitor.o

monitor.o: src/monitor.cpp include/monitor.hpp
	mkdir -p obj
	$(CC) -c -fPIC -o obj/monitor.o src/monitor.cpp

sample: examples/sample.cpp libnixmonitor.so
	mkdir -p bin
	$(CC) $(FLAGS) -o bin/sample examples/sample.cpp -L ./shared -lmonitor -Wl,-rpath=./shared/

install: libmonitor.so include/monitor.hpp
	mkdir -p $(DESTDIR)/usr/lib64
	install shared/libmonitor.so $(DESTDIR)/usr/lib64/
	install include/monitor.hpp $(DESTDIR)/usr/include/

clean:
	rm -rf obj/* bin/* shared/*
