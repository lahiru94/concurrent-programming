#include <iostream>  
#include <time.h>
#include <cstdlib> 
#include <chrono>
using namespace std;



double** generateMatrix(int n){


    double a = 0;//lower limit for random numbers
    double b = 100;//upper limit for random numbers

    double **matrix_1;
    matrix_1 = new double*[n];

    for (int i  = 0 ; i < n ; i++){
        matrix_1[i] = new double[n]; 
    }

    for (int row = 0; row < n ; row++){
        for (int col = 0; col < n ; col++){
            matrix_1[row][col] = ((double)rand() / RAND_MAX) * (b - a) + a;
            // cout << matrix_1[row][col] << (col == n-1 ? "" : "\t");
        }
        // cout << "\n";
    } 
    return matrix_1;

}

void MultiplySerial() {
  
    cout << "Enter arry size" << endl;
    int n;
    cin >> n;

    double **first = generateMatrix(n);
    
    double **product;
    product = new double*[n];

    for (int i  = 0 ; i < n ; i++){
        product[i] = new double[n]; 
    }

    double **second = generateMatrix(n);

    auto start = chrono::high_resolution_clock::now();
    // cout << "final result: \n";
    for (int row = 0; row < n; row++) {  
        for (int col = 0; col < n; col++) {   
            for (int inner = 0; inner < n; inner++) {  
                product[row][col] += first[row][inner] * second[inner][col];  
            }  
            // std::cout << product[row][col] << "  ";  
        }  
        // std::cout << "\n";  
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << ".\n";
 
}


void MultiplyParallel() {
  
    cout << "Enter arry size" << endl;
    int n;
    cin >> n;

    double **first = generateMatrix(n);
    
    double **product;
    product = new double*[n];

    for (int i  = 0 ; i < n ; i++){
        product[i] = new double[n]; 
    }

    double **second = generateMatrix(n);

    
    // cout << "final result: \n";

    auto start = chrono::high_resolution_clock::now();

    #pragma omp parallel for
    for (int row = 0; row < n; row++) {  
        for (int col = 0; col < n; col++) {   
            for (int inner = 0; inner < n; inner++) {  
                product[row][col] += first[row][inner] * second[inner][col];  
            }
        }   
    } 

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << ".\n"; 
}



void MultiplyParallelOptimized() {

    
    cout << "Enter arry size" << endl;
    int n;
    cin >> n;

    double *first = new double[n*n];
    double *second = new double[n*n];
    double *product = new double[n*n];


    auto start = chrono::high_resolution_clock::now();
    
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

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << ".\n";    
}  

int main() { 

    srandom(time(NULL));
    int type;

    cout << "Select simulation type: \n";
    cout << "   1 - Serial multiplication.\n";
    cout << "   2 - Parallel multiplication.\n";
    cout << "   3 - Optimized parallel multiplication.\n";

    cin >> type;

    if(type==1){
        MultiplySerial();
    }else if (type==2){
        MultiplyParallel();
    }else if (type==3){
        MultiplyParallelOptimized();
    }else{
        cout << "invalid input!";
    }
    
    return 0;  
}