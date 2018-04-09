#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct{
    double *a;
    double *b;
    double c; 
    int wsize;
    int repeat; 
} dotdata_t;

typedef struct{
    long start, end;
} ctimer_t;

/*
 * Tempo (wallclock) em microssegundos
 */ 
long wtime(){
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec*1000000 + tv.tv_usec;
}


// Variaveis globais
dotdata_t dotdata;
ctimer_t timer;

// Funcoes auxiliares
void timer_init(){
    timer.start = wtime();
}

void timer_end(){
    timer.end = wtime();
    printf("Tempo de execução: %ld usec\n", (long) (timer.end - timer.start));
    timer.start = 0;
    timer.end = 0;
}

/*
 * Preenche vetor
 */ 
void fill(double *a, int size, double value){  
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}

/*
 * Cria vetor 
 **/
void cria_vetor(){
    dotdata.a = (double *) malloc(dotdata.wsize*sizeof(double));
    fill(dotdata.a, dotdata.wsize, 0.01);
    dotdata.b = (double *) malloc(dotdata.wsize*sizeof(double));
    fill(dotdata.b, dotdata.wsize, 1.0);
    dotdata.c = 0.0;
}

/**
 * Realiza o calculo do produto escalar
 * */
void dotprod(){
    int i, k;
    double*  a = dotdata.a;
    double*  b = dotdata.b;
    double sum;
    int wsize = dotdata.wsize;
    
    #pragma omp parallel shared(a, b) private (i, sum)
    {
        for (k = 0; k < dotdata.repeat; k++) {
            sum = 0.0;      
            for (i = 0; i < wsize ; i++)  {
                sum += (a[i] * b[i]);
            }
        }
        #pragma omp critical
        dotdata.c += sum;
    }
}
/*
 * Funcao principal
 */ 
int main(int argc, char **argv){
    if ((argc != 4)) {
        printf("Uso: %s <nthreads> <worksize> <repetitions>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    omp_set_num_threads(atoi(argv[1]));
    dotdata.wsize = atoi(argv[2]);  // worksize = tamanho do vetor de cada thread
    dotdata.repeat = atoi(argv[3]); // numero de repeticoes dos calculos (para aumentar carga)
    cria_vetor(); //cria vetor
    timer_init();
    dotprod();
    timer_end();
    free(dotdata.a);
    free(dotdata.b);
    printf("%f\n", dotdata.c);
    return EXIT_SUCCESS;
}