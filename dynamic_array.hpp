#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "dynamic_array.h"

/**
 * @brief Dynamic Array constructor (without parameters)
 *
 * @returns DynamicArray<T>
*/
template <class T>
DynamicArray<T>::DynamicArray() : size(INITIAL_SIZE), capacity(INITIAL_CAPACITY),
                            load_factor(INITIAL_FACTOR), high_load_factor(DEFAULT_UPPER_FACTOR),
                             dynamic_array(new T *[INITIAL_CAPACITY])
{
    dynamicSetNullptr(INITIAL_CAPACITY);
}

/**
 * @brief Dynamic Array constructor (with parameters)
 * Worst case - O(1)
 *
 * @tparam T
 * @param high_factor high boundry factor
 * @returns DynamicArray<T>
*/
template <class T>
DynamicArray<T>::DynamicArray(float high_factor) : size(INITIAL_SIZE), capacity(INITIAL_CAPACITY),
                                                           load_factor(INITIAL_FACTOR), high_load_factor(high_factor),
                                                           dynamic_array(new T *[INITIAL_CAPACITY])
{
    dynamicSetNullptr(INITIAL_CAPACITY);
}

/**
 * @brief Destroy the Dynamic Array<T>::Dynamic Array object
 * 
 * @tparam T 
 */
template <class T>
DynamicArray<T>::~DynamicArray() 
{
    int num_of_elements = getSize();
    for (int i = 0; i < num_of_elements; i++)
    {
        if (*(dynamic_array + i) != nullptr) {
            delete *(dynamic_array + i);
        }
    }
    delete[] dynamic_array;
}

/**
 * @brief set all pointers in array to nullptr @ O(size).
 *
 * @param size size of array
*/
template <class T>
void DynamicArray<T>::dynamicSetNullptr(int size)
{
    for (int i = 0; i < size; i++)
    {
        *(dynamic_array + i) = nullptr;
    }
}


/**
 * Add new element to the array if it is not already in it.
 * @tparam T
 * @param element element to add to the array
 * @param function dynamic function to perform
 * @returns DynamicArrayResult
*/
template <class T>
DynamicArrayResult DynamicArray<T>::add(const T &element, int (*function)(T, int))
{
    if (find(element, function) != nullptr)
        return DYNAMIC_ARRAY_FAILURE;
    this->size++;
    updateLoadFactor();
    resize(INCREASE_SIZE);
    int dynamicIndex = function(element, this->getCapacity());
    if (*(dynamic_array + dynamicIndex) == nullptr)
        *(dynamic_array + dynamicIndex) = new T();
    (*(dynamic_array + dynamicIndex))->addFirst(element);
    return DYNAMIC_ARRAY_SUCCESS;
}

/**
 * @brief search for element in Dynamic Array
 * @tparam T 
 * @param element element to search in the array
 * @param function hash function to perform
 * @return T* 
 */
template <class T>
T *DynamicArray<T>::find(const T &element, int (*function)(T, int))
{
    // std::cout << "trying to find" << std::endl;
    int hashedIndex = function(element, this->getCapacity());
    // std::cout << (*(hash_array + hashedIndex) ? "123" : "0") << std::endl;
    if (*(hash_array + hashedIndex) == nullptr) //no list
        return nullptr;
    return (*(hash_array + hashedIndex))->contains(element);
}

/**
 * @brief Add new element to the array if it is not already in it.
 * @tparam T 
 * @param key key to search in the array
 * @param function hash function to perform
 * @return T* 
 */
template <class T>
T *DynamicArray<T>::find(const int key, int (*function)(int, int))
{
    int hashedIndex = function(key, this->getCapacity());
    if (*(hash_array + hashedIndex) == nullptr) //no list
        return nullptr;
    return (*(hash_array + hashedIndex))->contains(key);
}

/**
 * @brief Updates current load factor.
 * 
 * @tparam T 
 */
template <class T>
void DynamicArray<T>::updateLoadFactor()
{
    float new_load_factor = (float)getSize()/getCapacity();
    setFactor(new_load_factor);
}

/**
 * @brief wrapper function for resizing the array.
 * 
 * 
 * @param size_change (1) if increasing, (-1) if decreasing
 */
template <class T>
void DynamicArray<T>::resize(int size_change)
{
    if (resizeRequired(size_change))
    {
        // T **hash_array_copy = this->getArray();
        switch (size_change)
        {
        case INCREASE_SIZE:
            increaseSize(this);
            break;
        case DECREASE_SIZE:
            decreaseSize(this);
            break;
        }
    }
}

/**
 * @brief Decide whether a array needs to be resized, according to load factor constraints.
 * 
 * @tparam T
 * @param size_change 
 * @return true 
 * @return false 
 */
template <class T>
bool DynamicArray<T>::resizeRequired(int size_change)
{
    if (size_change == INCREASE_SIZE)
        return getFactor() > getHighFactor();
    else if (getCapacity() / DYNAMIC_ARRAY_CHANGE >= MIN_DYNAMIC_ARRAY_SIZE)
        return getFactor() < getLowFactor();
    return false;
}

/**
 * @brief Increases the array's size
 * 
 * @tparam T 
 * @param array DynamicArray to perform resizing on 
 */
template <class T>
static void increaseSize(DynamicArray<T> *array)
{
    int old_capacity = array->getCapacity();
    array->setCapacity(old_capacity * DYNAMIC_ARRAY_CHANGE);
    T **new_array = new T *[array->getCapacity()];
    T **old_array = array->getArray();
    array->setArray(new_array);
    array->hashSetNullptr(array->getCapacity());
    for (int i = 0; i < old_capacity; i++)
    {
        if (*(old_array + i) != nullptr)
        {
            T *iter = (*(old_array + i))->getHead();
            while (iter != (*(old_array + i))->getTail())
            {
                array->transfer(iter->getValue(), hashFunction);
                iter = iter->getNext();
            }
        }
        delete *(old_array + i);
    }
    delete[] old_array;
}

/**
 * @brief Decreases the array's size
 * 
 * @tparam T 
 * @param array DynamicArray to perform resizing on 
 */
template <class T>
static void decreaseSize(DynamicArray<T> *array)
{
    int old_capacity = array->getCapacity();
    array->setCapacity(old_capacity / DYNAMIC_ARRAY_CHANGE);
    T **new_array = new T *[array->getCapacity()];
    T **old_array = array->getArray();
    array->setArray(new_array);
    array->hashSetNullptr(array->getCapacity());
    for (int i = 0; i < old_capacity; i++)
    {
        if (*(old_array + i) != nullptr)
        {
            T *iter = (*(old_array + i))->getHead();
            while (iter != (*(old_array + i))->getTail())
            {
                array->transfer(iter->getValue(), hashFunction);
                iter = iter->getNext();
            }
        }
        delete *(old_array + i);
    }
    delete[] old_array;
}

/**
 * @brief add element to array without resizing (to avoid infinite loop)
 * 
 * @tparam T 
 * @param element element to add to the array
 * @param function hash function to perform
 */
template <class T>
void DynamicArray<T>::transfer(const T &element, int (*function)(T, int))
{
    int hashedIndex = function(element, this->getCapacity());
    if (*(hash_array + hashedIndex) == nullptr)
        *(hash_array + hashedIndex) = new T();
    (*(hash_array + hashedIndex))->addFirst(element);
}

#endif