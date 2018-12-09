package exam;

simple CommunicatingTeacher extends Teacher
{    
    parameters:
        @class(CommunicatingTeacher); 
        
    gates:
        input previousTeacher @loose;
        output nextTeacher @loose;
}
