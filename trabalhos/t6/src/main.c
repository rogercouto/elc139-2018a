#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int** matriz_create(int tam){
	int** m = (int**)malloc(tam*sizeof(int*));
	for (int i = 0; i < tam; i++){
		m[i] = (int*)malloc(tam*sizeof(int));
		for (int j = 0; j < tam; j++){
			m[i][j] = 0;
		}
	}
	return m;
}

void matriz_destroy(int** m, int tam){
	for (int i = 0; i < tam; i++)
		free(m[i]);
	free(m);
}

void matriz_print(int** m, int tam){
	for (int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j ++){
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int nrainhas(int tam){
	int solucoes = 0;
	omp_set_num_threads(tam);
	#pragma omp parallel
	{
		int** m = matriz_create(tam);		
		m[omp_get_thread_num()][0] = 1;
		for (int i = 1; i < tam; i++){
			for(int j = 0; j < tam; j ++){
				
			}
		}
		#pragma omp critical
		matriz_print(m, tam);
		
		matriz_destroy(m, tam);
	}
	return solucoes;
}

int main(int argc, char** argv){
	int tam = 4; 
	
	printf("Soluções: %d\n", nrainhas(tam));
	
	return 0;
}

