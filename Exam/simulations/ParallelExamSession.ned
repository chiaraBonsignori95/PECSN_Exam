package exam.simulations;
import exam.*;

network ParallelExamSession
{
    parameters:       
        int teachersNumber;
        string answerTimeDistribution;
        int minUniform;
        int maxUniform;
        @signal[examFinished](type = simtime_t);
        @statistic[examinationTime](source = "examFinished"; record = vector);
  
    submodules:
		teacher[teachersNumber]: Teacher;       
}
