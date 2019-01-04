#ifndef CEXAMINATIONTIME_H_
#define CEXAMINATIONTIME_H_

#include <omnetpp.h>
using namespace omnetpp;

class cExaminationTime: public cObject, noncopyable
{
    public:
        long studentID;
        simtime_t examinationTime;
};

#endif
