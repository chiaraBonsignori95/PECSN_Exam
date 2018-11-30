#include "Teacher.h"

Define_Module(Teacher);

void Teacher::askQuestion()
{
    double answerTime;
    simtime_t previousTime = student->getTotalAnswerTime();
    if(getParentModule()->par("uniformDistribution").boolValue())
        answerTime = uniform(0, 3);
    else
        answerTime = lognormal(0, 3);
    student->setTotalAnswerTime(previousTime + answerTime);
    scheduleAt(simTime() + answerTime, student);
}

bool Teacher::examFinished(int n)
{
    return n == getParentModule()->par("teachersNumber").intValue();
}

void Teacher::clearStudent()
{
    student->setQuestionNumber(0);
    student->setTotalAnswerTime(0);
}

void Teacher::initialize()
{
    string name = "Student " + to_string(studentsNumber);
    student = new Student(name.c_str());
    askQuestion();
}

void Teacher::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
       student = check_and_cast<Student*>(msg);
       int currentQuestionNumber = student->getQuestionNumber();
       currentQuestionNumber++;
       student->setQuestionNumber(currentQuestionNumber);
       askQuestion();

       if(examFinished(currentQuestionNumber))
       {
           studentsNumber++;
           clearStudent();
       }
    }
}
