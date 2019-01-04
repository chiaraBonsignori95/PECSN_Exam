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
        
        @signal[examFinished](type = cObject*);
        @statistic[examinationTime](source = "examFinished"; record = vector);
  
    submodules:
		teacher[teachersNumber]: Teacher;       
}
