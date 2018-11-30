package exam.simulations;
import exam.*;

network ParallelExamSession
{
    parameters:
        int teachersNumber;
        bool uniformDistribution;
  
    submodules:
		teacher[teachersNumber]: Teacher;       
}
