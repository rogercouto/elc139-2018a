#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_THREADS 4
#define N 15
#define TAM_VETOR NUM_THREADS*N

//Prototipo das funcoes auxiliares
void vetor_init(char* v);
bool vetor_add(char value, char* v);
void vetor_print(char* v);
char get_tchar();
void counter_print(char* v);

/**
 * Funcao principal
 * */
void main(void){
    int i;
    char v[TAM_VETOR];
    omp_set_num_threads(NUM_THREADS);
    printf("Cada thread deve colocar seu caractere no vetor N vezes (N = 20)\n");    
    printf("Resultado correto deve se exatamente NUM_THREADS*N caracteres\n");
    printf("Caso um: sem scheduling, sem mutex erros: ele faz o laço NUM_THREADS vezes a mais\n");
    vetor_init(v);
    #pragma omp parallel
    {
        for (i = 0; i < TAM_VETOR; i++){
            if (!vetor_add(get_tchar(),v)){
                printf("VETOR CHEIO!\n");
                break;
            }
        }
    }
    vetor_print(v);
    counter_print(v);

    printf("Caso dois, Auto scheduling sem mutex erro: adição na mesma posição do vetor, locais em branco\n");
    vetor_init(v);
    #pragma omp parallel
    {
        #pragma omp for schedule(auto)
        for (i = 0; i < TAM_VETOR; i++){
            vetor_add(get_tchar(),v);
        }
    }
    vetor_print(v);
    counter_print(v);
    printf("Caso correto, Auto scheduling com mutex\n");
    vetor_init(v);
    #pragma omp parallel
    {
        #pragma omp for schedule(auto)
        for (i = 0; i < TAM_VETOR; i++){
            #pragma omp critical
            vetor_add(get_tchar(),v);
        }
    }
    vetor_print(v);
    counter_print(v);

}

/**
 * Inicializa as posições do vetor com o caractere x
 * */
void vetor_init(char* v){
    int i;
    for (i = 0; i < TAM_VETOR; i++)
        v[i] = '.';
}

/**
 * Adiciona um caractere no próximo lugar livre 
 **/
bool vetor_add(char value, char* v){
    int i;
    for (i = 0; i < TAM_VETOR; i++){
        if (v[i] == '.'){
            v[i] = value;
            return true;
        }
    }
    return false;
}

/**
 * Imprime o vetor na tela
 **/
void vetor_print(char* v){
    int i;
    for (i = 0; i < TAM_VETOR; i++){
        printf((i == 0) ? "{" : "");
        printf("%c", v[i]);
    }
    printf("}\n");
}

/**
 * Retorna uma letra conforme o número da thread
 * */
char get_tchar(){
    return ('A'+omp_get_thread_num());
}

/**
 * Soma quantas posicoes do vetor foram ocupadas por cada thread e exibe o resultado
 **/
void counter_print(char* v){
    int i, j;
    int count[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; i++)
        count[i] = 0;
    for (i = 0; i < TAM_VETOR; i++){
        j = (int)(v[i]-'A');
        if (j < NUM_THREADS)
            count[j]++;
    }
    for (i = 0; i < NUM_THREADS; i++){
        if (i > 0)
            printf(", ");
        printf("%c: %d", ('A'+i),count[i]);
    }
    printf("\n");
}
