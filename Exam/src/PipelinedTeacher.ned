package exam;

simple PipelinedTeacher extends Teacher
{    
    parameters:
        @class(PipelinedTeacher); 
        
    gates:
        inout previousTeacher @loose;
        inout nextTeacher @loose;
}
