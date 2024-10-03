/* COP 3502C Assignment 3
This program is written by: Aiden Letourneau */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Garage{
  int x;
  int y;
  char* name;
} Garage;

void printGarage(Garage* g);
void printAllGarages(int numGarages);
Garage* createGarage(int x, int y, char* name);
float calculateDistance(Garage* g1, Garage* g2);
int numberOfPermutations(int n);
void getPermutations(int* perm, int* used, int k, int n);
void printPermutation(int* permutation, int n);
float calculatePermutationDistance(int* permutation, int numGarages);

int* bestPermutation;
Garage** garages;

int main(){
  int numGarages = 0;
  scanf("%d", &numGarages);
  numGarages *= 2;
  garages = (Garage**)malloc(sizeof(Garage*)*numGarages);
  for (int i = 0; i < numGarages; i++){
    int x, y;
    char name[21];
    scanf("%d %d %s", &x, &y, name);
    garages[i] = createGarage(x, y, name);
  }

  //let the starting best permutation be the first
  bestPermutation = (int*)malloc(sizeof(int)*numGarages);
  for(int i =0; i< numGarages; i++){
    bestPermutation[i] = i;
  }

  int perm[16] = {0};
  int used[16] = {0};

  getPermutations(perm, used, 0, numGarages);


  //display results
  printf("%.3f\n", calculatePermutationDistance(bestPermutation, numGarages));
  for (int i = 0; i < numGarages; i+= 2){
    printf("(%s, %s, %.3f)\n", 
      garages[bestPermutation[i]]->name, 
      garages[bestPermutation[i+1]]->name, 
      calculateDistance(garages[bestPermutation[i]], garages[bestPermutation[i+1]]));
  }

  return 0;
}


Garage* createGarage(int x, int y, char* name){
  Garage* newGarage = (Garage*)malloc(sizeof(Garage));
  newGarage->x = x;
  newGarage->y = y;
  newGarage->name = (char*)malloc(sizeof(char)*21);
  strcpy(newGarage->name, name);
  return newGarage;
}

void printGarage(Garage* g){
  printf("X: %d, Y: %d, Garage Name: %s\n", g->x, g->y, g->name);
}

void printAllGarages(int numGarages){
  for(int i = 0; i < numGarages; i++){
    printGarage(garages[i]);
  }
}

float calculateDistance(Garage* g1, Garage* g2){
  return sqrt(pow((float)(g1->x - g2->x), 2) + pow((float)(g1->y - g2->y), 2));
}

void getPermutations(int* perm, int* used, int k, int n) {
  if (k == n){ 
    if(calculatePermutationDistance(perm, n) < calculatePermutationDistance(bestPermutation, n)){
      memcpy(bestPermutation, perm, sizeof(int)*n);
    }
    return;
  }
  for (int i = 0; i<n; i++) {
    if (!used[i]) { 
      used[i] = 1;
      perm[k] = i;
      getPermutations(perm, used, k+1, n);
      used[i] = 0;
    }
  }
}

int numberOfPermutations(int n){
  return (int)tgamma(n+1);
}

void printPermutation(int* permutation, int n){

  for (int i = 0; i < n; i++){
    printf("%d ", permutation[i]);
  }
  printf("\n");
}

float calculatePermutationDistance(int* permutation, int numGarages){
  float permutationDistance = 0;
  for (int j = 0; j < numGarages; j += 2){
    permutationDistance += calculateDistance(garages[permutation[j]], garages[permutation[j+1]]);
  }
  return permutationDistance;
}