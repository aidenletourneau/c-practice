/* COP 3502C Lab 5
This program is written by: Aiden Letourneau */

#include <stdio.h>
#include <string.h>
#define Max_Size 100

int checkBalance(char exp[]);
void push(int x);
void printStack();
int isEmpty();
int isFull();
char peek();
int pop();


int priority(char ch); //returns the priority of a given operator
int isOperator(char ch); //check whether it is an operator
char *infixToPostfix(char infix[]); //convert a given infix into postfixTanvir Ahmed

void emptyStack();

char Stack[Max_Size];
int top = -1;

void main(void){
  char expression[1000];
  fgets(expression, sizeof(expression), stdin);
  printf("Enter Expression: Your input expression: %s\n", expression);
  printf("Checking balance...\n");
  if (!checkBalance(expression)){
    printf("INVALID for %c!!!\n", peek());
    return;
  }
  printf("VALID\n");
  emptyStack();
  printf("The postfix is: %s", infixToPostfix(expression));
}

int checkBalance(char exp[]){

  for (int i = 0; exp[i] != '\0'; i++){
    if ((exp[i] != '{') && (exp[i] != '(') && (exp[i] != '[') && (exp[i] != '}') && (exp[i] != ')') && (exp[i] != ']')) continue;
    if((exp[i] == '(') || (exp[i] == '{') || (exp[i] == '[')) {
      push(exp[i]);
      continue;
    }
    
    if((exp[i] == ')') && peek() == '('){
      pop();
      continue;
    }
    else if((exp[i] == '}') && peek() == '{'){
      pop();
      continue;
    }
    else if((exp[i] == ']') && peek() == '[') {
      pop();
      continue;
    }

    if ((exp[i] == ')' && isEmpty()) || (exp[i] == ']' && isEmpty()) || (exp[i] == ']' && isEmpty())){
      push(exp[i]);
      return 0;
    }
  }
  return isEmpty();
}

void push(int x){
  if( top >= Max_Size-1){
  printf("Stack Overflow");
  }
  else {
    Stack[++top] = x;
  }
}

int isEmpty(){
  if (top < 0){
    return 1;
  }
  else{
    return 0;
  }
}

int pop(){
  if(top < 0){
    return -9999;
  }
  else{
    return Stack[top--];
  }
}

int isFull(){
  if (top >= Max_Size-1){
    printf("Full\n");
    return 1;

  }
  else {
    printf("not full\n");
    return 0;
  }
}

void printStack(){
  for(int i = 0; i < top+1; i++ ){
    printf("%c ", Stack[i]);
  }
  printf("\n-----------\n");
}

char peek(){
  if (!isEmpty()){
    return Stack[top];
  }
  return '_';
}

int priority(char ch){
  if (ch == '+' || ch == '-'){
    return 3;
  }
  else if(ch == '/' || ch == '*' || ch =='%'){
    return 2;
  }
  else if(ch == '^'){
    return 4;
  }
  else if(ch == '('){
    return 1000;
  }
  else{
    return 0;
  }

}

int isOperator(char c){
  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') return 1;
  return 0;
}

char* infixToPostfix(char* exp){
  char copy[1000];
  strcpy(copy, exp);
  int index = 0;
  for(int i = 0; copy[i] != '\0'; i++){
    printStack();
    printf("copyi:%c %d >= %d\n",copy[i], priority(copy[i]), priority(peek()));
    if (copy[i] == ' ') continue;
    if (isOperator(exp[i])){
      if(isEmpty()){
        push(copy[i]);
        continue;
      }
      else{
        while(priority(copy[i]) >= priority(peek())){
          exp[index] = pop();
          index++;
          if (isEmpty()){
            break;
          }
        }
        push(exp[i]);
      }
    }
    else{
      if (copy[i] == '('){
        push(copy[i]);
        continue;
      }
      else if (copy[i] == ')'){
        while(peek() != '('){
          exp[index] = pop();
          index ++;
          if (isEmpty()){
            break;
          }
        }
        pop();
        continue;
      }
      exp[index] = copy[i];
      index ++;
    }
  }
  exp[index++] = ' ';
  while(!isEmpty()){
    exp[index++] = pop();
  }
  exp[index] = '\0';
  return exp;
}

void emptyStack(){
  while(!isEmpty()){
    pop();
  }
  return;
}