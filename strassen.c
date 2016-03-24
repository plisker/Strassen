//T(n) = 7T(n/2) + Θ(n^2) --> T(n) = Θ(n^(log7)) = Θ(n^(2.807))

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

void strassen(int, int**, int**, int, int, int, int, int**);

// Allocates memory for a square matrix of size d x d using double pointers and sets to all 0s...
int** allocateMatrix_set_zero(int d) {
    int** matrix;
    matrix = (int**) malloc(d*sizeof(int*)); //allocate space for d rows of ints
    for (int i = 0; i < d; i++){
        matrix[i] = (int*) malloc(d*sizeof(int)); //allocate enough space for d columns of ints
        
#warning Probaby inefficient and unnecessary for some malloc'ed matrices. Split into two functions?
        for (int j=0;j<d;j++){
            matrix[i][j]=0; //solves padding 0 problem for non-powers of 2
        }
    }
    return matrix;
}

// Allocates memory for a square matrix of size d x d using double pointers...
int** allocateMatrix(int d) {
    int** matrix;
    matrix = (int**) malloc(d*sizeof(int*)); //allocate space for d rows of ints
    for (int i = 0; i < d; i++){
        matrix[i] = (int*) malloc(d*sizeof(int)); //allocate enough space for d columns of ints
    }
    return matrix;
}

// Frees square matrix of size d x d
void freeMatrix(int** m, int d){
    for (int i = 0; i < d; i++)
        free(m[i]);
    free(m);
    return;
}

int** padMatrix(int** m, int d, int d_n){
    int** m2 = allocateMatrix_set_zero(d_n);
//    realloc(m, sizeof(m2));
//    for (int i=0;i<d_n;i++){
//        for (int j=0;j<d_n;j++){
//            m[i][j]=0;
//        }
//    }
    return m;
}

// Checks if an integer is a power of 2
int isPowerofTwo(int n){
    while(((n%2)==0) && n>1)
        n/=2;
    return (n==1);
}

// Returns next power of two (greater than or equal to d)
int nextPowofTwo(int d){
    double n = ceil(log2(((double) d)));
    int x = (int) pow((double) 2, n);
    return x;
}

// Standard matrix multiplication
#warning Optimize (or confirm that it is) by re-ordering  loops to take advantage of caching... Same with other helper functions.
void standard_mult(int d, int** a, int** b, int** answer){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
#warning Is this necessary? Will the answer matrix already be 0?
            answer[i][j] = 0;
            for (int k=0; k<d; k++){
                answer[i][j] += a[i][k]*b[k][j];
            }
        }
    }
}

// Strassen add, tracking indexes
// RS = row start
// CS = column start
#warning Optimize (or confirm that it is) by re-ordering  loops to take advantage of caching... Same with other helper functions.
void s_standard_mult(int d, int** a, int** b, int a_RS, int a_CS, int b_RS, int b_CS, int** answer){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
#warning Is this necessary? Will the answer matrix already be 0?
            answer[i][j] = 0;
            for (int k=0; k<d; k++){
                answer[i][j] += a[i+a_RS][k+a_CS]*b[k+b_RS][j+b_CS];
            }
        }
    }
}

// Prints the matrix
void display_mat(int d, int** matrix){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            printf("%d\t ",matrix[i][j]);
            if(j==d-1){
                printf("\n");
            }
        }
    }
}

void display_diagonal(int d, int** matrix){
    for (int i=0; i<d; i++){
        printf("%d\n", matrix[i][i]);
    }
}

// Strassen add, tracking indexes
// RS = row start
// CS = column start
void s_matrix_add(int d, int** a, int** b, int a_RS, int a_CS, int b_RS, int b_CS, int** answer, int c_RS, int c_CS){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            answer[i+c_RS][j+c_CS] = a[i+a_RS][j+a_CS]+b[i+b_RS][j+b_CS];
        }
    }
}

void s_matrix_add_to(int d, int** a, int a_RS, int a_CS, int** answer, int c_RS, int c_CS){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            answer[i+c_RS][j+c_CS] += a[i+a_RS][j+a_CS];
        }
    }
}

// Full add, no indexes tracked
void matrix_add(int d, int** a, int** b, int** answer){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            answer[i][j] = a[i][j]+b[i][j];
        }
    }
}

