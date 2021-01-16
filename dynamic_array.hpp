#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "dynamic_array.h"
#include "avl_node.hpp"
#include "avl_tree.hpp"

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
    // dynamicSetNullptr(INITIAL_CAPACITY);
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
        if (dynamic_array[i] && dynamic_array[i]->getValue())
            if (dynamic_array[i]->getValue()->getTime() == 0)
                deleteNode(dynamic_array[i]);
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
DynamicArrayResult DynamicArray<T>::add(T *element)
{
    if (element == NULL)
        return DYNAMIC_ARRAY_INVALID_INPUT;
    if (size >= high_load_factor * capacity)
        if (this->resize() == false)
            return DYNAMIC_ARRAY_OUT_OF_MEMORY;
    dynamic_array[size++] = element;
    return DYNAMIC_ARRAY_SUCCESS;
}

/**
 * update element in the array if it is already in it.
 * @tparam T
 * @param element element to add to the array
 * @param function dynamic function to perform
 * @returns DynamicArrayResult
*/
template <class T>
DynamicArrayResult DynamicArray<T>::update(T *element, const int key)
{
    if (element == NULL)
        return DYNAMIC_ARRAY_INVALID_INPUT;
    if (key > size || key < 0)
        return DYNAMIC_ARRAY_INVALID_INPUT;
    dynamic_array[key] = element;
    return DYNAMIC_ARRAY_SUCCESS;
}

/**
 * update element in the array if it is already in it.
 * @tparam T
 * @param element element to add to the array
 * @param function dynamic function to perform
 * @returns DynamicArrayResult
*/
template <class T>
DynamicArrayResult DynamicArray<T>::updateNull(const int key)
{
    if (key > size || key < 0)
        return DYNAMIC_ARRAY_INVALID_INPUT;
    dynamic_array[key] = NULL;
    return DYNAMIC_ARRAY_SUCCESS;
}

/**
 * @brief Find an element in the array
 * @param key key to search in the array
 * @return T* 
 */
template <class T>
T *DynamicArray<T>::find(const int key)
{
    if (key + 1 > size)
    {
        return NULL;
    }
    // std::cout << "The Key is: " << key << std::endl;
    return dynamic_array[key];
}

/**
 * @brief wrapper function for resizing the array.
 * 
 * 
 */
template <class T>
bool DynamicArray<T>::resize()
{
    int old_capacity = this->getCapacity();
    int old_size = this->getSize();
    this->setCapacity(old_capacity * DYNAMIC_ARRAY_CHANGE);
    T **new_array = new T *[this->getCapacity()];
    if (!new_array)
        return false;
    T **old_array = this->getArray();
    for (int i = 0; i < old_size; i++)
    {
        new_array[i] = old_array[i];
    }
    delete[] old_array;
    this->setArray(new_array);
    return true;
}

// /**
//  * @brief Decide whether an array needs to be resized, according to load factor constraints.
//  *
//  * @tparam T
//  * @param size_change
//  * @return true
//  * @return false
//  */
// template <class T>
// bool DynamicArray<T>::resizeRequired(int size_change)
// {
//     if (size_change == INCREASE_SIZE)
//         return ((float)(getSize() / getCapacity()) > getHighFactor());
//     return false;
// }

// /**
//  * @brief Increases the array's size
//  *
//  * @tparam T
//  * @param array DynamicArray to perform resizing on
//  */
// template <class T>
// static void increaseSize(DynamicArray<T> *array)
// {
//     int old_capacity = array->getCapacity();
//     int old_size = array->getSize();
//     array->setCapacity(old_capacity * DYNAMIC_ARRAY_CHANGE);
//     T **new_array = new T *[array->getCapacity()];
//     if (!new_array)
//     {
//         return DYNAMIC_ARRAY_OUT_OF_MEMORY;
//     }
//     T **old_array = array->getArray();
//     for (int i = 0; i < old_size; i++)
//     {
//         new_array[i] = old_array[i];
//     }
//     delete[] old_array;
//     array->setArray(new_array);
// }

// /**
//  * @brief add element to array without resizing (to avoid infinite loop)
//  *
//  * @tparam T
//  * @param element element to add to the array
//  * @param function hash function to perform
//  */
// template <class T>
// void DynamicArray<T>::transfer(const T &element, int (*function)(T, int))
// {
//     int hashedIndex = function(element, this->getCapacity());
//     if (*(hash_array + hashedIndex) == nullptr)
//         *(hash_array + hashedIndex) = new T();
//     (*(hash_array + hashedIndex))->addFirst(element);
// }

#endif