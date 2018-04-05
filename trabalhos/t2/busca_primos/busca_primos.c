#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAM_ELEM 1000

typedef struct presult{
    int id, total;
}PRESULT;

typedef struct timer{
    struct timespec tIni, tFim;
}TIMER;

bool is_primo(int num);
int cont_primos_seq(int* vet, int ini, int fim);
void seed();
int* gera_vetor_rand(int n);
int* gera_vetor_seq(int n);
int procura_primos_sequencial(int* vetor, int n);
PRESULT* procura_primos_threads(int* vetor, int n, int m);
PRESULT* procura_primos_processos(int* vetor, int n, int m);
double timedif(struct timespec tIni, struct timespec tFim);
void start_timer(TIMER* timer);
double end_timer(TIMER *timer);

int main(int argc, char** argv){
    //n - tamanho do vetor, m - numero de threads/processos
    double te[3]; //Tempo de execução 0-seq, 1-threads, 2-processos
    int i, n, m;
    int* vet;
    PRESULT* res;
    TIMER timer;
    if (argc < 3){
        puts("Falta(m) parametro(s)!");
        exit(-1);
    }
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    if (n == 0 || m == 0){
        puts("Parametro(s) invalido(s)!");
        exit(-1);
    }
    if (n % m != 0){
        puts("Numero de elementos do vetor precisa ser divisivel pelo numero de threads processos!");
        exit(-1);
    }
    /*
    if (n > 80000000){
        puts("Numero de elementos do vetor muito alto!");
        exit(-1);
    }
    */
    seed();
    vet = gera_vetor_rand(n);
    //vet = gera_vetor_seq(n);
    start_timer(&timer);//Inicia o contador de tempo para o metodo sequencial
    printf("Sequencial -> %d numeros primos!\n", procura_primos_sequencial(vet, n));
    te[0] = end_timer(&timer);//Encerra o contador de tempo e libera a memória
    start_timer(&timer);//Inicia o contador de tempo para as threads
    res = procura_primos_threads(vet, n, m);
    for (i = 0; i < m; i++)
        printf("Thread %d -> %d numeros primos!\n", res[i].id, res[i].total);
    free(res);
    te[1] = end_timer(&timer);//Encerra o contador de tempo e libera a memória
    start_timer(&timer);//Inicia o contador de tempo para os processos
    res = procura_primos_processos(vet, n, m);
    for (i = 0; i < m; i++)
        printf("Processo PID: %d -> %d numeros primos!\n", res[i].id, res[i].total);
    free(res);
    te[2] = end_timer(&timer);//Encerra o contador de tempo e libera a memória
    free(vet); // Libera a memória do vetor
    FILE* arq; // Arquivo para escrever o resultado
    //Salva o restultado em um arquivo
    arq = fopen("results.csv", "a");
    if (arq != NULL){
            fprintf(arq, "\"%lf\",\"%lf\",\"%lf\"\n", te[0], te[1], te[2]);
            fclose(arq);
    }
    return 0;
}

/*
 * Verifica se um número é primo
 */ 
bool is_primo(int num){
    int i, t;
    if (num <= 1)
        return false;
    t = 0; 
    for (i = 2; i <= sqrt(num); i++){
        if (num % i == 0)
            return false;
    }
    return true;
}

/*
 * Conta o numero de primos sequencialmente
 */
int cont_primos_seq(int* vet, int ini, int fim){
    int i, total;
    total = 0;
    for (i = ini; i < fim; i++)
        if (is_primo(vet[i]))
            total++;
    return total;
}

/*
 *Procedimento que alimenta o rand() com uma semente
*/
void seed(){
    srand( (unsigned)time(NULL) );
}

/*
 *Procedimento que gera um vetor de numeros aleatorios
 * n: tamanho do vetor a ser gerado
 */
int* gera_vetor_rand(int n){
    int i;
    int* vp = (int*)malloc(n*sizeof(int));
    for (i = 0; i < n; i++)
        vp[i] = rand()%TAM_ELEM;
    return vp;
}

/**
 * Procedimento que gera u vetor de numeros sequencias
 * n: tamanho do vetor a ser gerado
 */
int* gera_vetor_seq(int n){
    int i;
    int* vp = (int*)malloc(n*sizeof(int));
    for (i = 0; i < n; i++)
        vp[i] = i;
    return vp;
}

