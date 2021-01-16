import random
import sys

a = 1
b = 70


class BoomSystem:
    def __init__(self,InFile,OutFile):
        self.Courses = {}
        self.Classes = {}
        self.init = False
        self.functions = [self.AddCourseRand, self.RemoveCourseRand,
                          self.AddClassRand, #self.GetIthWatchedClassRand,
                          self.WatchClassRand, self.TimeViewedRand]
        self.functions_with_init = self.functions + [self.SetInit, self.Quit]
        self.InputTest = open(InFile, 'w', newline='\n')
        self.OutputTest = open(OutFile, 'w', newline='\n')

    def SetInit(self):
        self.InputTest.write("Init\n")
        if self.init:
            self.OutputTest.write("init was already called.\n")
        else:
            self.init = True
            self.OutputTest.write("init done.\n")

    def AddCourse(self, CourseID):
        command = f"AddCourse {CourseID}\n"
        self.InputTest.write(command)
        if CourseID <= 0 or self.init is False:
            self.OutputTest.write("AddCourse: INVALID_INPUT\n")
            return
        if self.Courses.get(CourseID):
            self.OutputTest.write("AddCourse: FAILURE\n")
            return
        self.Courses[CourseID] = [CourseID, 0]
        self.OutputTest.write("AddCourse: SUCCESS\n")

    def RemoveCourse(self, CourseID):
        command = f"RemoveCourse {CourseID}\n"
        self.InputTest.write(command)
        if CourseID <= 0 or self.init is False:
            self.OutputTest.write("RemoveCourse: INVALID_INPUT\n")
            return
        currCourse = self.Courses.get(CourseID)
        if currCourse is None:
            self.OutputTest.write("RemoveCourse: FAILURE\n")
            return
        for x in range(0, currCourse[1]):
            self.Classes.pop((CourseID, x))
        self.Courses.pop(CourseID)
        self.OutputTest.write("RemoveCourse: SUCCESS\n")

    def AddClass(self, CourseID):
        command = f"AddClass {CourseID}\n"
        self.InputTest.write(command)
        if CourseID <= 0 or self.init is False:
            self.OutputTest.write("AddClass: INVALID_INPUT\n")
            return
        if self.Courses.get(CourseID) is None:
            self.OutputTest.write("AddClass: FAILURE\n")
            return
        ClassNum = self.Courses[CourseID][1]
        self.Courses[CourseID] = [CourseID, ClassNum + 1]
        self.Classes[CourseID, ClassNum] = 0
        self.OutputTest.write(f"AddClass: {ClassNum}\n")

    def WatchClass(self, CourseID, ClassID, time):
        command = f"WatchClass {CourseID} {ClassID} {time}\n"
        self.InputTest.write(command)
        if CourseID <= 0 or time <= 0 or ClassID < 0 or self.init is False:
            self.OutputTest.write("WatchClass: INVALID_INPUT\n")
            return
        if self.Courses.get(CourseID) is None:
            self.OutputTest.write("WatchClass: FAILURE\n")
            return
        ClassNum = self.Courses[CourseID][1]
        if ClassNum <= ClassID:
            self.OutputTest.write("WatchClass: INVALID_INPUT\n")
            return
        streamTime = self.Classes[CourseID, ClassID]
        self.Classes[CourseID, ClassID] = streamTime + time
        self.OutputTest.write("WatchClass: SUCCESS\n")

    def GetClasses(self):
        retList = []
        for x in self.Classes:
            retList.append([x[0], x[1], self.Classes[x]])
        return retList

    def TimeViewed(self, courseID, classID):
        command = f"TimeViewed {courseID} {classID}\n"
        self.InputTest.write(command)
        if courseID <= 0 or classID < 0 or self.init is False:
            self.OutputTest.write("TimeViewed: INVALID_INPUT\n")
            return
        testVar = self.Courses
        # len(self.Courses) == 0 or
        if self.Courses.get(courseID) is None:
            self.OutputTest.write("TimeViewed: FAILURE\n")
            return
        if self.Courses[courseID][1] < classID + 1:
            self.OutputTest.write("TimeViewed: INVALID_INPUT\n")
            return
        self.OutputTest.write(f"TimeViewed: {self.Classes[(courseID, classID)]}\n")

    # def GetIthWatchedClass(self, i):
    #     command = f"GetIthWatchedClass {i}\n"
    #     self.InputTest.write(command)
    #     if i <= 0 or self.init is False:
    #         self.OutputTest.write("GetIthWatchedClass: INVALID_INPUT\n")
    #         return
    #     new_list = sorted(self.GetClasses(), key=lambda x: (-x[2], x[0], x[1]))
    #     sizeOf = len(self.Classes)
    #     if sizeOf < i or new_list[i - 1][2] == 0:
    #         self.OutputTest.write("GetIthWatchedClass: FAILURE\n")
    #         return
    #     self.OutputTest.write(f"GetIthWatchedClass: {new_list[i - 1][0]} {new_list[i - 1][1]}\n")

    def Quit(self):
        self.InputTest.write("Quit\n")
        self.Classes = {}
        self.Courses = {}
        self.init = False
        self.OutputTest.write("quit done.\n")

    def Do_Random_Func(self):
        func = random.randint(0, len(self.functions_with_init) - 1)
        self.functions_with_init[func]()

    def Do_Random_Func_No_Init_No_Quit(self):
        func = random.randint(0, len(self.functions) - 1)
        self.functions[func]()

    def AddCourseRand(self):
        self.AddCourse(random.randint(a, b))

    def RemoveCourseRand(self):
        self.RemoveCourse(random.randint(a, b))

    def AddClassRand(self):
        self.AddClass(random.randint(a, b))

    # def GetIthWatchedClassRand(self):
    #     self.GetIthWatchedClass(random.randint(a, b))

    def WatchClassRand(self):
        self.WatchClass(random.randint(a, b), random.randint(a, b), random.randint(a, b))

    def TimeViewedRand(self):
        self.TimeViewed(random.randint(a, b), random.randint(a, b))


def make_random_test(length,inputTest,outputTest):
    BM = BoomSystem(inputTest,outputTest)
    with_init = int(length / 10)
    no_init = int(length * 8 / 10)

    for i in range(with_init):
        BM.Do_Random_Func()

    BM.SetInit()
    for i in range(no_init):
        BM.Do_Random_Func_No_Init_No_Quit()

    for i in range(with_init):
        BM.Do_Random_Func()

    BM.Quit()
    BM.InputTest.close()
    BM.OutputTest.close()


def main():
    size_of_test = (int)(sys.argv[1])
    num_of_tests = (int)(sys.argv[2])
    for x in range(0,num_of_tests):
        make_random_test(size_of_test, f"Input_{x}.txt", f"expOut_{x}.txt")


main()