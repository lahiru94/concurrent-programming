#include <iostream>  
#include <time.h>
#include <cstdlib> 
#include <chrono>
#include<vector>
#include<cmath>
#include<numeric>
using namespace std;

double **product; //final reslut 


// generate random number(type double) matrix of a given type
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
        }
    } 
    return matrix_1;
}


double getStandardDiviation(vector<double> *vals, double mean){
    double std;
    std = 0.0;
    for(int i=0;i<vals->size();i++){
        std += pow((*vals)[i]-mean, 2);
    }
    std = sqrt(std/(vals->size()-1));
    return std;
}

double calculateSampleCount(int pilot, vector<double> *vals){
    double z = 1.96;
    int r = 5;
    double x_bar = accumulate(vals->begin(), vals->end(), 0.0) / vals->size();
    double s = getStandardDiviation(vals, x_bar);
    return max(pilot, (int) pow( (100*z*s)/(5*x_bar), 2));
}


// serial execution
double MultiplySerial(int n) {

    double **first = generateMatrix(n);
    double **second = generateMatrix(n);

    auto start = chrono::high_resolution_clock::now();
    for (int row = 0; row < n; row++) {  
        for (int col = 0; col < n; col++) {   
            for (int inner = 0; inner < n; inner++) {  
                product[row][col] += first[row][inner] * second[inner][col];  
            }   
        }  
    }
    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start;
    delete[] first;
    delete[] second;
    return elapsed.count();
}

// parallel execution
double MultiplyParallel(int n) {

    double **first = generateMatrix(n);
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
    
    delete[] first;
    delete[] second;
    return elapsed.count(); 
}

//optimized parallel execution
double MultiplyParallelOptimized(int n) {
  
    double **first = generateMatrix(n);
    double **second = generateMatrix(n);

    auto start = chrono::high_resolution_clock::now();
    #pragma omp parallel for 
    for(int i=0;i<n;i++){
        double* first_block = first[i]; // cache a block from first matrix
        double* product_block = product[i];
        for(int k=0;k<n;k++){
            double* second_block = second[k]; // cache a block from second matrix
            double first_value = first_block[k];
            for(int j=0;j<n;j++){
                product_block[j] += first_value + second_block[j];//final sum will be updated multiple times
            }
        }
    } 
    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start;
    delete[] first;
    delete[] second;
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
    if(required>iterations && required<200){ //sometimes due to anomalies requires value goes very high. To avoid program getting stuck in loop this logic is used 
        runBenchmarking(type,required,size); //if it doesnt run the required iterations you can simply restart the program.
    }else {
        cout<<"array size: "<< size << ", runs: " << iterations << ", required runs: "<< required << ", mean: " << accumulate(values.begin(), values.end(), 0.0) / iterations << endl;;
    }
}

int main() { 

    int type;
    cout << "Select simulation type: \n";
    cout << "Enter 1 for serial multiplication.\n";
    cout << "Enter 2 for parallel multiplication.\n";
    cout << "Enter 3 for optimized parallel multiplication.\n";
    cin >> type;
    cout << "Starting execution \n";
    for(int i=200;i<2001;i+=200){
        product = new double*[i];
        for (int j  = 0 ; j < i ; j++){
            product[j] = new double[i]; 
        }
        runBenchmarking(type,10,i);
        delete[] product;
    }

    return 0;  
}