packet Student 
{
    simtime_t totalAnswerTime;		//examination time
    
    simtime_t waitingTimeTotal;
    simtime_t waitingTimeStart;
    
    int answersNumber;				//number of given answers
    double currentAnswerTime;		//time to answer the current question
}
