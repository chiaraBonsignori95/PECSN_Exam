packet Student 
{
    simtime_t totalAnswerTime;		//examination time
    
    simtime_t totalWaitingTime;
    simtime_t startingWaitingTime;
    
    int answersNumber;				//number of given answers
    double currentAnswerTime;		//time to answer the current question
}
