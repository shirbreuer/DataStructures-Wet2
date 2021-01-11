#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "avl_tree.hpp"
#include "avl_node.hpp"
#include "class_node.h"
#include "course_node.h"
#include "library.h"
#include "two_way_list.hpp"
#include "two_way_list_node.hpp"
#include "hash_table.hpp"


class classNode;
class courseNode;

class courseManager
{
private:
    hashTable<courseNode>* courses;
    avlTree<classNode>* classes;
    // avlTree<twList<int>>* lists;
    int classes_counter = 0;

public:
    courseManager();
    ~courseManager();
    StatusType AddCourse(int courseID);
    StatusType RemoveCourse(int courseID);
    StatusType WatchClass(int courseID, int classID, int time);
    StatusType TimeViewed(int courseID, int classID, int *timeViewed);
    StatusType GetMostViewedClasses(int numOfClasses, int *courses, int *classes);
    hashTable<courseNode>* getCourses() { return this->courses; }
    avlTree<classNode>* getClasses() { return this->classes; }
    // avlTree<twList<int>>* getLists() { return this->lists; }
    void Quit(void **DS);
    StatusType replaceClass(avlNode<classNode> *ptr, int courseID, int classID, int time, avlNode<courseNode> *course);
};

#endif