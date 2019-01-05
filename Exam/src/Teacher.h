#ifndef __EXAM_TEACHER_H_
#define __EXAM_TEACHER_H_

#include <omnetpp.h>
#include "Student_m.h"

using namespace omnetpp;
using namespace std;

/**********************************************************************************************

Teacher is the class that implements the teacher's behaviour in "One student per teacher" model.

***********************************************************************************************/

class Teacher : public cSimpleModule
{
    private:
        bool examFinished(int questionNumber);
        void clearStudent();

    protected:
        Student* student;
        simsignal_t examFinishedSignal, studentIDSignal;
        const char* getDistribution();
        bool isUniform(const char* distribution);
        int getMinUniform();
        int getMaxUniform();
        bool isLognormal(const char* distribution);
        double getScaleLognormal();
        double getShapeLognormal();
        void registerSignals();
        int getTotalQuestionsNumber();
        void newStudent();
        void askQuestion();
        void updateStudentState(cMessage *student);
        virtual void initialize();
        virtual void handleMessage(cMessage* msg);
};

#endif
