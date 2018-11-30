package exam;

simple CommunicatingTeacher extends Teacher
{
    gates:
        input previousTeacher @loose;
        output nextTeacher @loose;
}
