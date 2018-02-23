#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "timer.h"


double** Make2DDoubleArray(int arraySizeX, int arraySizeY) {
  double** theArray;
  theArray = (double**) malloc(arraySizeX*sizeof(double*));
  for (int i = 0; i < arraySizeX; i++)
    theArray[i] = (double*) malloc(arraySizeY*sizeof(double));
    return theArray;
} 

int main()
{
  int  n, c, d, k; 
  double sum = 0;

  //timing variables
  double start;
  double finish;

  srand(time(NULL));   // should only be called once

 
  printf("Enter required size of matrices:\n");
  scanf("%d", &n);
  
 
  //generating first matrix
  double** first = Make2DDoubleArray(n,n);
  for (c = 0; c < n; c++){
    for (d = 0; d < n; d++){
      double curr_val = rand();
      first[c][d] = curr_val;
    } 
  }
  //generating second matrix
  double** second = Make2DDoubleArray(n,n);
  for (c = 0; c < n; c++){
    for (d = 0; d < n; d++){
      second[c][d] = rand();
    }
  }

  double multiply[n][n];//resulting matrix

  GET_TIME(start);//execution starts here
  for (c = 0; c < n; c++) {
    for (d = 0; d < n; d++) {
      for (k = 0; k < n; k++) {
        sum = sum + first[c][k]*second[k][d];
      }

      multiply[c][d] = sum;
      sum = 0;
    }
  }
  GET_TIME(finish);//execution ends here

  //caculating elapsed time
  double time_taken = finish-start;

  printf("Time taken: %f seconds\n",time_taken);

  // for (c = 0; c < n; c++) {
  //   for (d = 0; d < n; d++)
  //     printf("%f\t", multiply[c][d]);

  //   printf("\n");
  // }
  
 
  return 0;
}