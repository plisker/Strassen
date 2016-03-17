//T(n) = 7T(n/2) + Θ(n^2) --> T(n) = Θ(n^(log7)) = Θ(n^(2.807))

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int[][] m_malloc(dim){
	int[][] m = (int[][]) malloc(dim * sizeof(int*));
	for (int i=0;i<dim;i++){
		m[i]=(int*) malloc(dim * sizeof(int));
	}
	return m;
}

void m_free(int[][] m, int dim){
	for (int i=0; i<dim; i++){
		free(m[i]);
	}
	free m;
	return;
}

