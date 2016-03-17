//T(n) = 7T(n/2) + Θ(n^2) --> T(n) = Θ(n^(log7)) = Θ(n^(2.807))

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// allocates & initializes a matrix of size dim
// int** m_malloc(dim){
// 	int** m = (int**) malloc(dim * sizeof(int*));
// 	for (int i=0; i<dim; i++){
// 		m[i]=(int*) malloc(dim * sizeof(int));
// 	}
// 	return m;
// }

// // frees said matrix of size dim
// void m_free(int** m, int dim){
// 	for (int i=0; i<dim; i++){
// 		free(m[i]);
// 	}
// 	free(m);
// 	return;
// }


// Allocates memory for a square matrix of size N x N using double pointers...
#warning Will still need to free at some point…
int** allocateMatrix(int N) {
    int** matrix;
    matrix = (int**) malloc(N*sizeof(int*));
    for (int i = 0; i < N; i++)
        matrix[i] = (int*) malloc(N*sizeof(int));
    return matrix;
}

//STANDARD ALGORITHM
// standard matrix multiplication
void standard_mult(int d, int** a, int** b, int** answer){
	// int** c = m_malloc(dim);
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			answer[i][j] = 0;
			for (int k=0; k<d; k++){
				answer[i][j] += a[i][k]*b[k][j];
			}
		}
	}
}

//prints the matrix
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

//STRASSEN ALGORITHM
// matrix addition
void matrix_add(int d, int** a, int** b, int** answer){
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			answer[i][j] = a[i][j]+b[i][j];
		}
	}
}

//matrix subtraction
void matrix_subtract(int d, int** a, int** b, int** answer){

	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			answer[i][j] = a[i][j]-b[i][j];
		}
	}
}

//main function that lets user test
int main(void){
	int d;
	//What was tis supposed to be?
	//printf("Size of matrix is %d\n", (int) sizeof(a));
	printf("Enter the dimension of square matrices to be multiplied:\n");
	scanf("%d",&d);
	// int** m1 = m_malloc(d);
	// int** m2 = m_malloc(d);
	int** a = allocateMatrix(d);

	printf("Enter the values of the first matrix:\n");
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			scanf("%d", &a[i][j]);
		}
	}

	int** b = allocateMatrix(d);

	printf("Enter the values of the second matrix:\n");
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			scanf("%d", &b[i][j]);
		}
	}

	int** c = allocateMatrix(d);

	standard_mult(d,a,b,c);
	printf("The product of the two matrices is:\n");
	display_mat(d,c);

	int** e = allocateMatrix(d);

	matrix_add(d,a,b,e);
	printf("The sum of the two matrices is:\n");
	display_mat(d,e);

	int** f = allocateMatrix(d);

	matrix_subtract(d,a,b,f);
	printf("The difference of the two matrices is:\n");
	display_mat(d,f);
	// m_free(m1,d);
	// m_free(m2,d);
	return 0;
}