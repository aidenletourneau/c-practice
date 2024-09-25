/* COP 3502C Assignment 2
This program is written by: Aiden Letourneau */
#include <stdio.h>
#include <stdlib.h>
typedef struct Student{
  int sequenceNumber;
  struct Student* next;
} Student;

typedef struct Queue{
  Student* front;
  Student* back;
  int n;
  int k;
  int th;
  int number;
} Queue;

void enqueue(Queue* queueLinkedList, Student* student); //done
Student* dequeue(Queue* queueLinkedList); //done 
int peek(Queue* q); //done
int isEmpty(Queue* queueLinkedList); //done

Student* createStudent(int sequence); //done
void createReverseCircle(Queue* q); //done
void rearrangeCircle(Queue* q);
void display(Queue *q); //done


void addQueueToLinkedList(Queue** head, Queue*); //done
int oneCarLeft(Queue** arrayOfQueues); //done
void displayAllGarages(Queue** arrayOfQueues); //done
void applyPhaseOne(Queue* q);
void applyPhaseTwo(Queue** arrayOfQueues);


int main(){
  Queue** arrayOfQueues = (Queue**)malloc(sizeof(Queue*)*10);
  for (int i = 0; i < 10; i ++){
    arrayOfQueues[i] = NULL;
  }


  int numGarages;
  scanf("%d", &numGarages);
  for(int i = 0; i < numGarages; i++){
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    createReverseCircle(newQueue);
    arrayOfQueues[newQueue->number-1] = newQueue;

  }

  printf("Initial status of nonempty queues");
  displayAllGarages(arrayOfQueues);


  for(int i = 0; i < 10; i++){
    if (arrayOfQueues[i] == NULL) continue;
    rearrangeCircle(arrayOfQueues[i]);
  }

  printf("\nAfter ordering status of nonempty queues");
  displayAllGarages(arrayOfQueues);


  // PHASE 1
  printf("\nPhase1 elimination\n");
  for(int i = 0; i < 10; i ++){
    if(arrayOfQueues[i] == NULL) continue;
    printf("\nGroup for Garage# %d\n", arrayOfQueues[i]->number);
    applyPhaseOne(arrayOfQueues[i]);
  }

  // PHASE 2
  printf("\nPhase2 elimination\n");
  while(!oneCarLeft(arrayOfQueues)){
    applyPhaseTwo(arrayOfQueues);
  }

  for (int i = 0; i < 10; i++){
    if (arrayOfQueues[i] == NULL) continue;
    printf("\nStudent %d from the group for garage %d is the winner!", peek(arrayOfQueues[i]), arrayOfQueues[i]->number);
    free(arrayOfQueues[i]->front);
    free(arrayOfQueues[i]);
    free(arrayOfQueues);
    break;
  }
  return 0;
}


int oneCarLeft(Queue** arrayOfQueues){

  int count = 0;
  for(int i = 0; i < 10; i++){
    if (arrayOfQueues[i] == NULL) continue;
    count += arrayOfQueues[i]->n;
  }
  if (count != 1){
    return 0;
  }
  return 1;
}

Student* createStudent(int sequence){
  Student* newStudent = (Student*)malloc(sizeof(Student));
  newStudent->sequenceNumber = sequence;
  newStudent->next = NULL;
  return newStudent;
}

void createReverseCircle(Queue* q){
  int garageNumber, studentsInterested, k, th;
  scanf( "%d %d %d %d", &garageNumber, &studentsInterested, &k, &th);
  q->front = NULL;
  q->back = NULL;
  q->n = studentsInterested;
  q->k = k;
  q->number = garageNumber;
  q->th = th;
  for(int i = 0; i < studentsInterested; i++){
    enqueue(q, createStudent((studentsInterested)-i));
  }
}

void enqueue(Queue* q, Student* student){
  if (isEmpty(q)){
    q->front = student;
  }
  else {
    q->back->next = student;
  }
  q->back = student;
  student->next = q->front;
}

Student* dequeue(Queue* q){
  Student* temp = q->front;
  if (q->front == q->back){
    q->front = NULL;
    q->back = NULL;
    return temp;
  }
  else{
    q->front = q->front->next;
    q->back->next = q->front;
    q->n --;
    return temp;
  }
}

void display(Queue* q){
  int n = 0;
  Student* temp = q->front;

  printf("%d", q->number);
  while(temp != q->back && n < 20){
    printf(" %d", temp->sequenceNumber);
    temp = temp->next;    
    n++;
  }
  printf(" %d\n", temp->sequenceNumber);
}

int isEmpty(Queue* q){
  return (q->front == NULL && q->back == NULL);
}

void rearrangeCircle(Queue *q){
  // initialize variables to keep track of previous, current, and next nodes positions for reversal of circular singly-linked list
  Student* curr = q->front;
  Student* prev = q->back;
  Student* next = curr->next;

  Student* temp = q->back;
  q->back = q->front;
  q->front = temp;

  while(1){
    curr->next = prev;
    prev = curr;
    if(prev == q->front){
      return;
    }
    curr = next;
    next = curr->next;
  }
}

int peek(Queue* q){
  return q->front->sequenceNumber;
}

void applyPhaseOne(Queue* q){
  Student* temp = q->front;
  while(q->n > q->th){    
    
    for(int i = 0; i < q->k-2; i ++){
      temp = temp->next;
    }
    
    printf("Student# %d eliminated\n", temp->next->sequenceNumber);
    //if the student we are eliminating is the end if the queue, 
    if(temp->next == q->back){
      q->back = temp;
    }
    if(temp->next == q->front){
      q->front = temp->next->next;
    }

    //temp is a pointer to a student struct, temp->next is a pointer to the next student struct 
    Student* toFree = temp->next;
    temp->next = temp->next->next;
    free(toFree);
    temp = temp->next;  // Now temp points to the correct element.


    q->n --;
  }

}

void applyPhaseTwo(Queue** arrayOfQueues){
  //remove empty queues from queue linked list:::

  int indexOfHighestQueue;
  for(int i = 0; i < 10; i++){
    if (arrayOfQueues[i] != NULL){
      indexOfHighestQueue = i;
      break;
    }
  }

  for(int i = 0; i < 10; i++){
    if (arrayOfQueues[i] == NULL) continue;
    
    if (peek(arrayOfQueues[i]) > peek(arrayOfQueues[indexOfHighestQueue])){
      indexOfHighestQueue = i;
    }
  }


  printf("Eliminated student %d from group for garage %d\n", peek(arrayOfQueues[indexOfHighestQueue]), arrayOfQueues[indexOfHighestQueue]->number);
  free(dequeue(arrayOfQueues[indexOfHighestQueue]));


  if (isEmpty(arrayOfQueues[indexOfHighestQueue])){
    free(arrayOfQueues[indexOfHighestQueue]);
    arrayOfQueues[indexOfHighestQueue] = NULL;
  }
}

void displayAllGarages(Queue** arrayOfQueues){
  printf("\n");
  for(int i = 0; i < 10; i ++){
    if (arrayOfQueues[i] == NULL) continue;
    display(arrayOfQueues[i]);
  }
}