// Strassen subtract, tracking indexes
// RS = row start
// CS = column start
void s_matrix_subtract(int d, int** a, int** b, int a_RS, int a_CS, int b_RS, int b_CS, int** answer, int c_RS, int c_CS){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            answer[i+c_RS][j+c_CS] = a[i+a_RS][j+a_CS]-b[i+b_RS][j+b_CS];
        }
    }
}

void s_matrix_subtract_to(int d, int** a, int a_RS, int a_CS, int** answer, int c_RS, int c_CS){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            answer[i+c_RS][j+c_CS] -= a[i+a_RS][j+a_CS];
        }
    }
}

void initialize_values(int d, int** a, int a_RS, int a_CS, int** answer, int c_RS, int c_CS){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            answer[i+c_RS][j+c_CS] = a[i+a_RS][j+a_CS];
        }
    }
}

// Full subtract, no indexes tracked
void matrix_subtract(int d, int** a, int** b, int** answer){
    for (int i=0; i<d; i++){
        for (int j=0; j<d; j++){
            answer[i][j] = a[i][j]-b[i][j];
        }
    }
}


// RS = row start
// CS = column start

void strassen(int d, int** matrix1, int** matrix2, int a_RS, int a_CS, int b_RS, int b_CS, int** answer){
#warning Eventually this should be "if d < CROSSOVER"
    if(d<=64){
        s_standard_mult(d, matrix1, matrix2, a_RS, a_CS, b_RS, b_CS, answer);
    }
    else{
        int new_d = d/2;
        
        int** one = allocateMatrix_set_zero(new_d);
        int** two = allocateMatrix_set_zero(new_d);
        int** three = allocateMatrix_set_zero(new_d);
        
        /*************/
        
        /* Calculate P_4 */
        // G - E
        s_matrix_subtract(new_d, matrix2, matrix2, b_RS+new_d, b_CS, b_RS, b_CS, one, 0, 0);
        
        // P_4
        strassen(new_d, matrix1, one, a_RS+new_d, a_CS+new_d, 0, 0, two);
        
        /* AE + BG ... top left */
        initialize_values(new_d, two, 0, 0, answer, 0, 0);
        
        /* CE + DG ... bottom left */
        initialize_values(new_d, two, 0, 0, answer, 0+new_d, 0);
        
        /*************/
        
        /* Calculate P_2 */
        // A + B
        s_matrix_add(new_d, matrix1, matrix1, a_RS, a_CS, a_RS, a_CS+new_d, one, 0, 0);
        
        // P_2
        strassen(new_d, one, matrix2, 0, 0, b_RS+new_d, b_CS+new_d, two);
        
        /* AE + BG ... top left */
        s_matrix_subtract_to(new_d, two, 0, 0, answer, 0, 0);
        
        /* AF + BH ... top right */
        initialize_values(new_d, two, 0, 0, answer, 0, 0+new_d);
        
        
        /*************/
        
        /* Calculate P_5 */
        // A + D
        s_matrix_add(new_d, matrix1, matrix1, a_RS, a_CS, a_RS+new_d, a_CS+new_d, one, 0, 0);
        
        // E + H
        s_matrix_add(new_d, matrix2, matrix2, b_RS, b_CS, b_RS+new_d, b_CS+new_d, two, 0, 0);
        
        // P_5
        strassen(new_d, one, two, 0, 0, 0, 0, three);
        
        /* AE + BG ... top left */
        s_matrix_add_to(new_d, three, 0, 0, answer, 0, 0);
        
        /* CF + DH ... bottom right */
        initialize_values(new_d, three, 0, 0, answer, 0+new_d, 0+new_d);
        
        /*************/
        
        /* Calculate P_6 */
        // B - D
        s_matrix_subtract(new_d, matrix1, matrix1, a_RS, a_CS+new_d, a_RS+new_d, a_CS+new_d, one, 0, 0);
        
        // G + H
        s_matrix_add(new_d, matrix2, matrix2, b_RS+new_d, b_CS, b_RS+new_d, b_CS+new_d, two, 0, 0);
        
        // P_6
        strassen(new_d, one, two, 0, 0, 0, 0, three);
        
        /* AE + BG ... top left */
        s_matrix_add_to(new_d, three, 0, 0, answer, 0, 0);
        
        /*************/
        
        /* Calculate P_7 */
        // A - C
        s_matrix_subtract(new_d, matrix1, matrix1, a_RS, a_CS, a_RS+new_d, a_CS, one, 0, 0);
        
        // E + F
        s_matrix_add(new_d, matrix2, matrix2, b_RS, b_CS, b_RS, b_CS+new_d, two, 0, 0);
        
        // P_7
        strassen(new_d, one, two, 0, 0, 0, 0, three);
        
        /* CF + DH ... bottom right */
        s_matrix_subtract_to(new_d, three, 0, 0, answer, 0+new_d, 0+new_d);
        
        /*************/
        
        /* Calculate P_3 */
        // C + D
        s_matrix_add(new_d, matrix1, matrix1, a_RS+new_d, a_CS, a_RS+new_d, a_CS+new_d, one, 0,0);
        
        // P_3
        strassen(new_d, one, matrix2, 0, 0, b_RS, b_CS, two);
        
        /* CF + DH ... bottom right */
        s_matrix_subtract_to(new_d, two, 0, 0, answer, 0+new_d, 0+new_d);
        
        /* CE + DG ... bottom left */
        s_matrix_add_to(new_d, two, 0, 0, answer, 0+new_d, 0);
        
        /*************/
        
        /* Calculate P_1 */
        // F - H
        s_matrix_subtract(new_d, matrix2, matrix2, b_RS, b_CS+new_d, b_RS+new_d, b_CS+new_d, one, 0, 0);
        
        // P_1
        strassen(new_d, matrix1, one, a_RS, a_CS, 0, 0, two);
        
        /* AF + BH ... top right */
        s_matrix_add_to(new_d, two, 0, 0, answer, 0, 0+new_d);
        
        /* CF + DH ... bottom right */
        s_matrix_add_to(new_d, two, 0, 0, answer, 0+new_d, 0+new_d);
        
        /*************/
        
        freeMatrix(one, new_d);
        freeMatrix(two, new_d);
        freeMatrix(three, new_d);
    }
}

