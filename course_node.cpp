#include "course_node.h"

courseNode::courseNode(const int courseID) : course_id(courseID), num_of_classes(0),
                                             array(new DynamicArray<avlNode<classNode> >()) {}

courseNode::courseNode() : array(new DynamicArray<avlNode<classNode> >()) {}

courseNode::~courseNode()
{
    delete this->array;
}

void courseNode::setId(int new_id)
{
    this->course_id = new_id;
}
void courseNode::setNumOfClasses(int numOfClasses)
{
    this->num_of_classes = numOfClasses;
}

// void static printCourseNode2(avlNode<classNode> *node)
// {
//     std::cout << "=========================" << std::endl;
//     std::cout << "left child of " << node->getValue()->getKey() << " is: " << (node->getLeft() ? node->getLeft()->getValue()->getKey() : -1) << std::endl;
//     std::cout << "Right child of " << node->getValue()->getKey() << " is: " << (node->getRight() ? node->getRight()->getValue()->getKey() : -1) << std::endl;
//     std::cout << "parent of " << node->getValue()->getKey() << " is: " << (node->getParent() ? node->getParent()->getValue()->getKey() : -1) << std::endl;
//     std::cout << "time of " << node->getValue()->getKey() << " is: " << node->getValue()->getTime() << std::endl;
//     if (node->getParent())
//     {
//         if (node->isLeftChild())
//             std::cout << node->getValue()->getKey() << " is left child" << std::endl;
//         else
//             std::cout << node->getValue()->getKey() << " is right child" << std::endl;
//     }
//     std::cout << "=========================" << std::endl;
// }

avlNode<classNode> *courseNode::getClass(const int classID) const
{
    if (classID > num_of_classes)
        return NULL;

    avlNode<classNode> *result = this->array->find(classID);
    // std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    // printCourseNode2(result);
    return result;
}

CourseStatus courseNode::setClassPointer(int classID, avlNode<classNode> *class_ptr)
{
    if (!class_ptr)
        return COURSE_FAILURE;
    // printCourseNode2(class_ptr);
    if (classID == this->array->getSize())
        return (CourseStatus)this->array->add(class_ptr);
    // printCourseNode2(this->array->find(classID));
    else
        return (CourseStatus)this->array->update(class_ptr, classID);
}

bool courseNode::operator<(const courseNode &courseToCompare) const
{
    return this->course_id < courseToCompare.getKey();
}

bool courseNode::operator!() const
{
    if (getKey())
        return false;
    return true;
}

bool courseNode::operator==(const courseNode &courseToCompare) const
{
    return this->course_id == courseToCompare.getKey();
}

bool courseNode::operator<=(const courseNode &courseToCompare) const
{
    return this->course_id <= courseToCompare.getKey();
}
bool courseNode::operator>(const courseNode &courseToCompare) const
{
    return (this->course_id > courseToCompare.getKey());
}