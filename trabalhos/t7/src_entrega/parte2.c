#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "mpi.h"

int main(){
    int rank; // "rank" do processo
    int np; // numero de processos

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    char msg_env[10] = "teste"; // mensagem enviada
    char msg_tmp[10]; //mensagem temporária
    char msg_rec[10]; //mensagem recebida

    if (rank == 0){
        //primeiro processo
        strcpy(msg_tmp, msg_env);
    }else{
        //todos os processos menos o primeiro
        printf("p[%d] <- p[%d]\n", rank, rank-1);
        //recebendo mensagem do processo anterior
        MPI_Recv(msg_tmp, strlen(msg_tmp)+1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
        if (rank == np -1){
            //último processo
            strcpy(msg_rec, msg_tmp);
            printf("p[%d]=%s\n",rank, msg_rec);
        }
    }
    if (rank < np-1){
        //todos os processos menos o último
        printf("p[%d] -> p[%d]\n", rank, rank+1);
        //enviando mensagem para o próximo processo
        MPI_Send(msg_tmp, strlen(msg_tmp)+1, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize(); // finaliza MPI 
    return EXIT_SUCCESS;
}
