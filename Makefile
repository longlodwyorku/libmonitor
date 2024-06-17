FLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic $(NIX_CFLAGS_COMPILE)
NCC = c++
CC = g++

shared/libmonitor.so: obj/monitor.o
	mkdir -p shared
	$(CC) -shared -o $@ $^

obj/nixmonitor.o: src/monitor.cpp include/monitor.hpp
	mkdir -p obj
	$(NCC) -c -fPIC -o $@ $<

shared/libnixmonitor.so: obj/nixmonitor.o 
	mkdir -p shared
	$(NCC) -shared -o $@ $^

obj/monitor.o: src/monitor.cpp include/monitor.hpp
	mkdir -p obj
	$(CC) -c -fPIC -o $@ $<

bin/nixsample: examples/sample.cpp shared/libnixmonitor.so
	mkdir -p bin
	$(CC) $(FLAGS) -o bin/sample examples/sample.cpp -L ./shared -lmonitor -Wl,-rpath=./shared/

bin/sample: examples/sample.cpp shared/libmonitor.so
	mkdir -p bin
	$(CC) $(FLAGS) -o bin/sample examples/sample.cpp -L ./shared -lmonitor -Wl,-rpath=./shared/


install: shared/libmonitor.so include/monitor.hpp
	mkdir -p $(DESTDIR)/usr/lib64 $(DESTDIR)/usr/include
	install $< $(DESTDIR)/usr/lib64/libmonitor.so.0
	install $< $(DESTDIR)/usr/lib64/libmonitor.so
	install $(word 2, $^) $(DESTDIR)/usr/include/

clean:
	rm -rf obj/* bin/* shared/*
