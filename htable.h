#ifndef H_TABLE_H_
#define H_TABLE_H_

#include <vector>
#include <list>

template <class T, class S>
class HTable {
public:
	class iterator {
		friend class HTable;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
	public:
		iterator() : list_it{}, key_vector_it{}, data_vector_it{}, iter{} {}
		iterator(const iterator&);
		std::pair<T,S>& operator*() const;
		std::pair<T,S>* operator->() const;
		iterator& operator=(const iterator&);
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) const;
		bool operator!=(const iterator& other) const;
	private:
		typename std::list<std::pair<T, S>>::iterator list_it;
		typename std::vector<T>::iterator key_vector_it;
		typename std::vector<std::list<std::pair<T,S>>>::iterator data_vector_it;
		HTable<T, S>* iter;
	};
	friend class HTable<T,S>::iterator;
public:
	HTable() {}
	HTable(const HTable&);
	HTable(HTable&&);
	HTable& operator=(const HTable&);
	HTable operator=(HTable&&);
	S& operator[](const T& key);
	iterator find(const T& key);
	void insert(const T& key, const S& value);
	void erase(const T& key);
	iterator begin();
	iterator end();
	int getSize() const { return capacity; }
	std::vector<std::list<std::pair<T, S>>> getData() const { return data; }
	std::vector<T> getKeys() const { return keys; }
private:
	std::vector<std::list<std::pair<T, S>>> data{ 100 };
    std::vector<T> keys;
	int capacity = 100;
    static int setIndex(const T& key);
	void rehash();
	friend class iterator;
};

#include "htable.hpp"

#endif // H_TABLE_H_
