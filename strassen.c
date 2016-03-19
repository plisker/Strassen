//T(n) = 7T(n/2) + Θ(n^2) --> T(n) = Θ(n^(log7)) = Θ(n^(2.807))

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void strassen(int, int**, int**, int, int, int, int, int**);

// Allocates memory for a square matrix of size d x d using double pointers...
int** allocateMatrix(int d) {
    int** matrix;
    matrix = (int**) malloc(d*sizeof(int*)); //allocate space for d rows of ints
    for (int i = 0; i < d; i++){
        matrix[i] = (int*) malloc(d*sizeof(int)); //allocate enough space for d columns of ints
        
        #warning This is probaby inefficient, especially since the function is used for malloc'ing several different matrices
        for (int j=0;j<d;j++){
    		matrix[i][j]=0; //solves padding 0 problem for non-powers of 2
    	}
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
void standard_mult(int d, int** a, int** b, int** answer){
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			answer[i][j] = 0;
			for (int k=0; k<d; k++){
				answer[i][j] += a[i][k]*b[k][j];
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

// Full subtract, no indexes tracked
void matrix_subtract(int d, int** a, int** b, int** answer){

	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			answer[i][j] = a[i][j]-b[i][j];
		}
	}
}

// Still not sure whether this -- and other functions -- should be void, or should be returning an int** double pointer to matrix
// I think returning pointers could make code more legible and clearner, but in any case, it works as is, so it's of low priority
void strassen(int d, int** matrix1, int** matrix2, int a_RS, int a_CS, int b_RS, int b_CS, int** answer){
	#warning Eventually this should be "if d < CROSSOVER"
	if(d==1){
		answer[0][0] = matrix1[a_RS][a_CS] * matrix2[b_RS][b_CS];
	}
	else{

		int new_d = d/2;

		#warning Are these too many allocations?
		int** one = allocateMatrix(new_d);
		int** two = allocateMatrix(new_d);
		int** three = allocateMatrix(new_d);
		int** four = allocateMatrix(new_d);
		int** five = allocateMatrix(new_d);
		int** six = allocateMatrix(new_d);
		int** seven = allocateMatrix(new_d);

		// F - H
		int** sub_a = allocateMatrix(new_d);
		s_matrix_subtract(new_d, matrix2, matrix2, b_RS, b_CS+new_d, b_RS+new_d, b_CS+new_d, sub_a, 0, 0);
		
		// P_1
		strassen(new_d, matrix1, sub_a, a_RS, a_CS, 0, 0, one);

		// A + B
		int** sub_b = allocateMatrix(new_d);
		s_matrix_add(new_d, matrix1, matrix1, a_RS, a_CS, a_RS, a_CS+new_d, sub_b, 0, 0);
		
		// P_2
		strassen(new_d, sub_b, matrix2, 0, 0, b_RS+new_d, b_CS+new_d, two);

		// C + D
		s_matrix_add(new_d, matrix1, matrix1, a_RS+new_d, a_CS, a_RS+new_d, a_CS+new_d, sub_a, 0,0);
		
		// P_3
		strassen(new_d, sub_a, matrix2, 0, 0, b_RS, b_CS, three);

		// G - E
		s_matrix_subtract(new_d, matrix2, matrix2, b_RS+new_d, b_CS, b_RS, b_CS, sub_a, 0, 0);
		
		// P_4
		strassen(new_d, matrix1, sub_a, a_RS+new_d, a_CS+new_d, 0, 0, four);

		// A + D
		s_matrix_add(new_d, matrix1, matrix1, a_RS, a_CS, a_RS+new_d, a_CS+new_d, sub_a, 0, 0);
		
		// E + H
		s_matrix_add(new_d, matrix2, matrix2, b_RS, b_CS, b_RS+new_d, b_CS+new_d, sub_b, 0, 0);

		// P_5
		strassen(new_d, sub_a, sub_b, 0, 0, 0, 0, five);

		// B - D
		s_matrix_subtract(new_d, matrix1, matrix1, a_RS, a_CS+new_d, a_RS+new_d, a_CS+new_d, sub_a, 0, 0);
		
		// G + H
		s_matrix_add(new_d, matrix2, matrix2, b_RS+new_d, b_CS, b_RS+new_d, b_CS+new_d, sub_b, 0, 0);

		// P_6
		strassen(new_d, sub_a, sub_b, 0, 0, 0, 0, six);

		// A - C
		s_matrix_subtract(new_d, matrix1, matrix1, a_RS, a_CS, a_RS+new_d, a_CS, sub_a, 0, 0);
		
		// E + F
		s_matrix_add(new_d, matrix2, matrix2, b_RS, b_CS, b_RS, b_CS+new_d, sub_b, 0, 0);

		// P_7
		strassen(new_d, sub_a, sub_b, 0, 0, 0, 0, seven);

		freeMatrix(sub_a, new_d);
		freeMatrix(sub_b, new_d);

		//Calculating the four sub matrices

		// AE + BG ... top left
		s_matrix_add(new_d, five, four, 0, 0, 0, 0, answer, 0+new_d, 0);
		s_matrix_subtract(new_d, answer, two, 0+new_d, 0, 0, 0, answer, 0+new_d, 0+new_d);
		s_matrix_add(new_d, answer, six, 0+new_d, 0+new_d, 0, 0, answer, 0, 0);

		// CF + DH ... bottom right
		s_matrix_add(new_d, five, one, 0, 0, 0, 0, answer, 0+new_d, 0);
		s_matrix_subtract(new_d, answer, three, 0+new_d, 0, 0, 0, answer, 0, 0+new_d);
		s_matrix_subtract(new_d, answer, seven, 0, 0+new_d, 0, 0, answer, 0+new_d, 0+new_d);

		// AF + BH ... top right
		s_matrix_add(new_d, one, two, 0, 0, 0, 0, answer, 0, 0+new_d);
		
		// CE + DG ... bottom left
		s_matrix_add(new_d, three, four, 0, 0, 0, 0, answer, 0+new_d, 0);

		freeMatrix(one, new_d);
		freeMatrix(two, new_d);
		freeMatrix(three, new_d);
		freeMatrix(four, new_d);
		freeMatrix(five, new_d);
		freeMatrix(six, new_d);
		freeMatrix(seven, new_d);

		/*
		*** Code for debugging. Can delete later! ***

		printf("One:\n");
		display_mat(new_d, one);

		printf("Two:\n");
		display_mat(new_d, two);

		printf("Three:\n");
		display_mat(new_d, three);

		printf("Four:\n");
		display_mat(new_d, four);

		printf("Five:\n");
		display_mat(new_d, five);

		printf("Six:\n");
		display_mat(new_d, six);

		printf("Seven:\n");
		display_mat(new_d, seven);
		*/

	}
}

int main(void){
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
	int** a = allocateMatrix(x); //first input matrix
	int** b = allocateMatrix(x); //second input matrix

	
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
	int** c = allocateMatrix(d); //standard result matrix
	standard_mult(d,a,b,c);
	
	//Strassen multiplication
	int** s_c = allocateMatrix(x); //strassen result matrix
	strassen(x, a, b, 0, 0, 0, 0, s_c);	

	printf("The final output from the specs should be this:\n");
	display_diagonal(d, s_c);
	
	printf("The standard product of the two matrices is:\n");
	display_mat(d,c);

	printf("The strassen product of the two matrices is:\n");
	display_mat(d,s_c);

	//Subtraction of Standard and Strassen... to check if they're the same
	int** f = allocateMatrix(d);
	matrix_subtract(d,c,s_c,f);
	printf("The difference of the two matrices should be all 0s:\n");
	display_mat(d,f);

	#warning Do we need to free things here at the end?
	return 0;
}