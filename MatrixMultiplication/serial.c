#include <stdio.h>
#include <time.h>
#include <stdlib.h>
 
int main()
{
  int  n, c, d, k; 
  double sum = 0;
  srand(time(NULL));   // should only be called once

 
  printf("Enter required size of matrices:\n");
  scanf("%d", &n);
  
 
  //generating first matrix
  double first[n][n];
  for (c = 0; c < n; c++){
    for (d = 0; d < n; d++){
      double curr_val = rand();
      first[c][d] = curr_val;
    } 
  }
  //generating second matrix
  double second[n][n];
  for (c = 0; c < n; c++){
    for (d = 0; d < n; d++){
      second[c][d] = rand();
    }
  }

  double multiply[n][n];//resulting matrix
  for (c = 0; c < n; c++) {
    for (d = 0; d < n; d++) {
      for (k = 0; k < n; k++) {
        sum = sum + first[c][k]*second[k][d];
      }

      multiply[c][d] = sum;
      sum = 0;
    }
  }

  printf("Product of the matrices:\n");

  for (c = 0; c < n; c++) {
    for (d = 0; d < n; d++)
      printf("%f\t", multiply[c][d]);

    printf("\n");
  }
  
 
  return 0;
}