void strassen_p(int d, int** matrix1, int** matrix2, int a_RS, int a_CS, int b_RS, int b_CS, int** answer){
    if(d==1){
        s_standard_mult(d, matrix1, matrix2, a_RS, a_CS, b_RS, b_CS, answer);
    }
    else{
        int d_n;
        if(d%2==0){
            d_n=d;
        }
        else{
            d_n=d+1;
            printf("d is an odd number");
            int** tmp1 = realloc(matrix1,d_n*d_n*sizeof(int));
            matrix1=tmp1;
            printf("Matrix 1 reallocated");
            int** tmp2 = realloc(matrix2,d_n*d_n*sizeof(int));
            matrix2=tmp2;
            printf("Matrix 2 reallocated");
            int** tmp3 = realloc(answer,d_n*d_n*sizeof(int));
            answer=tmp3;
            printf("Answer matrix reallocated");
        }
        
        int new_d = d_n/2;
        
        int** one = allocateMatrix_set_zero(new_d);
        int** two = allocateMatrix_set_zero(new_d);
        int** three = allocateMatrix_set_zero(new_d);
        
        /*************/
        
        /* Calculate P_4 */
        // G - E
        s_matrix_subtract(new_d, matrix2, matrix2, b_RS+new_d, b_CS, b_RS, b_CS, one, 0, 0);
        
        // P_4
        strassen(new_d, matrix1, one, a_RS+new_d, a_CS+new_d, 0, 0, two);
        
        /* AE + BG ... top left */
        initialize_values(new_d, two, 0, 0, answer, 0, 0);
        
        /* CE + DG ... bottom left */
        initialize_values(new_d, two, 0, 0, answer, 0+new_d, 0);
        
        /*************/
        
        /* Calculate P_2 */
        // A + B
        s_matrix_add(new_d, matrix1, matrix1, a_RS, a_CS, a_RS, a_CS+new_d, one, 0, 0);
        
        // P_2
        strassen(new_d, one, matrix2, 0, 0, b_RS+new_d, b_CS+new_d, two);
        
        /* AE + BG ... top left */
        s_matrix_subtract_to(new_d, two, 0, 0, answer, 0, 0);
        
        /* AF + BH ... top right */
        initialize_values(new_d, two, 0, 0, answer, 0, 0+new_d);
        
        
        /*************/
        
        /* Calculate P_5 */
        // A + D
        s_matrix_add(new_d, matrix1, matrix1, a_RS, a_CS, a_RS+new_d, a_CS+new_d, one, 0, 0);
        
        // E + H
        s_matrix_add(new_d, matrix2, matrix2, b_RS, b_CS, b_RS+new_d, b_CS+new_d, two, 0, 0);
        
        // P_5
        strassen(new_d, one, two, 0, 0, 0, 0, three);
        
        /* AE + BG ... top left */
        s_matrix_add_to(new_d, three, 0, 0, answer, 0, 0);
        
        /* CF + DH ... bottom right */
        initialize_values(new_d, three, 0, 0, answer, 0+new_d, 0+new_d);
        
        /*************/
        
        /* Calculate P_6 */
        // B - D
        s_matrix_subtract(new_d, matrix1, matrix1, a_RS, a_CS+new_d, a_RS+new_d, a_CS+new_d, one, 0, 0);
        
        // G + H
        s_matrix_add(new_d, matrix2, matrix2, b_RS+new_d, b_CS, b_RS+new_d, b_CS+new_d, two, 0, 0);
        
        // P_6
        strassen(new_d, one, two, 0, 0, 0, 0, three);
        
        /* AE + BG ... top left */
        s_matrix_add_to(new_d, three, 0, 0, answer, 0, 0);
        
        /*************/
        
        /* Calculate P_7 */
        // A - C
        s_matrix_subtract(new_d, matrix1, matrix1, a_RS, a_CS, a_RS+new_d, a_CS, one, 0, 0);
        
        // E + F
        s_matrix_add(new_d, matrix2, matrix2, b_RS, b_CS, b_RS, b_CS+new_d, two, 0, 0);
        
        // P_7
        strassen(new_d, one, two, 0, 0, 0, 0, three);
        
        /* CF + DH ... bottom right */
        s_matrix_subtract_to(new_d, three, 0, 0, answer, 0+new_d, 0+new_d);
        
        /*************/
        
        /* Calculate P_3 */
        // C + D
        s_matrix_add(new_d, matrix1, matrix1, a_RS+new_d, a_CS, a_RS+new_d, a_CS+new_d, one, 0,0);
        
        // P_3
        strassen(new_d, one, matrix2, 0, 0, b_RS, b_CS, two);
        
        /* CF + DH ... bottom right */
        s_matrix_subtract_to(new_d, two, 0, 0, answer, 0+new_d, 0+new_d);
        
        /* CE + DG ... bottom left */
        s_matrix_add_to(new_d, two, 0, 0, answer, 0+new_d, 0);
        
        /*************/
        
        /* Calculate P_1 */
        // F - H
        s_matrix_subtract(new_d, matrix2, matrix2, b_RS, b_CS+new_d, b_RS+new_d, b_CS+new_d, one, 0, 0);
        
        // P_1
        strassen(new_d, matrix1, one, a_RS, a_CS, 0, 0, two);
        
        /* AF + BH ... top right */
        s_matrix_add_to(new_d, two, 0, 0, answer, 0, 0+new_d);
        
        /* CF + DH ... bottom right */
        s_matrix_add_to(new_d, two, 0, 0, answer, 0+new_d, 0+new_d);
        
        /*************/
        padMatrix(answer, d_n, d);
        
        freeMatrix(one, new_d);
        freeMatrix(two, new_d);
        freeMatrix(three, new_d);
    }
}

