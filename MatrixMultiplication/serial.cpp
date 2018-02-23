#include <iostream>  
#include <time.h>
#include <cstdlib> 
using namespace std;

void MultiplySerial() {

    double a = 0;//lower limit
    double b = 20;//upper limit
      
    srandom(time(NULL));
    int aMatrix[3][2] = {{1, 4}, {2, 5}, {3, 6}};  
    int bMatrix[2][3] = {{7, 8, 9}, {10, 11, 12}};  
    
    cout << "Enter arry size" << endl;
    int n;
    cin >> n;

    double *first = new double[n*n];
    double *second = new double[n*n];
    double *product = new double[n*n];
    // ary[i][j] is then rewritten as
    // ary[i*n+j]

    //creating arrays
    // int** first = new int*[n];
    // for(int i = 0; i < n; ++i)
    //     first[i] = new int[n];

    // int** second = new int*[n];
    // for(int i = 0; i < n; ++i)
    //     second[i] = new int[n];


    //populating arrays with random double values
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            first[i*n+j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
    }

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            second[i*n+j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
    }
    

    for (int row = 0; row < n; row++) {  
        for (int col = 0; col < n; col++) {  
            // Multiply the row of A by the column of B to get the row, column of product.  
            for (int inner = 0; inner < n; inner++) {  
                product[row*n+col] += first[row*n+inner] * second[inner*n+col];  
            }  
            // std::cout << product[row*n+col] << "  ";  
        }  
        // std::cout << "\n";  
    }  
} 

void MultiplyParallel() {

    double a = 0;//lower limit
    double b = 20;//upper limit
      
    srandom(time(NULL));
    int aMatrix[3][2] = {{1, 4}, {2, 5}, {3, 6}};  
    int bMatrix[2][3] = {{7, 8, 9}, {10, 11, 12}};  
    
    cout << "Enter arry size" << endl;
    int n;
    cin >> n;

    double *first = new double[n*n];
    double *second = new double[n*n];
    double *product = new double[n*n];
    // ary[i][j] is then rewritten as
    // ary[i*n+j]

    //creating arrays
    // int** first = new int*[n];
    // for(int i = 0; i < n; ++i)
    //     first[i] = new int[n];

    // int** second = new int*[n];
    // for(int i = 0; i < n; ++i)
    //     second[i] = new int[n];


    //populating arrays with random double values
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            first[i*n+j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
    }

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            second[i*n+j] = ((double)rand() / RAND_MAX) * (b - a) + a;
        }
    }
    
    #pragma omp parallel for num_threads(4) collapse(2)
    for (int row = 0; row < n; row++) { 
        for (int col = 0; col < n; col++) {  
            // Multiply the row of A by the column of B to get the row, column of product.  
            for (int inner = 0; inner < n; inner++) {  
                product[row*n+col] += first[row*n+inner] * second[inner*n+col];  
            }  
            // std::cout << product[row*n+col] << "  ";  
        }  
        // std::cout << "\n";  
    }  
}  

int main() { 

    time_t begin,end; // time_t is a datatype to store time values.

    time (&begin); // note time before execution
    MultiplySerial();
    // MultiplyParallel();
    time (&end); // note time after execution

    double difference = difftime (end,begin);
    cout << "time taken in seconds: "<< difference <<"\n" ; 
    
    return 0;  
}