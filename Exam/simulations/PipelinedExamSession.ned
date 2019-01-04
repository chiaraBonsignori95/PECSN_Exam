package exam.simulations;
import exam.*;

network PipelinedExamSession
{
    parameters:
        int teachersNumber;
        string answerTimeDistribution;
        int minUniform;
        int maxUniform;
        double scaleLognormal;
        double shapeLognormal;
        
        @signal[examFinished](type = simtime_t);
        @statistic[examinationTime](source = "examFinished"; record = vector);
        @signal[idleTime](type = simtime_t);
        @statistic[teachersIdleTime](source = "idleTime"; record = vector); 
        @signal[waitingTime](type = simtime_t);
        @statistic[studentsWaitingTime](source = "waitingTime"; record = vector); 
  
    submodules:
    	teacher[teachersNumber]: PipelinedTeacher;
        
    connections:
  		teacher[i].nextTeacher <--> teacher[i+1].previousTeacher for i = 0..teachersNumber-2;   
}
