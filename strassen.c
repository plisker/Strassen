//T(n) = 7T(n/2) + Θ(n^2) --> T(n) = Θ(n^(log7)) = Θ(n^(2.807))

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// allocates & initializes a matrix of size dim
int[][] m_malloc(dim){
	int[][] m = (int[][]) malloc(dim * sizeof(int*));
	for (int i=0; i<dim; i++){
		m[i]=(int*) malloc(dim * sizeof(int));
	}
	return m;
}

// frees said matrix of size dim
void m_free(int[][] m, int dim){
	for (int i=0; i<dim; i++){
		free(m[i]);
	}
	free m;
	return;
}

// standard matrix multiplication
int[][] standard_mult(int[][] a, int[][] b, dim){
	c = m_malloc(dim);
	for (int i=0; i<dim; i++){
		for (int j=0; j<dim; j++){
			c[i][j] = 0;
			for (int k=0; k<dim; k++){
				c[i][j] += c[i][k]*c[k][j];
			}
		}
	}
}

int main(){
	int d;
	printf("Enter the dimension of square matrices to be multiplied:\n");
	scanf("%d",&d);
	m1 = m_malloc(d);
	m2 = m_malloc(d);
	printf("Enter the values of the first matrix:\n");
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			scanf("%d", &m1[i][j]);
		}
	}
	printf("Enter the values of the second matrix:\n");
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++){
			scanf("%d", &m2[i][j]);
		}
	}
	m3 = standard_mult(m1,m2,d);
	printf("The resulting matrix is:\n");
	for (int i=0; i<d; i++){
		printf("\n");
		for (int j=0; j<d; j++){
			printf("%d\t", m3[i][j]);
		}
	}
	m_free(m1,d);
	m_free(m2,d);
	return 0
}