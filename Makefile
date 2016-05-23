CC=g++
CPPFLAGS=-std=c++11
LDFLAGS=-pthread

main : main.o utils.o proc_main.o post_data.o proc_stat.o proc_meminfo.o proc_vmstat.o proc_netstat.o data.o cpu.o meminfo.o vmstat.o netstat.o
	$(CC) $(LDFLAGS) -o $@ $^

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

data.o : data.cpp
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
	clean

clean:
	rm main *.o