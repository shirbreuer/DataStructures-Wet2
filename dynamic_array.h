#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "two_way_list.hpp"
#include "two_way_list_node.hpp"

#define INITIAL_CAPACITY 16
#define DEFAULT_UPPER_FACTOR 0.75f
#define INCREASE_SIZE 1
#define MIN_DYNAMIC_ARRAY_SIZE 16
#define INITIAL_SIZE 0
#define INITIAL_FACTOR 0
#define DYNAMIC_ARRAY_CHANGE 2

typedef enum DynamicArrayResult_t
{
    DYNAMIC_ARRAY_OUT_OF_MEMORY = -2,
    DYNAMIC_ARRAY_INVALID_INPUT = -3,
    DYNAMIC_ARRAY_SUCCESS = 0,
    DYNAMIC_ARRAY_FAILURE = -1
} DynamicArrayResult;

template <class T>
class DynamicArray
{
    // Data members
    int size;          // hold current amount of entries inside Dynamic Array
    int capacity;      //hold current max capacity of Dynamic Array
    float load_factor; //hold current loadF
    float high_load_factor; //hold table's higher bound load factor
    T** dynamic_array;

public:
    DynamicArray();
    ~DynamicArray();

    //getters
    int getSize() { return this->size; }
    float getHighFactor() { return this->high_load_factor; }
    float getFactor() { return this->load_factor; }
    T** getArray() { return this->dynamic_array; }
    int getCapacity() { return this->capacity; }

    //setters
    void setSize(int new_size) { this->size = new_size; }
    void setCapacity(int new_capacity) { this->capacity = new_capacity; }
    void setLowFactor(float new_low_factor) { this->low_load_factor = new_low_factor; }
    void setHighFactor(float new_high_factor) { this->high_load_factor = new_high_factor; }
    void setFactor(float new_factor) { this->load_factor = new_factor; }
    void setArray(T** new_table) { this->dynamic_array = new_table; }
    DynamicArrayResult add(const T &element, int (*function)(T, int));
    DynamicArrayResult remove(const T &element, int (*function)(T, int));
    T* find(const int key);
    void resize(int size_change);
    bool resizeRequired(int size_change);
    void updateLoadFactor();
    void transfer(const T &element, int (*function)(T, int));
    void dynamicSetNullptr(int size);
};

template<class T>
int dynamicFunction(const T &element, int size)
{
    return element.getKey() % size;
}


int dynamicFunction(int key, int size)
{
    return key % size;
}

#endif