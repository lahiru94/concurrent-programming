
This program requires open MP thread library and c++ 11 which has to be specified
in the compile time.
Compile with -fopenmp to enable OpenMP and -std=c++11 to specify the verision.
Complete command to compile this(on linux) is as follows(script.cpp is the name of the file):
    g++ script.cpp -fopenmp -std=c++11 


Steps to execute the program:
    1. Compile the script.cpp as instructed above.
    2. Run the a.out file produced.
    3. Follow the on screen instructions to select the multiplication method.

After this program will display the mean running times calculated accordingly along with the matrix size. 