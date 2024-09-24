/* COP 3502C Lab 4
This program is written by: Aiden Letourneau */


#include <stdio.h>
#include <stdlib.h>
#include "main.h"


void print_list(node* head);


void main(void) {
  node* root = NULL;
  int command;
  printf("\n");
  while(1){
    printf("Menu: 1.insert at front, 2. reverse list 3. Insert to place 0. exit: ");
    scanf("%d", &command);
    if (command == 1){
      int val;
      printf("Enter data (an integer): ");
      scanf("%d", &val);
      root = insert_front(root, val);
    }
    else if (command == 2){
      root = reverse(root);
      printf("list reversed.\n");
    }
    else if (command == 3){
      int val, place;
      printf("Enter data (an integer) and place (>1) separated by space: ");
      scanf("%d %d", &val, &place);
      insertToPlace(root, val, place);
    }
    else if (command == 0){
      break;
    }
    print_list(root);
  }
printf("GOOD BYE>>>>\n");
}


node* insert_front(node *root, int item){
  node* newNode = (node*)malloc(sizeof(node));
  if (root == NULL){
    newNode->next = NULL;
  }
  else {
    newNode->next = root;
  }
  newNode->data = item;
  return newNode;
}

void print_list(node* head){
  node* curr = head;
  printf("Printing your linked list.......");
  while(curr != NULL){
    printf(" %d", curr->data);
    curr = curr->next;
  }
  printf("\n");
}

node* reverse(node* head){
  if (head == NULL){
    return head;
  }
  node* prev = NULL;
  node* curr = head;
  node* next;
  while(curr->next != NULL){
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }
  curr->next = prev;
  return curr;
}

void insertToPlace(node* list, int val, int place){
  int index = 1;
  node* temp = list;
  while(index < place-1 && temp->next != NULL){
    temp = temp->next;
    index ++;
  }
  node* newNode = (node*)malloc(sizeof(node));
  newNode->data = val;
  newNode->next = temp->next;
  temp->next = newNode;
}