packet Student 
{        
    int studentID;					//is unique for each student and is used in statistics to associate 
    								//waitingTime and examinationTime to the corresponding student
    
    simtime_t totalAnswerTime;		//examination time
    
    simtime_t totalWaitingTime;
    simtime_t startingWaitingTime;
    
    int answersNumber;				//number of given answers
    double currentAnswerTime;		//time to answer the current question
}
