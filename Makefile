CC=g++
CPPFLAGS=-Wall -std=c++11 -g -I. -I./pcap -I./boost/include/boost-1_61
LDFLAGS=-pthread -L. -static-libstdc++ -Wl,--rpath=./lib -Wl,--dynamic-linker=./lib/ld-linux-x86-64.so.2

monitor : main.o websocket_server.o httpd.o utils.o post_data.o libjson.a libboost_chrono.a libboost_system.a libtins.a libpcap.a
	$(CC) -o $@ $^ $(LDFLAGS)

main.o : main.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
post_data.o : post_data.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
utils.o : utils.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^

websocket_server.o: websocket_server.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
httpd.o: httpd.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^


.PHONY:
	clean libjson.a

tar:
	tar -czvf monitor.tar.gz monitor static lib

clean:
	rm -f monitor *.o
