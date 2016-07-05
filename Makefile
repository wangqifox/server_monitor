CC=g++
CPPFLAGS=-Wall -std=c++11 -g -I. -I./boost/include/boost-1_61
# LDFLAGS=-static -pthread -lboost_system -lboost_filesystem -lboost_random -lrt -lboost_timer -lboost_chrono -lcurl
# LDFLAGS=-pthread -lboost_system -lboost_chrono
LDFLAGS=-pthread -L./boost/lib -lboost_system-gcc47-mt-1_61 -lboost_chrono-gcc47-mt-1_61

monitor : main.o websocket_server.o utils.o proc_main.o post_data.o proc_stat.o proc_meminfo.o proc_vmstat.o proc_netstat.o cpu.o meminfo.o vmstat.o netstat.o libjson.a 
	$(CC) -o $@ $^ $(LDFLAGS)

main.o : main.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
proc_main.o : proc_main.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
post_data.o : post_data.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
proc_stat.o : proc_stat.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
proc_meminfo.o : proc_meminfo.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
proc_vmstat.o : proc_vmstat.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
proc_netstat.o : proc_netstat.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
utils.o : utils.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^

websocket_server.o: websocket_server.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^

cpu.o : cpu.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
meminfo.o : meminfo.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
vmstat.o : vmstat.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^
netstat.o : netstat.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^

.PHONY:
	clean libjson.a

clean:
	rm monitor *.o
