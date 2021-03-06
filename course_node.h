#ifndef COURSE_NODE_H
#define COURSE_NODE_H

#include "class_node.h"
#include "two_way_list.hpp"
#include "two_way_list_node.hpp"
#include "hash_table.hpp"
#include "dynamic_array.hpp"

typedef enum CourseStatus_t
{
    COURSE_ALLOCATION_ERROR = -2,
    COURSE_INVALID_INPUT = -3,
    COURSE_SUCCESS = 0,
    COURSE_FAILURE = -1
} CourseStatus;

class courseNode
{
private:
    int course_id;
    int num_of_classes;
    DynamicArray<avlNode<classNode> > *array;

public:
    courseNode();
    courseNode(const courseNode &other) = default;
    courseNode(const int courseID);
    ~courseNode();
    avlNode<classNode> *getClass(const int classID) const;
    bool operator<(const courseNode &courseToCompare) const;
    bool operator>(const courseNode &courseToCompare) const;
    bool operator<=(const courseNode &courseToCompare) const;
    bool operator==(const courseNode &courseToCompare) const;
    bool operator!() const;
    void setId(int new_id);
    int getKey() { return this->course_id; }
    const int getKey() const { return this->course_id; }
    void setNumOfClasses(int numOfClasses);
    int getNumOfClasses() { return this->num_of_classes; }
    const int getNumOfClasses() const { return this->num_of_classes; }
    avlNode<classNode> *getClassPointer(int classID);
    avlNode<classNode> *const getClassPointer(int classID) const;
    DynamicArray<avlNode<classNode>> *getHashTable() { return this->array; }
    DynamicArray<avlNode<classNode>> *const getHashTable() const { return this->array; }
    CourseStatus setClassPointer(int classID, avlNode<classNode> *class_ptr);
    CourseStatus setNullPointer(int classID);
};

#endif