#ifndef __EXAM_PIPELINEDTEACHER_H_
#define __EXAM_PIPELINEDTEACHER_H_

#include <omnetpp.h>
#include "Teacher.h"

using namespace omnetpp;


/***************************************************************************************

PipelinedTeacher is the class that extends Teacher to model teacher's behaviour
in "Pipelined" model.

****************************************************************************************/

class PipelinedTeacher : public Teacher
{
private:
        bool busy;
        bool newIncomingStudent;
        simsignal_t idleTimeSignal, waitingTimeSignal;
        simtime_t idleTimeStart, idleTimeTotal;
        int studentCounter;
        bool firstTeacher();
        bool lastTeacher();
        bool newStudentAvailable(cMessage *msg);
        bool teacherNotBusy(cMessage *msg);
        void handleStudentResponse(cMessage *msg);
        void handleTeacherMessage(cMessage *msg);

    protected:
        void registerSignals();
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void finish();

    public:
        PipelinedTeacher() {busy = false; newIncomingStudent = false; idleTimeStart = 0; idleTimeTotal = 0; studentCounter = 0;};
        ~PipelinedTeacher() {cancelAndDelete(student);};
};


#endif
