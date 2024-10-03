#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int** permutations;
int idx = 0;
int numberOfPermutations(int n);
void getPermutations(int* perm, int* used, int k, int n);


void getPermutations(int* perm, int* used, int k, int n) {

  if (k == n){
    permutations[idx] = (int*)malloc(sizeof(int)*n);
    memcpy(permutations[idx++], perm, n * sizeof(int));
    return;
  }
  for (int i = 0; i<n; i++) {
    if (k == 0 && i != 0){
      continue;
    }
    if (!used[i]) { 
      used[i] = 1;
      perm[k] = i;
      getPermutations(perm, used, k+1, n);
      used[i] = 0;
    }
  }
}


int main(){
  permutations = (int**)malloc(sizeof(int*)*numberOfPermutations(6));
  int perm[6] = {0};
  int used[6] = {0};

  getPermutations(perm, used, 0, 4);
  for (int i = 0; i < numberOfPermutations(4); i++){
    for (int j = 0; j < 4; j++){
      printf("%d ", permutations[i][j]);
    }
    printf("\n");
  }



  return 0;
}


int numberOfPermutations(int n){
  return (int)tgamma(n+1);
}


