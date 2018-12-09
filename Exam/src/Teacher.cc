#include "Teacher.h"

Define_Module(Teacher);

/*
 questionNumber represents the i-th question asked by the teacher
 This function checks if a student answered to all questions
*/
bool Teacher::examFinished(int questionNumber)
{
    return questionNumber == getTotalQuestionsNumber();
}


/*
 Resets fields for each new student examined by the teacher,
 in this way we can reuse the same member student without creating a new one
*/
void Teacher::clearStudent()
{
    student->setAnswersNumber(0);
    student->setTotalAnswerTime(0);
}


/*
 Returns the value of the network parameter answerTimeDistribution
*/
const char* Teacher::getDistribution()
{
    return getParentModule()->par("answerTimeDistribution").stringValue();
}


/*
 Checks if the used answer time distribution is uniform
*/
bool Teacher::isUniform(const char* distribution)
{
    return strcmp(distribution, "uniform") == 0;
}


/*
Returns the minimum value for the uniform distribution
*/
int Teacher::getMinUniform()
{
   return getParentModule()->par("minUniform").intValue();
}


/*
Returns the maximum value for the uniform distribution
*/
int Teacher::getMaxUniform()
{
   return getParentModule()->par("maxUniform").intValue();
}


/*
 Checks if the used answer time distribution is lognormal
*/
bool Teacher::isLognormal(const char* distribution)
{
    return strcmp(distribution, "lognormal") == 0;
}


/*
 Registers signals for statistics
 */
void Teacher::registerSignals()
{
    examFinishedSignal = registerSignal("examFinished");
    studentExaminedSignal = registerSignal("studentExamined");
}


/*
 Returns the value of the network parameter teachersNumber
*/
int Teacher::getTotalQuestionsNumber()
{
    return getParentModule()->par("teachersNumber").intValue();
}


/*
 Creates a new student
*/
void Teacher::newStudent()
{
    student = new Student("Student");
    //DEBUG
    EV << "New student number " << student->getId() << endl;
    //DEBUG
}


/*
 Simulates the student's answer time according to the used distribution
 generating a new event for the teacher after the computed time
*/
void Teacher::askQuestion()
{
    double answerTime = 0.0;
    if(isUniform(getDistribution()))
        answerTime = uniform(getMinUniform(), getMaxUniform());
    else if(isLognormal(getDistribution()))
        answerTime = lognormal(0, 3);
    student->setCurrentAnswerTime(answerTime);
    //answerTime = 1; // debugging statistics
    scheduleAt(simTime() + answerTime, student);

    //DEBUG
    EV << "Question done to student  " << student->getId() << endl;
    //DEBUG
}


/*
 Updates the partial examination time and the number of completed questions
*/
void Teacher::updateStudentState(cMessage *msg)
{
    student = check_and_cast<Student*>(msg);
    simtime_t totalTime = student->getTotalAnswerTime();
    totalTime += student->getCurrentAnswerTime();
    student->setTotalAnswerTime(totalTime);

    int currentAnswersNumber = student->getAnswersNumber();
    currentAnswersNumber++;
    student->setAnswersNumber(currentAnswersNumber);
}


/*
 Creates new students and questions for all the teachers
*/
void Teacher::initialize()
{
    registerSignals();
    newStudent();
    askQuestion();
}


/*
 When the answer time elapsed student's fields are updated and a new question is asked.
 At the end of the n-th question, the student's fields are reset to simulate the new student's arrival
*/
void Teacher::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
       updateStudentState(msg);

       if(examFinished(student->getAnswersNumber()))
       {
           Student *s = check_and_cast<Student*>(msg);
           emit(examFinishedSignal, s->getTotalAnswerTime());
           emit(studentExaminedSignal, 1);
           clearStudent();
           EV << "Collect statistics" << endl;
       }

       askQuestion();
    }
}
