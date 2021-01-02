#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "two_way_list.hpp"
#include "two_way_list_node.hpp"

#define INITIAL_CAPACITY 16
#define DEFAULT_UPPER_FACTOR 0.75f
#define DEFAULT_LOWER_FACTOR 0.25f
#define INCREASE_SIZE 1
#define DECREASE_SIZE -1
#define MIN_HASH_TABLE_SIZE 1
#define INITIAL_SIZE 0
#define INITIAL_FACTOR 0
#define HASH_TABLE_CHANGE 2

typedef enum hashTableResult_t
{
    HASH_TABLE_OUT_OF_MEMORY = -2,
    HASH_TABLE_INVALID_INPUT = -3,
    HASH_TABLE_SUCCESS = 0,
    HASH_TABLE_FAILURE = -1
} hashTableResult;

template <class T>
class hashTable
{
    // Data members
    int size;          // hold current amount of entries inside hash table
    int capacity;      //hold current max capacity of hash table
    float load_factor; //hold current loadF

    float low_load_factor;  //hold table's lower bound load factor
    float high_load_factor; //hold table's higher bound load factor

    twList<T> **hash_array;

public:
    hashTable();
    hashTable(int low_factor, int high_factor);
    ~hashTable();

    //getters
    int getSize() { return this->size; }
    float getLowFactor() { return this->low_load_factor; }
    float getHighFactor() { return this->high_load_factor; }
    float getFactor() { return this->load_factor; }
    twList<T> **getArray() { return this->hash_array; }
    int getCapacity() { return this->capacity; }

    //setters
    void setSize(int new_size) { this->size = new_size; }
    void setCapacity(int new_capacity) { this->capacity = new_capacity; }
    void setLowFactor(float new_low_factor) { this->low_load_factor = new_low_factor; }
    void setHighFactor(float new_high_factor) { this->high_load_factor = new_high_factor; }
    void setFactor(float new_factor) { this->load_factor = new_factor; }
    void setArray(twList<T> **new_table) { this->hash_array = new_table; }


    hashTableResult add(const T& element, int (*function)(int));
    hashTableResult remove(const T& element, int (*function)(int));
    twListNode<T>* find(const T& element, int (*function)(int));
    
    
    
    void resize(int size_change);
    bool resizeRequired(int size_change);
    void updateLoadFactor();
    void transfer(const T &element, int (*function)(int));

    void hashSetNullptr(int size);
    int hashFunction(int key) { return key % size; };
};
#endif