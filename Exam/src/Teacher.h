#ifndef __EXAM_TEACHER_H_
#define __EXAM_TEACHER_H_

#include <omnetpp.h>
#include <string.h>
#include "Student_m.h"

using namespace omnetpp;
using namespace std;

class Teacher : public cSimpleModule
{
    private:
        int studentsNumber;
        Student* student;
        void askQuestion();
        bool examFinished(int n);
        void clearStudent();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage* msg);

    public:
        Teacher() {studentsNumber = 0;};
};

#endif
