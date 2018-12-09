#ifndef __EXAM_COMMUNICATINGTEACHER_H_
#define __EXAM_COMMUNICATINGTEACHER_H_

#include <omnetpp.h>
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
        cQueue waitingStudents;   //contains the students waiting to be examined by the teacher -- FIFO q.--
        simsignal_t idleTime, studentWaitingTime;
        simtime_t idleTimeTotal, idleTimeStart;
        bool isFirstStudentSeen;    //used to wait that the pipeline is full before compute the idle time
        bool firstTeacher();
        bool lastTeacher();
        void handleStudentResponse(cMessage *msg);
        void handleNewStudent(cMessage *msg);

    protected:
        void registerSignals();
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

    public:
        CommunicatingTeacher() {busy = false; idleTimeStart = 0; idleTimeTotal = 0; isFirstStudentSeen = false;};
};

#endif
