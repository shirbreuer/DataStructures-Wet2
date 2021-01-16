#include "library2.h"
#include "data_structure.h"

#define DataStructureIsValid(DS) \
        if (!DS)\
            return INVALID_INPUT;
void *Init()
{
    courseManager *DS = new courseManager();
    if (!DS)
        return NULL;
    return (void *)DS;
}

StatusType AddCourse(void *DS, int courseID)
{
    DataStructureIsValid(DS);
    return (((courseManager *)DS)->AddCourse(courseID));
}
StatusType RemoveCourse(void *DS, int courseID)
{
    DataStructureIsValid(DS);
    return ((courseManager *)DS)->RemoveCourse(courseID);
}

StatusType AddClass(void *DS, int courseID, int* classID)
{
    DataStructureIsValid(DS);
    return ((courseManager *)DS)->AddClass(courseID, classID);
}


StatusType WatchClass(void *DS, int courseID, int classID, int time)
{
    DataStructureIsValid(DS);
    return ((courseManager *)DS)->WatchClass(courseID, classID, time);
}

StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed)
{
    DataStructureIsValid(DS);
    return ((courseManager *)DS)->TimeViewed(courseID, classID, timeViewed);
}

StatusType GetIthWatchedClass(void *DS, int numOfClasses, int *courses, int *classes)
{
    DataStructureIsValid(DS);
    std::cout << "GetIthWatchedClass" << std::endl;
    return SUCCESS;
    return ((courseManager *)DS)->GetIthWatchedClass(numOfClasses, courses, classes);
}

void Quit(void** DS)
{
    if (!(DS))
        return;
    delete ((courseManager *)*DS);
    *DS = NULL;
}