/*
 *  Exemplo de programa para calculo de produto escalar em paralelo, usando MPI
 * baseado no código utilizando pthreads
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

typedef struct 
 {
   double *a;
   double *b;
   double c; 
   int wsize;
   int repeat; 
 } dotdata_t;

// Variaveis globais, acessiveis por todas threads
dotdata_t dotdata;
pthread_mutex_t mutexsum;

double dotprod(int arg){
    int i, k;
    long offset = (long) arg;
    double *a = dotdata.a;
    double *b = dotdata.b;     
    int wsize = dotdata.wsize;
    int start = offset*wsize;
    int end = start + wsize;
    double mysum;
    for (k = 0; k < dotdata.repeat; k++) {
        mysum = 0.0;
        for (i = start; i < end ; i++)  {
            mysum += (a[i] * b[i]);
        }
    }
    return mysum;
}

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */ 
void fill(double *a, int size, double value)
{  
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}

/*
 * Funcao principal
 */ 
int main(int argc, char **argv)
{
    int myrank; // "rank" do processo (0 a P-1)
    int np; // numero de processos

    int wsize, repeat;
    long start_time, end_time;

    if ((argc != 3)) {
        printf("Uso: %s <worksize> <repetitions>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    wsize = atoi(argv[1]);  // worksize = tamanho do vetor de cada thread
    repeat = atoi(argv[2]); // numero de repeticoes dos calculos (para aumentar carga)

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &np); 

    // Cria vetores
    dotdata.a = (double *) malloc(wsize*(np-1)*sizeof(double));
    fill(dotdata.a, wsize*(np-1), 0.01);
    dotdata.b = (double *) malloc(wsize*(np-1)*sizeof(double));
    fill(dotdata.b, wsize*(np-1), 1.0);
    dotdata.c = 0.0;
    dotdata.wsize = wsize;
    dotdata.repeat = repeat;

    start_time = wtime();
    // Calcula c = a . b
    printf("RANK: %d\n",myrank);
    if (myrank > 0){
        //processo que faz a contagem parcial
        double res = dotprod(myrank-1);
        MPI_Send(&res, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }else{
        //processo que vai receber a contagem
        double rec, psum;//soma parcial
        psum = 0;
        for (int source = 1; source < np; source ++){
            MPI_Recv(&rec, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            psum += rec;
        }
        dotdata.c = psum;
        end_time = wtime();
        // Mostra resultado e estatisticas da execucao
        printf("%f\n", dotdata.c);
        printf("%d processo(s), %ld usec\n", np, (long) (end_time - start_time));
        fflush(stdout);
        free(dotdata.a);
        free(dotdata.b);
    }
    MPI_Finalize(); 
    return EXIT_SUCCESS;
}

