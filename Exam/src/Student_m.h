//
// Generated file, do not edit! Created by nedtool 5.4 from Student.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __STUDENT_M_H
#define __STUDENT_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>Student.msg:2</tt> by nedtool.
 * <pre>
 * packet Student
 * {
 *     int studentID;
 * 
 *     simtime_t totalAnswerTime;		//examination time
 * 
 *     simtime_t totalWaitingTime;
 *     simtime_t startingWaitingTime;
 * 
 *     int answersNumber;				//number of given answers
 *     double currentAnswerTime;		//time to answer the current question
 * }
 * </pre>
 */
class Student : public ::omnetpp::cPacket
{
  protected:
    int studentID;
    ::omnetpp::simtime_t totalAnswerTime;
    ::omnetpp::simtime_t totalWaitingTime;
    ::omnetpp::simtime_t startingWaitingTime;
    int answersNumber;
    double currentAnswerTime;

  private:
    void copy(const Student& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Student&);

  public:
    Student(const char *name=nullptr, short kind=0);
    Student(const Student& other);
    virtual ~Student();
    Student& operator=(const Student& other);
    virtual Student *dup() const override {return new Student(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getStudentID() const;
    virtual void setStudentID(int studentID);
    virtual ::omnetpp::simtime_t getTotalAnswerTime() const;
    virtual void setTotalAnswerTime(::omnetpp::simtime_t totalAnswerTime);
    virtual ::omnetpp::simtime_t getTotalWaitingTime() const;
    virtual void setTotalWaitingTime(::omnetpp::simtime_t totalWaitingTime);
    virtual ::omnetpp::simtime_t getStartingWaitingTime() const;
    virtual void setStartingWaitingTime(::omnetpp::simtime_t startingWaitingTime);
    virtual int getAnswersNumber() const;
    virtual void setAnswersNumber(int answersNumber);
    virtual double getCurrentAnswerTime() const;
    virtual void setCurrentAnswerTime(double currentAnswerTime);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Student& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Student& obj) {obj.parsimUnpack(b);}


#endif // ifndef __STUDENT_M_H

