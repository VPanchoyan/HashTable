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

template<class T>
int hashValue(const T& value) {
	return std::hash<T>{}(value);
}

template <class T, class S>
void HTable<T,S>::rehash() {}

template <class T, class S> 
int HTable<T, S>::setIndex(const T& key) {
	return abs(hashValue(key)) % 100;
}

template <class T, class S>
HTable<T, S>::HTable(const HTable& ht) {
	data = ht.data;
	keys = ht.keys;
	capacity = ht.capacity;
}

template <class T, class S>
HTable<T, S>::HTable(HTable&& temp_ht) {
	data = std::move(temp_ht.data);
	keys = std::move(temp_ht.keys);
	capacity = std::move(temp_ht.capacity);
}

template <class T, class S>
HTable<T, S>& HTable<T, S>::operator=(const HTable& ht) {
	data = ht.data;
	keys = ht.keys;
	capacity = ht.capacity;
}

template <class T, class S>
HTable<T, S> HTable<T, S>::operator=(HTable&& temp_ht) {
	data = std::move(temp_ht.data);
	keys = std::move(temp_ht.keys);
	capacity = std::move(temp_ht.capacity);
}

template <class T, class S>
S& HTable<T, S>::operator[](const T& key) {
	int index = setIndex(key);
	if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
		keys.push_back(key);
		S value{};
		if (data[index].empty()) {
			std::pair<T, S> obj;
			obj.first = key;
			obj.second = value;
			std::list<std::pair<T, S>> bucket;
			bucket.push_front(obj);
			data[index] = bucket;
			return (data[index].front()).second;
		}
		else {
			std::pair<T, S> obj;
			obj.first = key;
			obj.second = value;
			data[index].push_front(obj);
			return (data[index].front()).second;
		}
	}
	else {
		iterator it = find(key);
		return it->second;
	}
}

template <class T, class S>
typename HTable<T, S>::iterator HTable<T, S>::find(const T& key) {
	HTable<T, S>::iterator it{};
	it.key_vector_it = std::find(keys.begin(), keys.end(), key);
	if (it.key_vector_it == keys.end()) {
		return end();
	}
	int index = setIndex(*it.key_vector_it);
	it.data_vector_it = data.begin() + index;
	it.list_it = data[index].begin();
	while ((*it.list_it).first != key) {
		++it.list_it;
	}
	it.iter = this;
	return it;
}

template <class T, class S>
void HTable<T, S>::insert(const T& key, const S& value) {
	int index = setIndex(key);
	if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
		keys.push_back(key);
		if (data[index].empty()) {
			std::pair<T, S> obj;
			obj.first = key;
			obj.second = value;
			std::list<std::pair<T, S>> bucket;
			bucket.push_front(obj);
			data[index] = bucket;
		}
		else {
			std::pair<T, S> obj;
			obj.first = key;
			obj.second = value;
			data[index].push_front(obj);
		}
	}
	else {
		(*this)[key] = value;
	}
}

template <class T, class S>
void HTable<T, S>::erase(const T& key) {
	if (std::find(keys.begin(), keys.end(), key) == keys.end()) return;
	iterator it = find(key);
	(*it.data_vector_it).erase(it.list_it);
	keys.erase(it.key_vector_it);
}

template <class T, class S>
typename HTable<T, S>::iterator HTable<T, S>::begin() {
	iterator beg{};
	if (!keys.empty())	beg = find(keys[0]);
	else {
		beg.data_vector_it = data.begin();
		beg.key_vector_it = keys.begin();
		beg.list_it = data[0].begin();
		beg.iter = this;
	}
	return beg;
}

template <class T, class S>
typename HTable<T, S>::iterator HTable<T, S>::end() {
	iterator beg{};
	int index = setIndex(keys[keys.size() - 1]);
	beg.key_vector_it = keys.end();
	beg.data_vector_it = data.end() - 1;
	beg.list_it = data[index].end();
	beg.iter = this;
	return beg;
}

template <class T, class S>
std::pair<T, S>& HTable<T, S>::iterator::operator*() const {
	return *list_it;
}

template <class T, class S>
std::pair<T, S>* HTable<T, S>::iterator::operator->() const {
	return &(*list_it);
}

template <class T, class S>
typename HTable<T, S>::iterator& HTable<T, S>::iterator::operator++() {
	if (key_vector_it + 1 != (this->iter)->keys.end()) {
		++key_vector_it;
		*this = (this->iter)->find(*key_vector_it);
	}
	else {
		*this = this->iter->end();
	}
	return *this;
}

template <class T, class S>
typename HTable<T, S>::iterator HTable<T, S>::iterator::operator++(int) {
	typename HTable<T, S>::iterator temp;
	temp = *this;
	if (key_vector_it + 1 != (this->iter)->keys.end()) {
		++key_vector_it;
		*this = (this->iter)->find(*key_vector_it);
	}
	else {
		*this = this->iter->end();
	}
	return temp;
}

template <class T, class S>
typename HTable<T, S>::iterator& HTable<T, S>::iterator::operator--() {
	if (key_vector_it != (this->iter)->keys.begin()) {
		--key_vector_it;
		*this = (this->iter)->find(*key_vector_it);
	}
	return *this;
}

template <class T, class S>
typename HTable<T, S>::iterator HTable<T, S>::iterator::operator--(int) {
	typename HTable<T, S>::iterator temp;
	temp = *this;
	if (key_vector_it != (this->iter)->keys.begin()) {
		--key_vector_it;
		*this = (this->iter)->find(*key_vector_it);
	}
	return temp;
}

template <class T, class S>
bool HTable<T, S>::iterator::operator==(const HTable<T, S>::iterator& other) const {
	return (*this == other);
}

template <class T, class S>
bool HTable<T, S>::iterator::operator!=(const HTable<T, S>::iterator& other) const {
	return (iter != other.iter || key_vector_it != other.key_vector_it || data_vector_it != other.data_vector_it);
}

template <class T, class S>
typename HTable<T, S>::iterator& HTable<T, S>::iterator::operator=(const iterator& other) {
	if (this != &other) {
		data_vector_it = other.data_vector_it;
		key_vector_it = other.key_vector_it;
		list_it = other.list_it;
		iter = other.iter;
	}
	return *this;
}

template <class T, class S>
HTable<T, S>::iterator::iterator(const iterator& other) {
	data_vector_it = other.data_vector_it;
	key_vector_it = other.key_vector_it;
	list_it = other.list_it;
	iter = other.iter;
}


#endif // H_TABLE_H_