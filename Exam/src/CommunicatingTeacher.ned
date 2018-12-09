package exam;

simple CommunicatingTeacher extends Teacher
{
    @class(CommunicatingTeacher);
    
    gates:
        input previousTeacher @loose;
        output nextTeacher @loose;
}
