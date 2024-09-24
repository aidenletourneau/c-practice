#include <stdio.h>

void main(void){
  int a = 89;
  int* ptr = &a;
  printf("%p\n", ptr);


  printf("Size of int: %lu\n", sizeof(int));
  printf("Size of char: %lu\n", sizeof(char));
  printf("Size of double: %lu\n", sizeof(double));
  printf("Size of int pointer: %lu\n", sizeof(int*));

  *ptr = 10;
  printf("%d", a);
}