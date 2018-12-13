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
        @signal[idleTimeInterval](type = simtime_t);
        @statistic[teachersIdleTime](source = "idleTimeInterval"; record = vector); 
        @signal[waitingTimeInterval](type = simtime_t);
        @statistic[studentsWaitingTime](source = "waitingTimeInterval"; record = vector); 
  
    submodules:
    	teacher[teachersNumber]: CommunicatingTeacher;
        
    connections:
  		teacher[i].nextTeacher --> teacher[i+1].previousTeacher for i = 0..teachersNumber-2;   
}