int main(int argc, char *argv[]){
    if(argc < 4){
        printf("Not enough arguments!\n");
        return 1;
    }
    else if (argc > 4){
        printf("Too many arguments!\n");
        return 2;
    }
    clock_t start, elapsed, start_s, elapsed_s;
    // If flag is 1, for our own testing
    if(atoi(argv[1])==1){
        int d;
        printf("Enter the dimension of square matrices to be multiplied:\n");
        scanf("%i",&d);
        int x = nextPowofTwo(d); //2^ceil(log_2(d))
        
        //Check if input integer is a power of 2
        if(isPowerofTwo(d)==1){
            //Allocate memory to fit size d matrix - powers of 2
            printf("This is a power of two\n");
        }
        else{
            //Allocate memory to fit size x matrix - non-powers of 2
            printf("This is not a power of two\n");
            printf("Next power of two is %d\n",x);
        }
        int** a = allocateMatrix_set_zero(x); //first input matrix
        int** b = allocateMatrix_set_zero(x); //second input matrix
        
        printf("Enter the values of the first matrix:\n");
        for (int i=0; i<d; i++){
            for (int j=0; j<d; j++){
                scanf("%d", &a[i][j]);
            }
        }
        printf("Your first input matrix is:\n");
        display_mat(d, a);
        //Check non-powers of 2
        //display_mat(x, a);
        
        printf("Enter the values of the second matrix:\n");
        for (int i=0; i<d; i++){
            for (int j=0; j<d; j++){
                scanf("%d", &b[i][j]);
            }
        }
        printf("Your second input matrix is:\n");
        display_mat(d, b);
        //Check non-powers of 2
        //display_mat(x, b);
        
        
        //Standard multiplication
        int** c = allocateMatrix_set_zero(d); //standard result matrix
        standard_mult(d,a,b,c);
        //Strassen multiplication
        int** s_c = allocateMatrix_set_zero(x); //strassen result matrix
        strassen(x, a, b, 0, 0, 0, 0, s_c);
        
        printf("The final output (diagonal of product matrix) from the specs should be this:\n");
        display_diagonal(d, s_c);
        
        printf("The standard product of the two matrices is:\n");
        display_mat(d,c);
        
        printf("The strassen product of the two matrices is:\n");
        display_mat(d,s_c);
        
        //Subtraction of Standard and Strassen... to check if they're the same
        int** f = allocateMatrix_set_zero(d);
        matrix_subtract(d,c,s_c,f);
        printf("The difference of the two matrices should be all 0s:\n");
        display_mat(d,f);
        
        return 0;
    }
    
    // If flag is 2, for our own testing of correctness from inputted matrix from file
    if(atoi(argv[1])==2){
        int d = atoi(argv[2]);
        int x = nextPowofTwo(d);
        
        // Save the filename
        const char* filename = argv[3];
        
        int** a = allocateMatrix_set_zero(x); //first input matrix
        int** b = allocateMatrix_set_zero(x); //second input matrix
        
        int** s_c = allocateMatrix_set_zero(x); //strassen result matrix
        
        int** c = allocateMatrix_set_zero(x); //standard result matrix
        
        int** f = allocateMatrix_set_zero(x); //difference of results matrix
        
        
        FILE* file = fopen(filename, "r");
        
        //To fill up matrices from text file
        int i = 0;
        int j = 0;
        bool first_matrix = true;
        int number;
        while(fscanf(file, "%d" , &number) > 0) {
            if(first_matrix){
                a[i][j]=number;
                j++;
                if(j==d){
                    j=0;
                    i++;
                    if(i==d){
                        first_matrix = false;
                        i = 0;
                    }
                }
            }
            else{
                if(j==d){
                    j=0;
                    i+=1;
                }
                if(i==d){
                    i=0;
                }
                b[i][j]=number;
                j++;
            }
        }
        fclose(file);
        start = clock();
        strassen(x, a, b, 0, 0, 0, 0, s_c);
        elapsed = clock() - start;
        start_s = clock();
        standard_mult(x, a, b, c);
        elapsed_s = clock() - start_s;
        
        printf("The diagonal of strassen:\n");
        display_diagonal(d, s_c);
        
        matrix_subtract(d,c,s_c,f);
        bool worked = true;
        
        for (int i=0; i<d; i++){
            for (int j=0; j<d; j++){
                if(f[i][j]!=0){
                    worked = false;
                }
            }
        }
        
        /*
         printf("Standard\n");
         display_mat(d,c);
         
         printf("Strassen\n");
         display_mat(d,s_c);
         */
        
        printf("\nDid it work?\n");
        if(worked==true){
            printf("Yes!\n\n");
        }
        else{
            printf("No :(\n");
        }
        printf("Standard takes %d\n",1000*elapsed/CLOCKS_PER_SEC);
        printf("Strassen takes %d\n",1000*elapsed_s/CLOCKS_PER_SEC);
        //int msec = elapsed * 1000 / CLOCKS_PER_SEC;
        //printf("Standard algo takes %d seconds %d milliseconds\n", msec/1000, msec%1000);
        //int msec_s = elapsed_s * 1000 / CLOCKS_PER_SEC;
        //printf("Strassen algo takes %d seconds %d milliseconds", msec_s/1000, msec_s%1000)
    }
    
    // If flag is 3, for our own testing, just standard multiplication
    if(atoi(argv[1])==3){
        int d = atoi(argv[2]);
        int x = nextPowofTwo(d);
        
        // Save the filename
        const char* filename = argv[3];
        
        int** a = allocateMatrix_set_zero(x); //first input matrix
        int** b = allocateMatrix_set_zero(x); //second input matrix
        
        int** c = allocateMatrix_set_zero(x); //standard result matrix
        
        
        FILE* file = fopen(filename, "r");
        
        //To fill up matrices from text file
        int i = 0;
        int j = 0;
        bool first_matrix = true;
        int number;
        while(fscanf(file, "%d" , &number) > 0) {
            if(first_matrix){
                a[i][j]=number;
                j++;
                if(j==d){
                    j=0;
                    i++;
                    if(i==d){
                        first_matrix = false;
                        i = 0;
                    }
                }
            }
            else{
                if(j==d){
                    j=0;
                    i+=1;
                }
                if(i==d){
                    i=0;
                }
                b[i][j]=number;
                j++;
            }
        }
        fclose(file);
        
        standard_mult(x, a, b, c);
        
        printf("The diagonal of standard:\n");
        display_diagonal(d, c);
    }
    
    // If flag is 4, for our own testing, Curren test
    if(atoi(argv[1])==4){
        int d = atoi(argv[2]);
        //int x = nextPowofTwo(d);
        
        // Save the filename
        const char* filename = argv[3];
        
        int** a = allocateMatrix_set_zero(d); //first input matrix
        int** b = allocateMatrix_set_zero(d); //second input matrix
        
        int** s_c = allocateMatrix_set_zero(d); //strassen result matrix
        
        int** c = allocateMatrix_set_zero(d); //standard result matrix
        
        int** f = allocateMatrix_set_zero(d); //difference of results matrix
        
        
        FILE* file = fopen(filename, "r");
        
        //To fill up matrices from text file
        int i = 0;
        int j = 0;
        bool first_matrix = true;
        int number;
        while(fscanf(file, "%d" , &number) > 0) {
            if(first_matrix){
                a[i][j]=number;
                j++;
                if(j==d){
                    j=0;
                    i++;
                    if(i==d){
                        first_matrix = false;
                        i = 0;
                    }
                }
            }
            else{
                if(j==d){
                    j=0;
                    i+=1;
                }
                if(i==d){
                    i=0;
                }
                b[i][j]=number;
                j++;
            }
        }
        fclose(file);
        
        strassen(d, a, b, 0, 0, 0, 0, s_c);
        printf("The diagonal of strassen:\n");
        display_diagonal(d, s_c);
        
        strassen_p(d, a, b, 0, 0, 0, 0, c);
        
        matrix_subtract(d,c,s_c,f);
        bool worked = true;
        
        for (int i=0; i<d; i++){
            for (int j=0; j<d; j++){
                if(f[i][j]!=0){
                    worked = false;
                }
            }
        }
        
        printf("\nDid it work?\n");
        if(worked==true){
            printf("Yes!\n\n");
        }
        else{
            printf("No :(\n");
        }
        
    }
    
    // If flag is 0, as per pset specs, returns just the diagonal result.
    if(atoi(argv[1])==0){
        int d = atoi(argv[2]);
        int x = nextPowofTwo(d);
        
        // Save the filename
        const char* filename = argv[3];
        
        int** a = allocateMatrix_set_zero(x); //first input matrix
        int** b = allocateMatrix_set_zero(x); //second input matrix
        
        int** s_c = allocateMatrix_set_zero(x); //result matrix
        
        FILE* file = fopen(filename, "r");
        
        //To fill up matrices from text file
        int i = 0;
        int j = 0;
        bool first_matrix = true;
        int number;
        while(fscanf(file, "%d" , &number) > 0) {
            if(first_matrix){
                a[i][j]=number;
                j++;
                if(j==d){
                    j=0;
                    i++;
                    if(i==d){
                        first_matrix = false;
                        i = 0;
                    }
                }
            }
            else{
                if(j==d){
                    j=0;
                    i+=1;
                }
                if(i==d){
                    i=0;
                }
                b[i][j]=number;
                j++;
            }
        }
        fclose(file);
        
        strassen(x, a, b, 0, 0, 0, 0, s_c);
        
        display_diagonal(d, s_c);
    }
}