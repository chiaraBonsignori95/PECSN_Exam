package exam.simulations;
import exam.*;

network ParallelExamSession
{
    parameters:       
        int teachersNumber;
        string answerTimeDistribution;
        int minUniform;
        int maxUniform;
  
    submodules:
		teacher[teachersNumber]: Teacher;       
}