/*
 * Busca sequencialmente por numeros primos no vetor
 */
int procura_primos_sequencial(int* vetor, int n){
    return cont_primos_seq(vetor, 0, n);
}

/*
 *  Strutura passada para a thred_procura_primos
 */
typedef struct{
    int tid, ini, fim, total;
    int* vet;
}svetor;

/*
 * Thread para pesquisar por primos
 */
void* thread_procura_primos(void* arg){
    svetor* svet = (svetor*) arg;
    svet->total = cont_primos_seq(svet->vet, svet->ini, svet->fim);
}

/*
 * Porcura primos utilizando threads
 */
PRESULT* procura_primos_threads(int* vet, int n, int m){
    int i, l;
    l = n / m;
    pthread_t t[m];
    svetor svets[m];
    PRESULT* result;
    for (i = 0; i < m; i++){
        svets[i].tid = i;
        svets[i].vet = vet;
        svets[i].ini = i*l;
        svets[i].fim = svets[i].ini + l;
        pthread_create(&t[i], NULL, thread_procura_primos, &svets[i]);
    }
    result = (PRESULT*)malloc(m*sizeof(PRESULT));
    for (i = 0; i < m; i++){
        pthread_join(t[i], NULL);
        result[i].id = svets[i].tid;
        result[i].total = svets[i].total;
    }
    return result;
}

/*
* Procura primos utilizando threads
*/
PRESULT* procura_primos_processos(int* vetor, int n, int m){
    int i, j, l, pi;
    l = n / m; // tamanho do subvetor
    int fd[m][2]; //pipes envio
    int fdr[m][2]; //pipes retorno
    pid_t pid[m], ppid;
    ppid = getpid(); //pid do processo original
    PRESULT res[m];
    PRESULT* result;
    //Criando os subprocessos
    for (i = 0; i < m; i++){
        if(pipe(fd[i]) < 0){
            perror("Erro ao criar pipe!");
            exit(-1);
        }
        if(pipe(fdr[i]) < 0){
            perror("Erro ao criar pipe!");
            exit(-1);
        }
        if (getpid() == ppid){
            pid[i] = fork();
            if (pid[i] > 0){
                close(fd[i][0]); //Fechando a entrada de leitura no pipe
                write(fd[i][1], &i, sizeof(i));
                if ( i == m-1){
                    for (j = m; j > 0; j--){
                        waitpid(pid[j], NULL, 0);
                    }
                }
            }else if (pid[i] == 0){
                close(fd[i][1]);
                read(fd[i][0], &pi, sizeof(pi)); 
            }
        }
    }
    if (getpid() != ppid){
        int ini, fim, total;
        ini = l * pi;
        fim = ini + l;
        total = cont_primos_seq(vetor, ini, fim);
        close(fdr[pi][0]); //Fechando a entrada de leitura no pipe
        res[pi].id = getpid();
        res[pi].total = total;
        write(fdr[pi][1], &res[pi], sizeof(res[pi]));
    }else{
        //PRESULT rres[m];
        result = (PRESULT*)malloc(m*sizeof(PRESULT));
        for (i = 0; i < m; i++){
            close(fdr[i][1]);
            read(fdr[i][0], &result[i], sizeof(result[i])); //Lendo o subvetor que ele precisa verificar
        } 
    }
    if (getpid() != ppid)
        exit(0);
    return result;
}

/*
* Retorna a diferença entre 2 tempos
*/
double timedif(struct timespec tIni, struct timespec tFim){
    return ((double)tFim.tv_sec - tIni.tv_sec) + ((double)(tFim.tv_nsec-tIni.tv_nsec) * 1e-9);
}

/*
* Inicia a contagem do tempo, retorna um ponteiro para a estrutura timer
*/
void start_timer(TIMER* timer){
    clock_gettime(CLOCK_MONOTONIC_RAW, &timer->tIni); 
}

/*
* Encerra o timer, retorna tempo em segundos desde o metodo start
*/
double end_timer(TIMER *timer){
    clock_gettime(CLOCK_MONOTONIC_RAW, &timer->tFim);   
    double d = timedif(timer->tIni, timer->tFim);
    printf("\nTempo de execução: %lf seg.\n\n", d);
    return d;
}