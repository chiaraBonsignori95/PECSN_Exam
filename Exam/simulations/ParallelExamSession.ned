package exam.simulations;
import exam.*;

network ParallelExamSession
{
    parameters:
        int teachersNumber;
        string answerTimeDistribution;
  
    submodules:
		teacher[teachersNumber]: Teacher;       
}
