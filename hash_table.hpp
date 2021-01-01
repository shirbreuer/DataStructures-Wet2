#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "hash_table.h"

/**
 * @brief Hash table constructor (without parameters)
 *
 * @returns hashTable<T>
*/
template <class T>
hashTable<T>::hashTable() : size(INITIAL_SIZE), capacity(INITIAL_CAPACITY),
                            load_factor(INITIAL_FACTOR), low_load_factor(DEFAULT_LOWER_FACTOR),
                            high_load_factor(DEFAULT_UPPER_FACTOR), hash_array(new twList<T> *[INITIAL_SIZE])
{
    hashSetNullptr(INITIAL_SIZE);
}

/**
 * @brief Hash table constructor (with parameters)
 * Worst case - O(1)
 *
 * @tparam T
 * @param low_factor low boundry factor
 * @param high_factor high boundry factor
 * @returns hashTable<T>
*/
template <class T>
hashTable<T>::hashTable(int low_factor, int high_factor) : size(INITIAL_SIZE), capacity(INITIAL_CAPACITY),
                                                           load_factor(INITIAL_FACTOR), low_load_factor(low_factor),
                                                           high_load_factor(high_factor), hash_array(new twList<T> *[INITIAL_SIZE])
{
    hashSetNullptr(INITIAL_SIZE);
}

/**
 * @brief Destroy the hash Table<T>::hash Table object
 * 
 * @tparam T 
 */
template <class T>
hashTable<T>::~hashTable()
{
    for (int i = 0; i < getSize(); i++)
    {
        if (*(hash_array + i) != nullptr)
            delete *(hash_array + i);
    }
}

/**
 * @brief set all pointers in array to nullptr @ O(size).
 *
 * @param size size of array
*/
template <class T>
void hashTable<T>::hashSetNullptr(int size)
{
    for (int i = 0; i < size; i++)
    {
        *(hash_array + i) = nullptr;
    }
}


/**
 * Add new element to the table if it is not already in it.
 * @tparam T
 * @param key key of item to add
 * @param function hash function to perform
 * @returns hashTableResult
*/
template<class T>
hashTableResult hashTable<T>::add(const T& element, int (*function)(int))
{
    if (find(element) != nullptr)
        return HASH_TABLE_FAILURE;
    this->size++;
    resize(INCREASE_SIZE);
    int hashedIndex = function(key);
    if (*(hash_array + hashedIndex) == nullptr)
        *(hash_array + hashedIndex) = new twList<T>();
    *(hash_array + hashedIndex)->addFirst(element);
    return HASH_TABLE_SUCCESS;
}

/**
 * Add new element to the table if it is not already in it.
 * @tparam T
 * @param key key of item to add
 * @param function hash function to perform
 * @returns hashTableResult
*/
template<class T>
hashTableResult hashTable<T>::find(const T& element, int (*function)(int))
{
    if (find(element) != nullptr)
        return HASH_TABLE_FAILURE;
    this->size++;
    resize(INCREASE_SIZE);
    int hashedIndex = function(key);
    if (*(hash_array + hashedIndex) == nullptr)
        *(hash_array + hashedIndex) = new twList<T>();
    *(hash_array + hashedIndex)->addFirst(element);
    return HASH_TABLE_SUCCESS;
}


#endif