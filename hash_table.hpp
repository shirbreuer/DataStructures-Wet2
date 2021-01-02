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
 * @param element element to add to the table
 * @param function hash function to perform
 * @returns hashTableResult
*/
template <class T>
hashTableResult hashTable<T>::add(const T &element, int (*function)(int))
{
    if (find(element) != nullptr)
        return HASH_TABLE_FAILURE;
    this->size++;
    updateLoadFactor();
    resize(INCREASE_SIZE);
    int hashedIndex = function(element.getKey());
    if (*(hash_array + hashedIndex) == nullptr)
        *(hash_array + hashedIndex) = new twList<T>();
    *(hash_array + hashedIndex)->addFirst(element);
    return HASH_TABLE_SUCCESS;
}

/**
 * @brief Add new element to the table if it is not already in it.
 * @tparam T 
 * @param element element to search in the table
 * @param function hash function to perform
 * @return twListNode<T>* 
 */
template <class T>
twListNode<T> *hashTable<T>::find(const T &element, int (*function)(int))
{
    int hashedIndex = function(element.getKey());
    if (*(hash_array + hashedIndex) == nullptr) //no list
        return nullptr;
    return *(hash_array + hashedIndex)->contains(element);
}

/**
 * @brief delete element from the table if the table contains this element.
 * @tparam T
 * @param element element to remove from the table
 * @param function hash function to perform
 * @returns hashTableResult
*/
template <class T>
hashTableResult hashTable<T>::remove(const T &element, int (*function)(int))
{
    if (find(element) != nullptr)
        return HASH_TABLE_FAILURE;
    this->size--;
    updateLoadFactor();
    int hashedIndex = function(element.getKey());
    *(hash_array + hashedIndex)->remove(element);
    resize(DECREASE_SIZE);
    return HASH_TABLE_SUCCESS;
}

/**
 * @brief Updates current load factor.
 * 
 * @tparam T 
 */
template <class T>
void hashTable<T>::updateLoadFactor()
{
    float new_load_factor = (float)this->getSize() / this->getCapacity();
    this->setFactor(new_load_factor);
}

/**
 * @brief wrapper function for resizing the table
 * 
 * @param size_change (1) if increasing, (-1) if decreasing
 */
template <class T>
void hashTable<T>::resize(int size_change)
{
    if (resizeRequired(size_change))
    {
        twList<T> **hash_array_copy = this->getArray();
        switch (size_change)
        {
        case INCREASE_SIZE:
            increaseSize(hash_array_copy);
            break;
        case DECREASE_SIZE:
            decreaseSize(hash_array_copy);
            break;
        }
    }
}

/**
 * @brief Decide whether a table needs to be resized, according to load factor constraints.
 * 
 * @tparam T
 * @param size_change 
 * @return true 
 * @return false 
 */
template <class T>
bool hashTable<T>::resizeRequired(int size_change)
{
    if (size_change == INCREASE_SIZE)
        return this->getFactor() > this->getHighFactor();
    else if (this->getCapacity() / HASH_TABLE_CHANGE >= MIN_HASH_TABLE_SIZE)
        return this->getFactor() < this->getLowFactor();
    return false;
}

/**
 * @brief Increases the table's size
 * 
 * @tparam T 
 * @param table Hashtable to perform resizing on 
 */
template <class T>
static void increaseSize(hashTable<T> *table)
{
    int old_capacity = table->getCapacity();
    table->setCapacity(old_capacity * HASH_TABLE_CHANGE);
    twList<T> **new_array = new twList<T> *[table->getCapacity()];
    twList<T> **old_array = table->getArray();
    table->setArray(new_array);
    table->hashSetNullptr(table->getCapacity());
    for (int i = 0; i < old_capacity; i++)
    {
        if (*(old_array + i) != nullptr)
        {
            twListNode<T> *iter = *(old_array + i)->getHead();
            while (iter != *(old_array + i)->getTail())
            {
                table->transfer(iter->getValue(), hashFunction);
                iter = iter->getNext();
            }
        }
        delete *(old_array + i);
    }
    delete[] old_array;
}

/**
 * @brief Decreases the table's size
 * 
 * @tparam T 
 * @param table Hashtable to perform resizing on 
 */
template <class T>
static void decreaseSize(hashTable<T> *table)
{
    int old_capacity = table->getCapacity();
    table->setCapacity(old_capacity / HASH_TABLE_CHANGE);
    twList<T> **new_array = new twList<T> *[table->getCapacity()];
    twList<T> **old_array = table->getArray();
    table->setArray(new_array);
    table->hashSetNullptr(table->getCapacity());
    for (int i = 0; i < old_capacity; i++)
    {
        if (*(old_array + i) != nullptr)
        {
            twListNode<T> *iter = *(old_array + i)->getHead();
            while (iter != *(old_array + i)->getTail())
            {
                table->transfer(iter->getValue(), hashFunction);
                iter = iter->getNext();
            }
        }
        delete *(old_array + i);
    }
    delete[] old_array;
}

/**
 * @brief add element to table without resizing (to avoid infinite loop)
 * 
 * @tparam T 
 * @param element element to add to the table
 * @param function hash function to perform
 */
template <class T>
void hashTable<T>::transfer(const T &element, int (*function)(int))
{
    int hashedIndex = function(element.getKey());
    if (*(hash_array + hashedIndex) == nullptr)
        *(hash_array + hashedIndex) = new twList<T>();
    *(hash_array + hashedIndex)->addFirst(element);
}

#endif