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
 Handles the response of the student
 */
void CommunicatingTeacher::handleStudentResponse(cMessage *msg)
{
    updateStudentState(msg);
    busy = false;                       //the teacher is free and available to ask a new question to another student
    idleTimeStart = simTime();          //the starting time of the idle period is set in any case, this value will be used
                                        //in the handleNewStudent only when a new student arrives in the empty queue
    if(firstTeacher())
    {
        if(!lastTeacher())              //handle the case in which there is only 1 teacher
            send(msg, "nextTeacher");   //all the teachers (except the last one) send the student to the next teacher
        newStudent();
        askQuestion();
        busy = true;
    }

    if(lastTeacher())
    {
        Student *s = check_and_cast<Student*>(msg);
        emit(examFinishedSignal, s->getTotalAnswerTime());
        emit(studentExaminedSignal, 1);
        emit(studentWaitingTime, s->getWaitingTimeTotal());
        delete msg;                     //the student leaves the system
    }

    if(!firstTeacher() && !lastTeacher())
        send(msg, "nextTeacher");

    if(!waitingStudents.isEmpty())
    {
        student = check_and_cast<Student*>(waitingStudents.pop());
        simtime_t currentWaitingTime = student->getWaitingTimeTotal() + simTime() -  student->getWaitingTimeStart();
        student->setWaitingTimeTotal(currentWaitingTime);
        EV << student->getWaitingTimeTotal() << endl;
        askQuestion();
        busy = true;
    }
}


/*
 Handles a new student to examine
*/
void CommunicatingTeacher::handleNewStudent(cMessage *msg)
{
    Student *s = check_and_cast<Student*>(msg);
    s->setWaitingTimeStart(simTime());
    waitingStudents.insert(s);                  //if it isn't a selfMessage, it's for sure a student coming from the previous teacher
                                                //the student will be insert into the teacher's waiting queue and he will be scheduled
                                                //by handleNewStudent() function
    if(!busy)
    {
        student = check_and_cast<Student*>(waitingStudents.pop());    //get the reference to the oldest student
        askQuestion();
        busy = true;
        if(isFirstStudentSeen)
            idleTimeTotal += simTime() - idleTimeStart;
    }
    isFirstStudentSeen = true;
}


/*
 Registers signals for statistics
 */
void CommunicatingTeacher::registerSignals()
{
    Teacher::registerSignals();
    idleTime = registerSignal("idleTimeInterval");
    studentWaitingTime = registerSignal("waitingTimeInterval");
}


/*
 It is used only by the first and the last teachers
*/
void CommunicatingTeacher::initialize()
{
    if(firstTeacher())
    {
        isFirstStudentSeen = true;
        newStudent();
        askQuestion();
        busy = true;
    }

    registerSignals();
}


void CommunicatingTeacher::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())                //a selfMessage is always a student's answer
        handleStudentResponse(msg);
    else
        handleNewStudent(msg);
}


/*
 Clears the queue of students to avoid memory leak
 */
void CommunicatingTeacher::finish()
{
    waitingStudents.clear();
    emit(idleTime, idleTimeTotal);
}


/*
 * Another possible body oh the handleMessage function
 *
 *
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
            emit(examFinishedSignal, s->getTotalAnswerTime());
            emit(studentExaminedSignal, 1);
            delete msg;                 //the student leaves the system
        }

        if(!firstTeacher() && !waitingStudents.isEmpty())
        {
            student = check_and_cast<Student*>(waitingStudents.pop());    //get the reference to the oldest student
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
        Student *s = check_and_cast<Student*>(msg);
        waitingStudents.insert(s);      //if it isn't a selfMessage, it's for sure a student coming from the previous teacher
                                                //the student will be insert into the teacher's waiting queue and he will be scheduled
                                                //by handleNewStudent() function
        if(!busy)
        {
            student = check_and_cast<Student*>(waitingStudents.pop());    //get the reference to the oldest student
            askQuestion();
            busy = true;
        }
    }

    //handleNewStudent();
}

*/


