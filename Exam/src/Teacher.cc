#include "Teacher.h"

Define_Module(Teacher);

/*
 questionNumber represents the i-th question asked by the teacher
 This function checks if a student has answered to all questions
*/
bool Teacher::examFinished(int questionNumber)
{
    return questionNumber == getTotalQuestionsNumber();
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
Returns the minimum value parameter for the uniform distribution
*/
int Teacher::getMinUniform()
{
    return getParentModule()->par("minUniform").intValue();
}


/*
Returns the maximum value parameter for the uniform distribution
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
Returns the scale parameter for the lognormal distribution (that is the mean of the corresponding Normal distribution)
*/
double Teacher::getScaleLognormal()
{
    return getParentModule()->par("scaleLognormal").doubleValue();
}


/*
Returns the shape parameter for the lognormal distribution (that is the standard deviation of the corresponding Normal distribution)
*/
double Teacher::getShapeLognormal()
{
    return getParentModule()->par("shapeLognormal").doubleValue();
}


/*
 Registers signals for statistics
 */
void Teacher::registerSignals()
{
    examFinishedSignal = registerSignal("examFinished");
    studentIDSignal = registerSignal("student");
}


/*
 Returns the value of the network parameter teachersNumber
*/
int Teacher::getTotalQuestionsNumber()
{
    return getParentModule()->par("teachersNumber").intValue();
}


/*
 Creates a new student and associates to him a unique ID
*/
void Teacher::newStudent()
{
    student = new Student("Student");
    student->setStudentID((int)student->getTotalMessageCount());
}


/*
 Simulates the student's answer time according to the used distribution and
 schedules a new event for the teacher after this time
*/
void Teacher::askQuestion()
{
    double answerTime = 0.0;
    if(isUniform(getDistribution()))
        answerTime = uniform(getMinUniform(), getMaxUniform());
    else if(isLognormal(getDistribution()))
        answerTime = lognormal(getScaleLognormal(), getShapeLognormal());

    student->setCurrentAnswerTime(answerTime);
    scheduleAt(simTime() + answerTime, student);
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
 When the answer time elapses, updates student's fields and asks a new question.
 At the end of the n-th question, deletes the student and simulates a new student's arrival
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
           emit(studentIDSignal, s->getStudentID());
           delete s;
           newStudent();
       }

       askQuestion();
    }
}
