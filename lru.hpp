#ifndef LRU_HPP
#define LRU_HPP

#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

template<typename key_t, typename value_t>
class lru {
public:
	typedef typename std::pair<key_t, value_t> key_value_pair_t;
	typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

	lru(size_t max_size):max_size(max_size){}

	// lru(const lru& l) {
	// 	items_list = l.items_list;
	// 	items_map = l.items_map;
	// 	max_size = l.max_size;
	// }

	void put(const key_t& key, const value_t& value) {
		auto it = items_map.find(key);
		items_list.push_front(key_value_pair_t(key, value));
		if(it != items_map.end()) {
			items_list.erase(it->second);
			items_map.erase(it);
		}
		items_map[key] = items_list.begin();

		if(items_map.size() > max_size) {
			auto last = items_list.end();
			last--;
			items_map.erase(last->first);
			items_list.pop_back();
		}
	}

	value_t& get(const key_t& key) {
		auto it = items_map.find(key);
		if(it == items_map.end()) {
			throw std::range_error("There is no such key in cache");
		} else {
			items_list.splice(items_list.begin(), items_list, it->second);
			return it->second->second;
		}
	}

	std::list<key_value_pair_t>& getList() {
		return items_list;
	}

	void traverse() {
		std::cout << "traverse" << std::endl;
		for(auto it = items_list.begin(); it != items_list.end(); ++it) {
			std::cout << it->first << " : " << it->second << std::endl;
		}
	}

	bool exists(const key_t& key) const {
		return items_map.find(key) != items_map.end();
	}

	size_t size() const {
		return items_map.size();
	}

private:
	std::list<key_value_pair_t> items_list;
	std::unordered_map<key_t, list_iterator_t> items_map;
	size_t max_size;
};

#endif