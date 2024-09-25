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
  struct Queue* next;
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
int oneCarLeft(Queue* q); //done
void displayAllGarages(Queue* q); //done
Queue* removeQueue(Queue* q, int number);
void applyPhaseOne(Queue* q);
Queue* applyPhaseTwo(Queue* q);


int main(){
  int numGarages;
  Queue* queueLinkedList = NULL;
  scanf( "%d", &numGarages);
  for(int i = 0; i < numGarages; i++){
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    createReverseCircle(newQueue);
    addQueueToLinkedList(&queueLinkedList, newQueue);
  }

  printf("Initial status of nonempty queues");
  
  displayAllGarages(queueLinkedList);


  Queue* temp = queueLinkedList;
  while(temp != NULL){
    rearrangeCircle(temp);
    temp = temp->next;
  }
  printf("\nAfter ordering status of nonempty queues");
  displayAllGarages(queueLinkedList);


  //PHASE 1
  temp = queueLinkedList;
  printf("\nPhase1 elimination\n");
  while(temp != NULL){
    printf("\nGroup for Garage# %d\n", temp->number);
    applyPhaseOne(temp);
    temp = temp->next;
  }

  //PHASE 2
  printf("\nPhase2 elimination\n");
  while(!oneCarLeft(queueLinkedList)){
    queueLinkedList = applyPhaseTwo(queueLinkedList);
  }
  printf("\nStudent %d from the group for garage %d is the winner!", peek(queueLinkedList), queueLinkedList->number);
  free(queueLinkedList->front);
  free(queueLinkedList);
  return 0;
}


int oneCarLeft(Queue* q){

  int count = 0;
  Queue* temp = q;
  while(temp != NULL && count <= 1){
    if(!isEmpty(temp)){
      count += temp->n;
    }
    temp = temp->next;
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
  q->next = NULL;
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

void addQueueToLinkedList(Queue** head, Queue* newQueue){
  if(*head == NULL){
    *head = newQueue;
    return;
  }

  //if fisrt garage's number is higher than new garage then insert in front
  if((*head)->number > newQueue->number){
    newQueue->next = (*head);
    *head = newQueue;
    return;
  }

  Queue* temp = (*head);
  while(temp->next != NULL && temp->next->number < newQueue->number){
    temp = temp->next;
  }

  newQueue->next = temp->next;
  temp->next = newQueue;
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

Queue* applyPhaseTwo(Queue* q){
  //remove empty queues from queue linked list:::

  Queue* temp = q;
  Queue* queueWithHighestSequeneceNumber = q;
  while(temp != NULL){
    if(peek(temp) > peek(queueWithHighestSequeneceNumber)){
      queueWithHighestSequeneceNumber = temp;
    }
    temp = temp->next;
  }
  printf("Eliminated student %d from group for garage %d\n", peek(queueWithHighestSequeneceNumber), queueWithHighestSequeneceNumber->number);
  free(dequeue(queueWithHighestSequeneceNumber));


  if (isEmpty(queueWithHighestSequeneceNumber)){
    q = removeQueue(q, queueWithHighestSequeneceNumber->number);
  }
  return q;
}

void displayAllGarages(Queue* q){
  printf("\n");
  Queue* temp = q;
  while(temp != NULL){
    display(temp);
    temp = temp->next;
  }
}

Queue* removeQueue(Queue* q, int number){
  Queue* toFree;

  if(q->number == number){
    Queue* next = q->next;
    free(q);
    return next;
  }
  Queue* temp = q;
  while(temp->next != NULL && temp->next->number != number){
    temp = temp->next;
  }
  toFree = temp->next;
  temp->next = temp->next->next;
  free(toFree);
  return q;
}