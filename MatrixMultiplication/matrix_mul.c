#include <stdio.h>
#include <time.h>
#include <stdlib.h>
 
int main()
{
  int m, n, p, q, c, d, k, sum = 0;
  srand(time(NULL));   // should only be called once

 
  printf("Enter number of rows and columns of first matrix\n");
  scanf("%d%d", &m, &n);
  int first[m][n];
  printf("Enter elements of first matrix\n");
 
  for (c = 0; c < m; c++)
    for (d = 0; d < n; d++)
      first[c][d] = rand(); 
 
  printf("Enter number of rows and columns of second matrix\n");
  scanf("%d%d", &p, &q);
  int second[p][q];
  int multiply[10][10];

  if (n != p)
    printf("The matrices can't be multiplied with each other.\n");
  else
  {
    printf("Enter elements of second matrix\n");
 
    for (c = 0; c < p; c++)
      for (d = 0; d < q; d++)
        second[c][d] = rand();
 
    for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++) {
        for (k = 0; k < p; k++) {
          sum = sum + first[c][k]*second[k][d];
        }
 
        multiply[c][d] = sum;
        sum = 0;
      }
    }
 
    printf("Product of the matrices:\n");
 
    for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++)
        printf("%d\t", multiply[c][d]);
 
      printf("\n");
    }
  }
 
  return 0;
}