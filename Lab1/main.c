/* THIS SERVES AS A CODE TEMPLATE FOR CS1 LAB1 ASSIGNMENT */

/*This program demonstrates an example structure, function, and pointer to
strcture. It reads data for N number of student's id numbers and their scores
for 3 courses. The program reads those scores and output the same information
and average score. Then it finds the maximum average and the student who
obtained the max average
Sample data:
5
861022 65 72 56
851102 78 45 80
860501 55 75 90
841205 75 80 95
850630 40 50 48
*/
#include <stdio.h>
#include "main.h"
#define MAXSIZE 500

struct Student getMaxAverageStudent(struct Student *s, int n) {

  int index;
  float max = 0;
  for(int i = 0; i < n; i++){
    if(s[i].average > max){
      max = s[i].average;
      index = i;
    }
  }
  
  return s[index];
	
}

void readData(struct Student *students, int *c) {
  
	// read first line using scanf and assuming n<=MAXSIZE
  scanf("%d", c);
  //loop through number of students, 
	for (int i = 0; i < *c; i++) {
    //read the student's id

    scanf("%d %d %d %d", &students[i].student_ID, &students[i].g1, &students[i].g2, &students[i].g3);
		
    //compute the student's average, prevent integer division
    students[i].average = (students[i].g1 + students[i].g2 + students[i].g3) / 3.0;

    //print the student's values
    printf("%d %d %d %d %0.2f", students[i].student_ID, students[i].g1, students[i].g2, students[i].g3, students[i].average);
  }
}

int main(void) {
  

  int n;
	struct Student students[MAXSIZE];

  readData(students, &n);

  struct Student bestStudnet = getMaxAverageStudent(students, n);


	return 0;
}