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
    student->setStartingWaitingTime(simTime());

    idleTimeStart = simTime();

    if(!lastTeacher())
    {
        cMessage *handshake = new cMessage("New student available");
        send(handshake, "nextTeacher$o");

    }
    else
    {
        //DEBUG
        EV << student->getTotalAnswerTime() << endl;
        //DEBUG

        /*   collect statistics   */
        emit(examFinishedSignal, student->getTotalAnswerTime() + student->getTotalWaitingTime());
        emit(waitingTimeSignal, student->getTotalWaitingTime());

        //DEBUG
        EV << "Student " << student->getId() << " deleted"  << endl;
        //DEBUG

        delete student;
        student = NULL;
        busy = false;

        if(newIncomingStudent)
        {
            cMessage *handshake = new cMessage("Teacher no more busy");
            send(handshake, "previousTeacher$o");
            newIncomingStudent = false;
        }

        if(firstTeacher())
        {
            //DEBUG
            EV << "Not allowed to be here with more than 1 teacher" << endl;
            //DEBUG
            newStudent();
            askQuestion();
            busy = true;
        }
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
    if(newStudentAvailable(msg))
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
        //DEBUG
        EV << "Student " << student->getId()  << " sent to next teacher" << endl;
        //DEBUG

        simtime_t currentTotalWaitingTime = student->getTotalWaitingTime();
        student->setTotalWaitingTime(currentTotalWaitingTime +  simTime() - student->getStartingWaitingTime());

        idleTimeTotal += simTime() - idleTimeStart;

        send(student, "nextTeacher$o");
        student = NULL;
        busy = false;

        idleTimeStart = simTime();

        if(newIncomingStudent)
        {
            msg->setName("Teacher no more busy");
            send(msg, "previousTeacher$o");
            newIncomingStudent = false;
        }
        else
            delete msg;

        if(firstTeacher())
        {
            newStudent();
            askQuestion();
            busy = true;
        }
    }
    else //is a student
    {
        student = check_and_cast<Student*>(msg);
        askQuestion();
        busy = true;

        idleTimeTotal += simTime() - idleTimeStart;
    }
}


/*
 Registers signals for statistics
*/
void PipelinedTeacher::registerSignals()
{
    Teacher::registerSignals();
    idleTimeSignal = registerSignal("idleTime");
    waitingTimeSignal = registerSignal("waitingTime");
}


/*
 It is used only by the first and the last teachers
*/
void PipelinedTeacher::initialize()
{
    //DEBUG
    WATCH(busy);
    WATCH(newIncomingStudent);
    //DEBUG

    if(firstTeacher())
    {
        newStudent();
        askQuestion();
        busy = true;
    }

    registerSignals();
}


void PipelinedTeacher::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())                //a selfMessage is always a student's answer
        handleStudentResponse(msg);
    else
        handleTeacherMessage(msg);
}



void PipelinedTeacher::finish()
{
    emit(idleTimeSignal, idleTimeTotal);
}





