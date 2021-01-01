#ifndef COURSE_NODE_H
#define COURSE_NODE_H

#include "class_node.h"
#include "two_way_list.hpp"
#include "two_way_list_node.hpp"

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
    avlNode<classNode>** classes_pointers_array;
    twListNode<int>** zero_views_node_pointers;
    twList<int>* zero_views_classes;

public:
    courseNode();
    courseNode(const courseNode& other);
    courseNode(const int courseID, const int numOfClasses);
    ~courseNode();
    avlNode<classNode>* getClass(const int classID);
    bool operator<(const courseNode& courseToCompare) const;
    bool operator>(const courseNode& courseToCompare) const;
    bool operator<=(const courseNode& courseToCompare) const;
    bool operator==(const courseNode& courseToCompare) const;
    bool operator!() const;
    void setId(int new_id);
    int getId() {return this->course_id;}
    const int getId() const {return this->course_id;}
    void setNumOfClasses(int numOfClasses);
    int getNumOfClasses() {return this->num_of_classes;}
    const int getNumOfClasses() const {return this->num_of_classes;}
    avlNode<classNode>* getClassPointer(int classID) { return *(this->classes_pointers_array+classID);}
    avlNode<classNode>* const getClassPointer(int classID) const { return *(this->classes_pointers_array+classID);}
    avlNode<classNode>** getPointersArray() { return this->classes_pointers_array;}
    avlNode<classNode>** const getPointersArray() const { return this->classes_pointers_array;}
    twListNode<int>* getClassNodePointer(int classID) { return *(this->zero_views_node_pointers+classID);}
    twListNode<int>*  const getClassNodePointer(int classID) const { return *(this->zero_views_node_pointers+classID);}
    twList<int>* getList() { return this->zero_views_classes; }
    CourseStatus setClassPointer(int classID, avlNode<classNode>* class_ptr);

};


#endif