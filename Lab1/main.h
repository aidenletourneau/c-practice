struct Student {
	int student_ID;
	int g1, g2, g3;
	float average;
};

//function prototypes

struct Student getMaxAverageStudent(struct Student *s, int n); 

void readData(struct Student *students, int *c);