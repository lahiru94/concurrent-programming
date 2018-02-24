#include <iostream>  
#include <time.h>
#include <cstdlib> 
#include <chrono>
#include<vector>
#include<cmath>
#include<numeric>
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

double getStandardDiviation(vector<double> *vals, double mean){
    double stdev;

    stdev = 0.0;
    for(int i=0;i<vals->size();i++){
        stdev += pow((*vals)[i]-mean, 2);
    }

    stdev = sqrt(stdev/(vals->size()-1));

    return stdev;
}

double calculateSampleCount(int pilot, vector<double> *vals){
    double z = 1.96;
    int r = 5;

    double x_bar = accumulate(vals->begin(), vals->end(), 0.0) / vals->size();
    double s = getStandardDiviation(vals, x_bar);

    return max(pilot, (int) pow( (100*z*s)/(5*x_bar), 2));
}



double MultiplySerial(int n) {
  

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
    return elapsed.count();
 
}


double MultiplyParallel(int n) {
  

    double **first = generateMatrix(n);
    
    double **product;
    product = new double*[n];

    for (int i  = 0 ; i < n ; i++){
        product[i] = new double[n]; 
    }

    double **second = generateMatrix(n);

    

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
    return elapsed.count(); 
}



double MultiplyParallelOptimized(int n) {

    double a = 0;//lower limit
    double b = 20;//upper limit

    double *first = new double[n*n];
    double *second = new double[n*n];
    double *product = new double[n*n];

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

    auto start = chrono::high_resolution_clock::now();
    
    #pragma omp parallel for num_threads(4) collapse(2)
    for (int row = 0; row < n; row++) { 
        for (int col = 0; col < n; col++) {  
            // Multiply the row of A by the column of B to get the row, column of product.  
            for (int inner = 0; inner < n; inner++) {  
                product[row*n+col] += first[row*n+inner] * second[inner*n+col];  
            }   
        }   
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start;    
    return elapsed.count();
}  

void runBenchmarking(int type,int iterations,int size){
    srand (time(NULL));
    vector<double> values;
    double elapsed;
    for(int i=0;i<iterations;i++){
        if(type==1){
            elapsed = MultiplySerial(size);
        }else if (type==2){
            elapsed = MultiplyParallel(size);
        }else {
            elapsed = MultiplyParallelOptimized(size);
        }
        values.push_back(elapsed);
    }   
    int required = calculateSampleCount(iterations, &values);  
    if(required>iterations){
        runBenchmarking(type,required,size);
    }else{
        cout<<"array size: "<< size << " runs: " << iterations << " required runs: "<< required << " mean: " << accumulate(values.begin(), values.end(), 0.0) / iterations << endl;;
    }
}

int main() { 

    int type;

    cout << "Select simulation type: \n";
    cout << "   1 - Serial multiplication.\n";
    cout << "   2 - Parallel multiplication.\n";
    cout << "   3 - Optimized parallel multiplication.\n";

    for(int i=200;i<2001;i+=200){
        runBenchmarking(type,10,i);
    }
    
    return 0;  
}