#include "course_node.h"

courseNode::courseNode(const int courseID) :
        course_id(courseID), num_of_classes(0),
        classes_pointers_array(new avlNode<classNode> *[0]),
        // zero_views_node_pointers(new twListNode<int> *[0]),
        // zero_views_classes(new twList<int>(courseID))
{
    // for (int i = 0; i < this->num_of_classes; i++)
    // {
    //     *(this->classes_pointers_array + i) = nullptr;
    //     this->zero_views_classes->addLast(i);
    //     *(this->zero_views_node_pointers + i) = this->zero_views_classes->getTail()->getPrev();
    // }
}

courseNode::courseNode():
        classes_pointers_array(new avlNode<classNode> *[1]),
        // zero_views_node_pointers(new twListNode<int> *[1]),
        // zero_views_classes(new twList<int>())
{}

courseNode::~courseNode()
{
    // std::cout << "deleted course: " << this->course_id << std::endl;
    delete[] this->classes_pointers_array;
    // delete[] this->zero_views_node_pointers;
    // delete this->zero_views_classes;
}

void courseNode::setId(int new_id)
{
    this->course_id = new_id;
}
void courseNode::setNumOfClasses(int numOfClasses)
{
    this->num_of_classes = numOfClasses;
}

avlNode<classNode> *courseNode::getClass(const int classID)
{
    if (classID > num_of_classes)
    {
        return NULL;
    }
    return classes_pointers_array[classID];
}

CourseStatus courseNode::setClassPointer(int classID, avlNode<classNode> *class_ptr)
{
    if (!class_ptr)
        return COURSE_FAILURE;
    *(this->classes_pointers_array + classID) = class_ptr;
    return COURSE_SUCCESS;
}

bool courseNode::operator<(const courseNode& courseToCompare) const
{
    return this->course_id < courseToCompare.getId();
}

bool courseNode::operator!() const
{
    if (getId())
        return false;
    return true;
}

bool courseNode::operator==(const courseNode& courseToCompare) const
{
    return this->course_id == courseToCompare.getId();
}

bool courseNode::operator<=(const courseNode& courseToCompare) const
{
    return this->course_id <= courseToCompare.getId();
}
bool courseNode::operator>(const courseNode& courseToCompare) const
{
    return (this->course_id > courseToCompare.getId());
}