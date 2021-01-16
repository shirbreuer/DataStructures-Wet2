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
                            high_load_factor(DEFAULT_UPPER_FACTOR), hash_array(new twList<T*> *[INITIAL_CAPACITY])
{
    // std::cout << "non parametric constructor" << std::endl;
    hashSetNullptr(INITIAL_CAPACITY);
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
hashTable<T>::hashTable(float low_factor, float high_factor) : size(INITIAL_SIZE), capacity(INITIAL_CAPACITY),
                                                           load_factor(INITIAL_FACTOR), low_load_factor(low_factor),
                                                           high_load_factor(high_factor), hash_array(new twList<T*> *[INITIAL_CAPACITY])
{
    // std::cout << "parametric constructor: " << INITIAL_SIZE << std::endl;
    hashSetNullptr(INITIAL_CAPACITY);
    // std::cout << "12322";
}

/**
 * @brief Destroy the hash Table<T>::hash Table object
 * 
 * @tparam T 
 */
template <class T>
hashTable<T>::~hashTable()
{
    for (int i = 0; i < getCapacity(); i++)
    {
        if (*(hash_array + i) != nullptr) {
            hash_array[i]->emptyList();
            delete hash_array[i];
        }
    }
    delete[] hash_array;
}

/**
 * @brief set all pointers in array to nullptr @ O(size).
 *
 * @param size size of array
*/
template <class T>
void hashTable<T>::hashSetNullptr(int size)
{
    // std::cout << "initial size is: " << size << std::endl;
    for (int i = 0; i < size; i++)
    {
        *(hash_array + i) = nullptr;
        // std::cout << i << std::endl;
    }
}


/**
 * Add new element to the table if it is not already in it.
 * @tparam T
 * @param element element to add to the table
 * @returns hashTableResult
*/
template <class T>
hashTableResult hashTable<T>::add(T* element)
{
    if (!element)
        return HASH_TABLE_INVALID_INPUT;
    // std::cout <<"trying to add" << std::endl;
    if (find(element->getKey()) != nullptr)
        return HASH_TABLE_FAILURE;
    // std::cout <<"not found" << std::endl;

    this->size++;
    updateLoadFactor();
    resize(INCREASE_SIZE);
    int hashedIndex = element->getKey()%this->getCapacity();
    if (hash_array[hashedIndex] == nullptr)
        hash_array[hashedIndex] = new twList<T*>();
    (hash_array[hashedIndex])->addFirst(element);
    // std::cout << *(*(hash_array + hashedIndex)) << std::endl;
    updateLoadFactor();
    return HASH_TABLE_SUCCESS;
}

/**
 * @brief search for element in hash table
 * @tparam T 
 * @param element element to search in the table
 * @return twListNode<T>* 
 */
template <class T>
twListNode<T*> *hashTable<T>::find(T* element)
{
    // std::cout << "trying to find" << std::endl;
    int hashedIndex = element->getKey()%this->getCapacity();
    // std::cout << (*(hash_array + hashedIndex) ? "123" : "0") << std::endl;
    if (*(hash_array + hashedIndex) == nullptr) //no list
        return nullptr;
    return (*(hash_array + hashedIndex))->contains(element);
}

/**
 * @brief Add new element to the table if it is not already in it.
 * @tparam T 
 * @param key key to search in the table
 * @return twListNode<T>* 
 */
template <class T>
twListNode<T*> *hashTable<T>::find(const int key)
{
    // std::cout << "trying to find" << std::endl;
    int hashedIndex = key%this->getCapacity();
    // std::cout << (*(hash_array + hashedIndex) ? "123" : "0") << std::endl;
    if (*(hash_array + hashedIndex) == nullptr) //no list
        return nullptr;
    return (hash_array[hashedIndex])->contains(key);
}




/**
 * @brief delete element from the table if the table contains this element.
 * @tparam T
 * @param element element to remove from the table
 * @returns hashTableResult
*/
template <class T>
hashTableResult hashTable<T>::remove(T* element)
{
    twListNode<T*>* element_node_to_remove = find(element);
    if (element_node_to_remove == nullptr)
        return HASH_TABLE_FAILURE;
    this->size--;
    updateLoadFactor();
    int hashedIndex = element->getKey()%this->getCapacity();
    if (element_node_to_remove->getValue())
        delete element_node_to_remove->getValue();
    hash_array[hashedIndex]->remove(element_node_to_remove);
    resize(DECREASE_SIZE);
    updateLoadFactor();
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
    float new_load_factor = (float)getSize()/getCapacity();
    setFactor(new_load_factor);
}

/**
 * @brief wrapper function for resizing the table.
 * 
 * 
 * @param size_change (1) if increasing, (-1) if decreasing
 */
template <class T>
void hashTable<T>::resize(int size_change)
{
    if (resizeRequired(size_change))
    {
        // twList<T> **hash_array_copy = this->getArray();
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
        return getFactor() > getHighFactor();
    else if (getCapacity() / HASH_TABLE_CHANGE >= MIN_HASH_TABLE_SIZE)
        return getFactor() < getLowFactor();
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
    twList<T*> **new_array = new twList<T*> *[table->getCapacity()];
    twList<T*> **old_array = table->getArray();
    table->setArray(new_array);
    table->hashSetNullptr(table->getCapacity());
    for (int i = 0; i < old_capacity; i++)
    {
        if (*(old_array + i) != nullptr)
        {
            twListNode<T*> *iter = (*(old_array + i))->getHead();
            while (iter != (*(old_array + i))->getTail())
            {
                table->transfer(iter->getValue());
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
    twList<T*> **new_array = new twList<T*> *[table->getCapacity()];
    twList<T*> **old_array = table->getArray();
    table->setArray(new_array);
    table->hashSetNullptr(table->getCapacity());
    for (int i = 0; i < old_capacity; i++)
    {
        if (*(old_array + i) != nullptr)
        {
            twListNode<T*> *iter = (*(old_array + i))->getHead();
            while (iter != (*(old_array + i))->getTail())
            {
                table->transfer(iter->getValue());
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
 */
template <class T>
void hashTable<T>::transfer(T* element)
{
    if (!element || !element->getKey())
        return;
    int hashedIndex = element->getKey()%this->getCapacity();
    if (hash_array[hashedIndex] == nullptr)
        hash_array[hashedIndex] = new twList<T*>();
    hash_array[hashedIndex]->addFirst(element);
}



#endif