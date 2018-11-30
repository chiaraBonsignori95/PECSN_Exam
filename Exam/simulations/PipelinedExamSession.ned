package exam.simulations;
import exam.*;

network PipelinedExamSession
{
    parameters:
        int teachersNumber;
        bool uniformDistribution;
  
    submodules:
    	teacher[teachersNumber]: CommunicatingTeacher;
        
    connections:
  		teacher[i].nextTeacher --> teacher[i+1].previousTeacher for i = 0..teachersNumber-2;   
}
