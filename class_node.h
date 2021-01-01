#ifndef CLASS_NODE_H
#define CLASS_NODE_H

#include "avl_node.hpp"

class classNode
{
private:
    const int course_id;
    const int class_id;
    void *parent;
    int time;

public:
    classNode() = default;
    classNode(const classNode& other);
    classNode(const int course_id, const int class_id, void* parent, int time);
    classNode(const int course_id, const int class_id);
    ~classNode() = default;
    int getTime() { return this->time; }
    int getClassId() { return this->class_id; }
    int getCourseId() { return this->course_id; }
    const int getCourseId() const { return this->course_id; }
    void* getParentPointer() { return this->parent;}
    classNode& operator=(const classNode& other);
    bool operator==(const classNode& other) const;
    bool operator<(const classNode& other) const;
    bool operator!() const;

};

#endif