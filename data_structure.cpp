#include "data_structure.h"

courseManager::courseManager() : courses(new hashTable<courseNode>()), classes(new avlTree<classNode>())
{
}

courseManager::~courseManager()
{
    delete courses;
    delete classes;
}

StatusType courseManager::AddCourse(int courseID)
{
    if (courseID <= 0)
        return INVALID_INPUT;

    courseNode *new_course = new courseNode(courseID);
    if (!new_course)
        return (StatusType)HASH_TABLE_OUT_OF_MEMORY;

    StatusType add_result = (StatusType)getCourses()->add(new_course);
    if (add_result != SUCCESS)
        delete new_course;
    return add_result;
}

void printCourseNode2(avlNode<classNode> *node)
{
    std::cout << "=========================" << std::endl;
    std::cout << "left child of " << node->getValue()->getKey() << " is: " << (node->getLeft() ? node->getLeft()->getValue()->getKey() : -1) << std::endl;
    std::cout << "Right child of " << node->getValue()->getKey() << " is: " << (node->getRight() ? node->getRight()->getValue()->getKey() : -1) << std::endl;
    std::cout << "parent of " << node->getValue()->getKey() << " is: " << (node->getParent() ? node->getParent()->getValue()->getKey() : -1) << std::endl;
    std::cout << "time of " << node->getValue()->getKey() << " is: " << node->getValue()->getTime() << std::endl;
    if (node->getParent())
    {
        if (node->isLeftChild())
            std::cout << node->getValue()->getKey() << " is left child" << std::endl;
        else
            std::cout << node->getValue()->getKey() << " is right child" << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

StatusType courseManager::RemoveCourse(int courseID)
{
    if (courseID <= 0)
        return INVALID_INPUT;

    twListNode<courseNode *> *course_pointer = this->getCourses()->find(courseID);
    if (!course_pointer)
        return FAILURE;

    int number_of_classes = course_pointer->getValue()->getNumOfClasses();
    for (int i = 0; i < number_of_classes; i++)
    {
        avlNode<classNode> *class_to_remove = course_pointer->getValue()->getClass(i);
        if (class_to_remove)
        {
            if (class_to_remove->getValue()->getTime() == 0) //class is not inside classes tree.
            {
                deleteNode(class_to_remove);
            }
            else
                this->getClasses()->remove(class_to_remove->getValue()); //class is inside classes tree.
        }
    }

    hashTableResult remove_result = this->getCourses()->remove(course_pointer->getValue());
    if (remove_result == HASH_TABLE_SUCCESS)
        this->classes_counter -= number_of_classes;

    return (StatusType)remove_result;
}

StatusType courseManager::AddClass(int courseID, int *classID)
{
    if (courseID <= 0)
        return INVALID_INPUT;

    twListNode<courseNode *> *course_pointer = this->getCourses()->find(courseID);
    if (!course_pointer)
        return FAILURE;
    int curr_class_count = course_pointer->getValue()->getNumOfClasses();
    classNode *new_class_pointer = new classNode(courseID, curr_class_count);
    if (!new_class_pointer)
        return (StatusType)HASH_TABLE_OUT_OF_MEMORY;
    avlNode<classNode> *new_avl_node = new avlNode<classNode>(new_class_pointer);
    if (!new_avl_node)
    {
        delete new_class_pointer;
        return (StatusType)HASH_TABLE_OUT_OF_MEMORY;
    }

    StatusType return_result = (StatusType)course_pointer->getValue()->setClassPointer(curr_class_count, new_avl_node);
    if (return_result == SUCCESS)
    {
        course_pointer->getValue()->setNumOfClasses(curr_class_count + 1);
        *classID = curr_class_count;
    }
    return return_result;
}

StatusType courseManager::TimeViewed(int courseID, int classID, int *timeViewed)
{
    if (courseID <= 0 || classID < 0)
        return INVALID_INPUT;

    twListNode<courseNode *> *course_pointer = this->getCourses()->find(courseID);
    if (!course_pointer)
        return FAILURE;

    int classes_num = course_pointer->getValue()->getNumOfClasses();
    if (classID + 1 > classes_num)
        return INVALID_INPUT;

    avlNode<classNode> *class_pointer = course_pointer->getValue()->getClass(classID);
    *timeViewed = class_pointer->getValue()->getTime();

    return SUCCESS;
}

StatusType courseManager::WatchClass(int courseID, int classID, int time)
{
    if (courseID <= 0 || classID < 0 || time <= 0)
    {
        // std::cout << "11111############################" << std::endl;
        return INVALID_INPUT;
    }

    twListNode<courseNode *> *wanted_course = this->getCourses()->find(courseID);
    if (!wanted_course)
    {
        return FAILURE;
    }
    int classes_num = wanted_course->getValue()->getNumOfClasses();
    if (classID + 1 > classes_num)
    {
        // std::cout << "22222222############################" << std::endl;
        return INVALID_INPUT;
    }
    avlNode<classNode> *ptr = wanted_course->getValue()->getClass(classID);
    if (!ptr)
        exit(3);
    int curr_time = ptr->getValue()->getTime();
    // if (curr_time == 0)
    // {
    //     ptr->getValue()->setTime(time);
    //     std::cout << "############################" << std::endl;
    //     std::cout << ptr->getValue()->getTime() << std::endl;
    //     return (StatusType)this->getClasses()->insertAvlNode(this->getClasses()->getRoot(), ptr);
    // }
    // else
    // {

        
    classNode *new_class = new classNode(*(ptr->getValue()));
    if (!new_class)
        return (StatusType)AVL_TREE_OUT_OF_MEMORY;
    new_class->setTime(curr_time + time);
    if (curr_time > 0)
    {
        avlTreeResult_t remove_old_class_result = this->getClasses()->removeWOFreeing((ptr->getValue()));
        if (remove_old_class_result != AVL_TREE_SUCCESS)
        {
            // std::cout << "333333333############################" << std::endl;
            return (StatusType)remove_old_class_result;
        }
    }
    deleteNode(ptr);
    avlTreeResult_t insert_class_result = this->getClasses()->insert(new_class);
    if (insert_class_result != AVL_TREE_SUCCESS)
    {
        // std::cout << "4444444444############################" << std::endl;
        return (StatusType)insert_class_result;
    }
    else
    {
        avlNode<classNode> *class_pointer = find(this->getClasses()->getRoot(), new_class);
        // std::cout << "555555################################" << std::endl;
        // printCourseNode2(class_pointer);
        // return (StatusType)wanted_course->getValue().setClassPointer(classID, class_pointer);
        StatusType res = (StatusType)wanted_course->getValue()->setClassPointer(classID, class_pointer);
        // std::cout << "66666555555################################" << std::endl;
        // avlNode<classNode> *res_ptr = wanted_course->getValue().getClass(classID);
        // printCourseNode2(res_ptr);
        return res;

        // }
    }
}

// StatusType courseManager::replaceClass(avlNode<classNode> *ptr, int courseID, int classID, int time, avlNode<courseNode> *course)
// {
//     classNode *temp;
//     if (ptr->getValue()->getTime() == 0)
//     {
//         StatusType insert_result = (StatusType)this->getClasses()->insertAvlNode(this->getClasses()->getRoot(), ptr);
//         if (insert_result != SUCCESS)
//             return insert_result;
//     }
//     else if (ptr->getValue()->getTime() > 0)
//     {
//         classNode *new_class = new classNode(ptr->getValue()->getCourseId(), ptr->getValue()->getKey(), ptr->getValue()->getParentPointer(), ptr->getValue()->getTime() + time);
//         if (!new_class)
//             return (StatusType)AVL_TREE_OUT_OF_MEMORY;
//         avlTreeResult_t remove_old_class_result = this->getClasses()->remove((ptr->getValue()));
//         if (remove_old_class_result != AVL_TREE_SUCCESS)
//             return (StatusType)remove_old_class_result;
//         temp = new_class;
//     }
//     avlTreeResult_t insert_class_result = this->getClasses()->insert(temp);
//     if (insert_class_result != AVL_TREE_SUCCESS)
//     {
//         return (StatusType)insert_class_result;
//     }
//     else
//     {
//         avlNode<classNode> *class_pointer = find(this->getClasses()->getRoot(), temp);
//         return (StatusType)course->getValue()->setClassPointer(classID, class_pointer);
//     }
// }

// void copyNodeToArrays(avlNode<classNode> *node, int *courses, int *classes, int index)
// {
//     courses[index] = node->getValue()->getCourseId();
//     classes[index] = node->getValue()->getKey();
// }

// void copyEmptyClassesToArray(avlNode<twList<int>> *node, int *courses, int *classes, int *index_address, int classes_with_zero_views)
// {
//     twListNode<int> *head = node->getValue()->getHead();
//     twListNode<int> *tail = node->getValue()->getTail();
//     while (head->getNext() != tail && (classes_with_zero_views - *index_address))
//     {
//         head = head->getNext();
//         courses[*index_address] = node->getValue()->getKey();
//         classes[*index_address] = head->getValue();
//         (*index_address)++;
//     }
// }

// StatusType courseManager::GetMostViewedClasses(int numOfClasses, int *courses, int *classes)
// {
//     // std::cout << this->classes_counter << std::endl;
//     // int * temp_courses = courses;
//     if (numOfClasses <= 0)
//         return INVALID_INPUT;

//     if (this->classes_counter < numOfClasses)
//         return FAILURE;

//     int classes_with_zero_views = this->getClasses()->reverseInOrder(numOfClasses, copyNodeToArrays, courses, classes);

//     if (classes_with_zero_views == 0)
//         return SUCCESS;

//     this->getLists()->nonRecursiveInOrder(classes_with_zero_views, copyEmptyClassesToArray, courses + numOfClasses - classes_with_zero_views, classes + numOfClasses - classes_with_zero_views);
//     return SUCCESS;
// }
