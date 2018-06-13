#include <mpi.h>
#include <stdio.h>

#define SIZE 8

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

void fill_matrix(int m[SIZE][SIZE]){
  static int n=0;
  int i, j;
  for (i=0; i<SIZE; i++)
    for (j=0; j<SIZE; j++)
      m[i][j] = n++;
}

void print_matrix(int m[SIZE][SIZE]){
  int i, j = 0;
  for (i=0; i<SIZE; i++) {
    printf("\n\t| ");
    for (j=0; j<SIZE; j++)
      printf("%*d", 6, m[i][j]);
    printf("|");
  }
}

void fill_counts(int counts[], int size){
    int rem = SIZE;
    int max = SIZE / size;
    for (int i = 0; i < size; i++){
        counts[i] = max;
        rem -= max;
    }
    int count = 0;
    while (rem > 0){
        counts[count++]++;
        rem--;
    }
}

void fill_frons(int frons[], int counts[], int size){
    int sum = 0;
    for(int i = 0; i < size; i++){
        frons[i] = sum;
        sum += counts[i];
    }
}

void print_vet(int v[], int size){
    printf("{");
    for (int i = 0; i < size; i++){
        if (i > 0)
            printf(", ");
        printf("%d", v[i]);
    }
    printf("}\n");
}

/**
 * Faz a multiplicação das matrizes
 * */
void multiplica_matrizes(int from, int to){
    for (int i = from; i < to; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j]=0;
            for (int k=0; k< SIZE; k++){
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]){
    int myrank, nproc, from, to;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
    MPI_Comm_size(MPI_COMM_WORLD, &nproc); /* number of processors */
    MPI_Status status;

    int counts[nproc];
    int frons[nproc];

    fill_counts(counts, nproc);
    fill_frons(frons, counts, nproc);
    
    if (myrank == 0){
        fill_matrix(A);
        fill_matrix(B);
        print_vet(counts, nproc);
        print_vet(frons, nproc);
    }
    //Envia a matriz B por broadcast para todos os processos
    MPI_Bcast(B, SIZE*SIZE, MPI_INT, 0 ,MPI_COMM_WORLD);

    //Envia partes da matriz
    if (myrank == 0){
        //Envia todas as linhas
        int proc = 1;
        int start = frons[1];
        //2 até 7
        for (int i = start; i < SIZE; i++){
            if (i == frons[proc+1])
                proc++;
            MPI_Send(A[i], SIZE, MPI_INT, proc, i, MPI_COMM_WORLD);
        }
    }else{
        int proc = 1;
        int start = frons[1];
        for (int i = start; i < SIZE; i++){
            if (i == frons[proc+1])
                proc++;
            if (proc == myrank){
                MPI_Recv(A[i], SIZE, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
            }    
        }
    }
    from = frons[myrank];
    to = from+counts[myrank];
    //Faz o cálculo de C
    multiplica_matrizes(from, to);
    if (myrank > 0){
        //Envia todas as respostas
        for (int i = from; i < to; i++){
            MPI_Send(C[i], SIZE, MPI_INT, 0, i, MPI_COMM_WORLD);
        }
    }else{
        int proc = 1;
        int start = frons[1];
        for (int i = start; i < SIZE; i++){
            if (i == frons[proc+1])
                proc++;
            printf("Recieving c[i:%d], tag:%d\n", i, i);
            MPI_Recv(C[i], SIZE, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
        }
    }

    if (myrank==0) {
        printf("\n\n");
        print_matrix(A);
        printf("\n\n\t       * \n");
        print_matrix(B);
        printf("\n\n\t       = \n");
        print_matrix(C);
        printf("\n\n");
    }

    MPI_Finalize();
    return 0;
}