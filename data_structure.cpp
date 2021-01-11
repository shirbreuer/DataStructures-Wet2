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
    hashTableResult insert_result = getCourses()->add(*new_course, hashFunctionDS);
    if (insert_result != HASH_TABLE_SUCCESS)
    {
        return (StatusType)insert_result;
    }
    avlNode<courseNode> *course_pointer = find(this->getCourses()->getRoot(), new_course);
    this->getLists()->insert(course_pointer->getValue()->getList());
    this->classes_counter += numOfClasses;
    return (StatusType)insert_result;
}

void printCourseNode2(avlNode<courseNode> *node)
{
    std::cout << "=========================" << std::endl;
    std::cout << "left child of: " << node->getValue()->getId() << " is: " << (node->getLeft() ? node->getLeft()->getValue()->getId() : -1) << std::endl;
    std::cout << "Right child of: " << node->getValue()->getId() << " is: " << (node->getRight() ? node->getRight()->getValue()->getId() : -1) << std::endl;
    std::cout << "parent of: " << node->getValue()->getId() << " is: " << (node->getParent() ? node->getParent()->getValue()->getId() : -1) << std::endl;
    if (node->getParent())
    {
        if (node->isLeftChild())
            std::cout << node->getValue()->getId() << " is left child" << std::endl;
        else
            std::cout << node->getValue()->getId() << " is right child" << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

void printCourseNode2(avlNode<classNode> *node)
{
    std::cout << "=========================" << std::endl;
    std::cout << "left child of: " << node->getValue()->getClassId() << " is: " << (node->getLeft() ? node->getLeft()->getValue()->getClassId() : -1) << std::endl;
    std::cout << "Right child of: " << node->getValue()->getClassId() << " is: " << (node->getRight() ? node->getRight()->getValue()->getClassId() : -1) << std::endl;
    std::cout << "parent of: " << node->getValue()->getClassId() << " is: " << (node->getParent() ? node->getParent()->getValue()->getClassId() : -1) << std::endl;
    if (node->getParent())
    {
        if (node->isLeftChild())
            std::cout << node->getValue()->getClassId() << " is left child" << std::endl;
        else
            std::cout << node->getValue()->getClassId() << " is right child" << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

StatusType courseManager::RemoveCourse(int courseID)
{
    if (courseID <= 0)
    {
        return INVALID_INPUT;
    }
    courseNode searching_course_template;
    searching_course_template.setId(courseID);
    avlNode<courseNode> *course_pointer = find(this->getCourses()->getRoot(), searching_course_template);
    if (!course_pointer)
    {
        return FAILURE;
    }
    int number_of_classes = course_pointer->getValue()->getNumOfClasses();
    for (int i = 0; i < number_of_classes; i++)
    {
        avlNode<classNode> *class_to_remove = *(course_pointer->getValue()->getPointersArray() + i);
        if (class_to_remove)
        {
            this->getClasses()->remove(class_to_remove->getValue());
        }
    }
    this->getLists()->removeWOFreeing(course_pointer->getValue()->getList());
    avlTreeResult_t remove_result = this->getCourses()->remove((course_pointer->getValue()));
    if (remove_result == AVL_TREE_SUCCESS)
    {
        this->classes_counter -= number_of_classes;
    }

    return (StatusType)remove_result;
}

StatusType courseManager::TimeViewed(int courseID, int classID, int *timeViewed)
{
    if (courseID <= 0 || classID < 0)
        return INVALID_INPUT;

    courseNode new_course;
    new_course.setId(courseID);
    avlNode<courseNode> *wanted_course = find(this->getCourses()->getRoot(), &new_course);
    if (!wanted_course)
        return FAILURE;

    int num_of_classes = wanted_course->getValue()->getNumOfClasses();
    if (classID + 1 > num_of_classes)
        return INVALID_INPUT;

    avlNode<classNode> *ptr = wanted_course->getValue()->getClassPointer(classID);
    if (!ptr)
        *timeViewed = 0;
    else
        *timeViewed = ptr->getValue()->getTime();

    return SUCCESS;
}

StatusType courseManager::WatchClass(int courseID, int classID, int time)
{
    if (courseID <= 0 || classID < 0 || time <= 0)
    {
        return INVALID_INPUT;
    }
    courseNode temp_course;
    temp_course.setId(courseID);
    avlNode<courseNode> *wanted_course = find(this->getCourses()->getRoot(), temp_course);
    if (!wanted_course)
    {
        return FAILURE;
    }
    int num_of_classes = wanted_course->getValue()->getNumOfClasses();
    if (classID + 1 > num_of_classes)
    {
        return INVALID_INPUT;
    }
    avlNode<classNode> *ptr = wanted_course->getValue()->getClassPointer(classID);
    return this->replaceClass(ptr, courseID, classID, time, wanted_course);
}

StatusType courseManager::replaceClass(avlNode<classNode> *ptr, int courseID, int classID, int time, avlNode<courseNode> *course)
{
    classNode *temp;
    if (!ptr)
    {
        classNode *new_class = new classNode(courseID, classID, (void *)course, time);
        if (!new_class)
            return (StatusType)AVL_TREE_OUT_OF_MEMORY;
        temp = new_class;
        twListNode<int> *node_to_remove = course->getValue()->getClassNodePointer(classID);
        if (course->getValue()->getList()->remove(node_to_remove) != TW_LIST_SUCCESS)
            return (StatusType)TW_LIST_FAILURE;
        if (course->getValue()->getList()->size() == 0)
            this->getLists()->removeWOFreeing(course->getValue()->getList());
    }
    else
    {
        classNode *new_class = new classNode(ptr->getValue()->getCourseId(), ptr->getValue()->getClassId(), /*(avlNode<courseNode> *)*/ ptr->getValue()->getParentPointer(), ptr->getValue()->getTime() + time);
        if (!new_class)
            return (StatusType)AVL_TREE_OUT_OF_MEMORY;
        avlTreeResult_t remove_old_class_result = this->getClasses()->remove((ptr->getValue()));
        if (remove_old_class_result != AVL_TREE_SUCCESS)
            return (StatusType)remove_old_class_result;

        temp = new_class;
    }
    avlTreeResult_t insert_class_result = this->getClasses()->insert(temp);
    if (insert_class_result != AVL_TREE_SUCCESS)
    {
        return (StatusType)insert_class_result;
    }
    else
    {
        avlNode<classNode> *class_pointer = find(this->getClasses()->getRoot(), temp);
        return (StatusType)course->getValue()->setClassPointer(classID, class_pointer);
    }
}

void copyNodeToArrays(avlNode<classNode> *node, int *courses, int *classes, int index)
{
    courses[index] = node->getValue()->getCourseId();
    classes[index] = node->getValue()->getClassId();
}

void copyEmptyClassesToArray(avlNode<twList<int>> *node, int *courses, int *classes, int *index_address, int classes_with_zero_views)
{
    twListNode<int> *head = node->getValue()->getHead();
    twListNode<int> *tail = node->getValue()->getTail();
    while (head->getNext() != tail && (classes_with_zero_views - *index_address))
    {
        head = head->getNext();
        courses[*index_address] = node->getValue()->getKey();
        classes[*index_address] = head->getValue();
        (*index_address)++;
    }
}

StatusType courseManager::GetMostViewedClasses(int numOfClasses, int *courses, int *classes)
{
    // std::cout << this->classes_counter << std::endl;
    // int * temp_courses = courses;
    if (numOfClasses <= 0)
        return INVALID_INPUT;

    if (this->classes_counter < numOfClasses)
        return FAILURE;

    int classes_with_zero_views = this->getClasses()->reverseInOrder(numOfClasses, copyNodeToArrays, courses, classes);

    if (classes_with_zero_views == 0)
        return SUCCESS;

    this->getLists()->nonRecursiveInOrder(classes_with_zero_views, copyEmptyClassesToArray, courses + numOfClasses - classes_with_zero_views, classes + numOfClasses - classes_with_zero_views);
    return SUCCESS;
}

int static hashFunctionDS(int num, int size = 16)
{
    return num%size;
}

int static hashFunctionDS(courseNode course, int size = 16)
{
    return course.getId()%size;
}
