# HashTable

## What is hash table
![Hash_table_3_1_1_0_1_0_0_SP svg](https://user-images.githubusercontent.com/114210044/230922890-b0d5e7c5-ab95-4a48-aead-f885e9ab08e7.png)

> Hash Table is a data structure which stores information in form of (key,value) pairs. It associates each key with appropriate value. A hash table uses a hash function to compute an index, also called a hash code, into an array of buckets or slots, from which the desired value can be found. Ideally, the hash function will assign each key to a unique bucket, but most hash table designs (and my implementation is not an exception) employ an imperfect hash function, which might cause hash collisions where the hash function generates the same index for more than one key. 

## Implementation

> There are many versions of hash table implementation and all use different logic and underlying types. In this implementation are used STL containers: 
- std::pair
- std::vector
- std::list
- iterators of all mentioned containers

Also is defined a custom iterator for this hash table. It allows to iterate on hash table in the order of inserted keys. 

## Defined functions

-- For hash table
- `HTable()`                                  default constructor
- `HTable(const HTable&)`                     copy constructor
- `HTable(HTable&&)`                          move constructor
- `find(const T&)`                            returns iterator to given key
- `insert(const T&,const S&)`                 inserts new pair
- `erase(const T&)`                           erase value
- `begin()`                                   returns begin iterator
- `end()`                                     returns end iterator
- `setIndex(const T&)`                     calculates hash value for given key
- `rehash()`     extends hash table and calculates new hash values

## Defined operators

-- For hash table
- `operator=(const HTable&)` copy assignement operator
- `operator=(HTable&&)` move assignement operator
- `operator[](const T&)`   subscript operator for key-based access

-- For iterator

- `iterator()`  default constructor
- `iterator(const iterator&)`   copy constructor
- `operator*()`   dereferencing operator
-	`operator->()`  arrow operator
-	`operator=(const iterator&)`   copy operator assignment
-	`operator++()`     prefix increment operator
-	`operator++(int)`  postfix increment operator
-	`operator--()`     prefix decrement operator
-	`operator--(int)`  postfix decrement operator
- `operator==(const iterator&)` equality operator
- `operator!=(const iterator&)` inequality operator

## How to use this hash table in your project
- git clone https://github.com/VPanchoyan/HashTable
