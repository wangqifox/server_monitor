#ifndef SNIFFER_HPP
#define SNIFFER_HPP

#include <iostream>
#include <tins/tins.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <list>
#include <mutex>
#include "lru.hpp"

using namespace Tins;

class Traffic{
private:
	IP::address_type address;
	ulong speed_in;
	ulong speed_out;
	ulong total_in;
	ulong total_out;
public:
	Traffic(IP::address_type address):speed_in(0), speed_out(0), total_in(0), total_out(0) {
		this->address = address;
	}

	void addTotalIn(ulong in) {
		this->total_in += in;
	}

	void addTotalOut(ulong out) {
		this->total_out += out;
	}

	void addSpeedIn(ulong in) {
		this->speed_in += in;
		addTotalIn(in);
	}

	void addSpeedOut(ulong out) {
		this->speed_out += out;
		addTotalOut(out);
	}

	void clearSpeed() {
		this->speed_in = 0;
		this->speed_out = 0;
	}

	std::string getAddress() {
		return address.to_string();
	}

	ulong getTotalIn() {
		return total_in;
	}

	ulong getTotalOut() {
		return total_out;
	}

	ulong getSpeedIn() {
		return speed_in;
	}

	ulong getSpeedOut() {
		return speed_out;
	}

	friend std::ostream& operator<< (std::ostream &os, const Traffic &traffic) {
		os << traffic.speed_in << " " << traffic.speed_out << " " << traffic.total_in << " " << traffic.total_out << std::endl;
		return os;
	}
};

// std::ostream& operator<< (std::ostream &os, const Traffic &traffic) {
// 	os << traffic.speed_in << " " << traffic.speed_out << " " << traffic.total_in << " " << traffic.total_out << std::endl;
// 	return os;
// }

class TrafficData {
private:
	lru<IP::address_type, Traffic> traffic_buffer;
	std::mutex mtx;
public:
	TrafficData(size_t size):traffic_buffer(size) {}

	TrafficData(const TrafficData& t):traffic_buffer(t.traffic_buffer){}

	void addIn(IP::address_type address, int in) {
		unique_lock<mutex> lock(mtx);
		if(traffic_buffer.exists(address)) {
			Traffic& traffic = traffic_buffer.get(address);
			traffic.addSpeedIn(in);
			traffic_buffer.put(address, traffic);
		} else {
			Traffic traffic(address);
			traffic.addSpeedIn(in);
			traffic_buffer.put(address, traffic);
		}
		lock.unlock();
	}

	void addOut(IP::address_type address, int out) {
		unique_lock<mutex> lock(mtx);
		if(traffic_buffer.exists(address)) {
			Traffic& traffic = traffic_buffer.get(address);
			traffic.addSpeedOut(out);
			traffic_buffer.put(address, traffic);
		} else {
			Traffic traffic(address);
			traffic.addSpeedOut(out);
			traffic_buffer.put(address, traffic);
		}
		lock.unlock();
	}

	void clearSpeed() {
		std::list<std::pair<IP::address_type, Traffic> >& traffic_list = traffic_buffer.getList();
		for(auto it = traffic_list.begin(); it != traffic_list.end(); ++it) {
			it->second.clearSpeed();
		}
	}

	void traverse() {
		std::list<std::pair<IP::address_type, Traffic> >& traffic_list = traffic_buffer.getList();
		for(auto it = traffic_list.begin(); it != traffic_list.end(); ++it) {
			std::cout << it->first << " : " << it->second << std::endl;
		}
	}

	std::list<std::pair<IP::address_type, Traffic> > getList() {
		unique_lock<mutex> lock(mtx);
		std::list<std::pair<IP::address_type, Traffic> > traffic_list = traffic_buffer.getList();
		lock.unlock();
		return traffic_list;
	}


};

#endif