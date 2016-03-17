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

// standard matrix multiplication
void standard_mult(int d, int a[][10], int b[][10], int c[][10]){
	// int** c = m_malloc(dim);
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			c[i][j] = 0;
			for (int k=0; k<d; k++){
				c[i][j] += a[i][k]*b[k][j];
			}
		}
	}
}

//prints the matrix
void display_mat(int d, int c[][10]){
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			printf("%d\t ",c[i][j]);
			if(j==d-1){
				printf("\n");
			}
		}
	}
}

//main function that lets user test
int main(void){
	int a[10][10],b[10][10],c[10][10], d;
	printf("Enter the dimension of square matrices to be multiplied:\n");
	scanf("%d",&d);
	// int** m1 = m_malloc(d);
	// int** m2 = m_malloc(d);
	printf("Enter the values of the first matrix:\n");
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			scanf("%d", &a[i][j]);
		}
	}
	printf("Enter the values of the second matrix:\n");
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			scanf("%d", &b[i][j]);
		}
	}
	standard_mult(d,a,b,c);
	printf("The resulting matrix is:\n");
	// for (int i=0; i<d; i++){
	// 	printf("\n");
	// 	for (int j=0; j<d; j++){
	// 		printf("%d\t", c[i][j]);
	// 	}
	// }
	display_mat(d,c);
	// m_free(m1,d);
	// m_free(m2,d);
	return 0;
}