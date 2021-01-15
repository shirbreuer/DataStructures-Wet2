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
                            high_load_factor(DEFAULT_UPPER_FACTOR),
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
 * Add new element to the array if it is not already in it.
 * @tparam T
 * @param element element to add to the array
 * @param function dynamic function to perform
 * @returns DynamicArrayResult
*/
template <class T>
DynamicArrayResult DynamicArray<T>::add(const T &element)
{
    if(element==NULL)
        return DYNAMIC_ARRAY_INVALID_INPUT;
    if(size>=high_load_factor*capacity)
        increaseSize(dynamic_array);
    dynamic_array[size++]=element;
    return DYNAMIC_ARRAY_SUCCESS;
}


/**
 * @brief Find an element in the array
 * @param key key to search in the array
 * @return T* 
 */
template <class T>
T* DynamicArray<T>::find(const int key)
{
    if (key<size){
        return NULL;
    }
    return *dynamic_array[index];
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
 * @brief Decide whether an array needs to be resized, according to load factor constraints.
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
    int old_size = array->getSize();
    array->setCapacity(old_capacity * DYNAMIC_ARRAY_CHANGE);
    T **new_array = new T *[array->getCapacity()];
    if(!new_array){
        return DYNAMIC_ARRAY_OUT_OF_MEMORY;
    }
    T **old_array = array->getArray();
    for (int i = 0; i < old_size; i++)
    {
        new_array[i]=old_array[i];
    }
    delete[] old_array;
    array->setArray(new_array);
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