#include "PipelinedTeacher.h"

Define_Module(PipelinedTeacher);

/*
 Checks if the selected teacher is the first one
*/
bool PipelinedTeacher::firstTeacher()
{
    return getIndex() == 0;
}


/*
 Checks if the selected teacher is the last one
*/
bool PipelinedTeacher::lastTeacher()
{
    return getIndex() == getTotalQuestionsNumber() - 1;
}


/*
 Handles the response of the student (that is a self message)
*/
void PipelinedTeacher::handleStudentResponse(cMessage *msg)
{
    updateStudentState(msg);
    busy = false;

    if(!lastTeacher())
    {
        cMessage *handshake = new cMessage("New student available");
        send(handshake, "nextTeacher$o");
    }
    else
    {
        //update statistics
        delete msg;
        student = NULL;
    }
}


bool PipelinedTeacher::newStudentAvailable(cMessage *msg)
{
    return strcmp(msg->getName(), "New student available") == 0;
}


bool PipelinedTeacher::teacherNotBusy(cMessage *msg)
{
    return strcmp(msg->getName(), "Teacher no more busy") == 0;
}


/*
 Handles a message received from another teacher
 */
void PipelinedTeacher::handleTeacherMessage(cMessage *msg)
{
    if(newStudentAvailable(msg))    //handshake message
    {
        if(busy)
        {
            newIncomingStudent = true;
            delete msg;
        }
        else
        {
            msg->setName("Teacher no more busy");
            send(msg, "previousTeacher$o");
        }
    }
    else if(teacherNotBusy(msg))
    {
        send(student, "nextTeacher$o");
        student = NULL;
        if(firstTeacher())
        {
            delete msg;
            newStudent();
            askQuestion();
        }
        else
        {
           if(newIncomingStudent)
           {
               cMessage *handshake = new cMessage("Teacher no more busy");
               send(handshake, "previousTeacher$o");
           }
           else
               delete msg;
        }
    }
    else    //new student arrived
    {
        student = check_and_cast<Student*>(msg);
        askQuestion();
        busy = true;
        newIncomingStudent = false;
    }
}


/*
 Registers signals for statistics

void PipelinedTeacher::registerSignals()
{
    Teacher::registerSignals();
    idleTime = registerSignal("idleTimeInterval");
    studentWaitingTime = registerSignal("waitingTimeInterval");
}


 It is used only by the first and the last teachers
*/
void PipelinedTeacher::initialize()
{
    if(firstTeacher())
    {
        newStudent();
        askQuestion();
        busy = true;
    }

    //registerSignals();
}


void PipelinedTeacher::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())                //a selfMessage is always a student's answer
        handleStudentResponse(msg);
    else
        handleTeacherMessage(msg);
}


/*
 Clears the queue of students to avoid memory leak
 */
void PipelinedTeacher::finish()
{

    //emit(idleTime, idleTimeTotal);
}





