#ifndef __EXAM_COMMUNICATINGTEACHER_H_
#define __EXAM_COMMUNICATINGTEACHER_H_

#include <omnetpp.h>

using namespace omnetpp;

class CommunicatingTeacher : public cSimpleModule
{
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
