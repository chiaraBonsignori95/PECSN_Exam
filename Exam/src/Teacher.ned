package exam;

simple Teacher
{
	parameters:
    	@signal[examFinished](type = simtime_t);
        @signal[studentExamined](type = int);
        @statistic[examinationTime](source = "examFinished"; record = vector);
        @statistic[studentsExaminedNumber](source = "studentExamined"; record = sum);       
}
