#include "CommunicatingTeacher.h"

Define_Module(CommunicatingTeacher);

/*
 Checks if the selected teacher is the first one
*/
bool CommunicatingTeacher::firstTeacher()
{
    return getIndex() == 0;
}


/*
 Checks if the selected teacher is the last one
*/
bool CommunicatingTeacher::lastTeacher()
{
    return getIndex() == getTotalQuestionsNumber() - 1;
}


/*
 Checks if there are any students that are waiting to be examined
*/
void CommunicatingTeacher::handleNewStudent()
{
    if(firstTeacher())                              //the first teacher must create the new student
        newStudent();
    else if(!waitingStudents.empty())      //the others teachers, if available, must verify if there are any students waiting to be examined
    {
        student = check_and_cast<Student*>(waitingStudents.front());    //get the reference to the oldest student
        waitingStudents.pop();                                          //delete the oldest student
    }

    askQuestion();
    busy = true;
}


/*
 It is used only by the first teacher
*/
void CommunicatingTeacher::initialize()
{
    if(firstTeacher())
    {
        Teacher::initialize();
        busy = true;
    }
}


/*

*/
void CommunicatingTeacher::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())            //a selfMessage is always a student's answer
    {
        updateStudentState(msg);
        busy = false;                   //the teacher is free and available to ask a new question to another student

        if(!lastTeacher())
        {
            send(msg, "nextTeacher");   //all the teachers (except the last one) send the student to the next teacher
            EV << "Send student" << endl;
        }
        else
        {
            EV << "I'm last teacher" << endl;
            Student *s = check_and_cast<Student*>(msg);
            //emit(examFinishedSignal, s->getTotalAnswerTime());
            //emit(studentExaminedSignal, 1);
            delete msg;                 //the student leaves the system
        }

        if(!firstTeacher() && !waitingStudents.empty())
        {
            student = check_and_cast<Student*>(waitingStudents.front());    //get the reference to the oldest student
            waitingStudents.pop();
            askQuestion();
            busy = true;
        }

        if(firstTeacher())
        {
            newStudent();
            askQuestion();
            busy = true;
        }
    }
    else
    {
        waitingStudents.push(msg);      //if it isn't a selfMessage, it's for sure a student coming from the previous teacher
                                                //the student will be insert into the teacher's waiting queue and he will be scheduled
                                                //by handleNewStudent() function
        if(!busy)
        {
            student = check_and_cast<Student*>(waitingStudents.front());    //get the reference to the oldest student
            waitingStudents.pop();
            askQuestion();
            busy = true;
        }
    }

    //handleNewStudent();
}


void CommunicatingTeacher::finish()
{
    cancelEvent(student);
    cancelAndDelete(student);

    EV << waitingStudents.size() << endl;
    while(!waitingStudents.empty())
    {
        EV << "I'm deleting" << endl;
        waitingStudents.pop();
    }
    EV << waitingStudents.size() << endl;
}


/*
 * Another possible body oh the handleMessage function
 *
 *
if(firstTeacher())
{
    send(msg, "nextTeacher");
    newStudent();
    askQuestion();
    busy = true;
}
else if(lastTeacher())
{
    //statistic for student
    delete msg;
    handleNewStudent();
}
else
{
    send(msg, "nextTeacher");
    handleNewStudent();
}
*/


