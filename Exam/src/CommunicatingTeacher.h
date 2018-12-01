#ifndef __EXAM_COMMUNICATINGTEACHER_H_
#define __EXAM_COMMUNICATINGTEACHER_H_

#include <omnetpp.h>
#include <queue>
#include "Teacher.h"

using namespace omnetpp;
using namespace std;

/***************************************************************************************

CommunicatingTeacher is the class that extends Teacher to model teacher's behaviour
in "Pipelined" model.

****************************************************************************************/

class CommunicatingTeacher : public Teacher
{
    private:
        bool busy;
        bool firstTeacher();
        bool lastTeacher();
        queue<cMessage*> waitingStudents;   //contains the students waiting to be examined by the teacher -- FIFO q.--

    protected:
        void handleNewStudent();
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        CommunicatingTeacher() {busy = false;};
};

#endif
