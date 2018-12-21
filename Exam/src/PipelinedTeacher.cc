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
 Returns true if the previous teacher has a new student ready to be sent
 (in this case he sent the "New student available" message)
*/
bool PipelinedTeacher::newStudentAvailable(cMessage *msg)
{
    return strcmp(msg->getName(), "New student available") == 0;
}


/*
 Returns true if the following teacher isn't busy and is ready to receive a new student
 (in this case he sent the "Teacher no more busy" message)
*/
bool PipelinedTeacher::teacherNotBusy(cMessage *msg)
{
    return strcmp(msg->getName(), "Teacher no more busy") == 0;
}


/*
 Handles the response of the student (that is always a self message)
*/
void PipelinedTeacher::handleStudentResponse(cMessage *msg)
{
    updateStudentState(msg);
    student->setStartingWaitingTime(simTime());

    idleTimeStart = simTime();  //initialized to count a possible idle time period that can occur if there isn't a new student to examine
                                //or if is not possible to send immediately the student just examined to the next teacher
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

        //collect statistics
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

        //used to handle the situation with only one teacher (in this case he is both the first and the last of the pipeline)
        //if the number of teacher is greater than one, this statement is not used any more
        if(firstTeacher())
        {
            newStudent();
            askQuestion();
            busy = true;
        }
    }
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

        idleTimeTotal += simTime() - idleTimeStart;     //when the next colleague is no more busy,
                                                        //the teacher can stop to wait and send to him the student
        send(student, "nextTeacher$o");
        student = NULL;
        busy = false;

        idleTimeStart = simTime();                      //the idle time starts again because it may happen that
                                                        //the previous teacher isn't ready to send a new student
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

        idleTimeTotal += simTime() - idleTimeStart;     //the idle time is stopped because the teacher can start to examine the incoming student
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


void PipelinedTeacher::initialize()
{
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
    if(msg->isSelfMessage())
        handleStudentResponse(msg);         //handles a selfMessage, that is always a student's answer
    else
        handleTeacherMessage(msg);          //handles communication between teachers and student exchanges
}


/*
 The teachers idle time is emitted only at the end of the simulation.
 Other statistics are related to students and are emitted by the last teacher of the pipeline
 */
void PipelinedTeacher::finish()
{
    emit(idleTimeSignal, idleTimeTotal);
}





