package exam.simulations;
import exam.*;

network ParallelExamSession
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
        @signal[student](type = int);
        @statistic[studentID](source = "student"; record = vector);
  
    submodules:
		teacher[teachersNumber]: Teacher;       
}
