[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T8: Comunicação coletiva em MPI

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Roger Eduardo Couto

## Parte 1

Alterado código original, trocando send/recieve para:
MPI_Bcast para enviar a matriz B que é utilizada por todos os processos
MPI_Scatter para dividir a matriz pelos processos e fazer o calculo em cada processo
Recebimento continuou sendo feito por MPI_Gather

Código da solução: [parte1.c](entrega/mult_mat_mpi1.c)

## Parte 2 

Primeiramente foi feito uma tentativa de utilizar as funções  MPI_Scatter e MPI_Gatherv porém os elementos da matriz a estavam sendo recebidos e colocados nas posições iniciais da matriz

A solução foi fazer uma divisão pelos processos e linhas restantes alocados entre os processos
por exemplo, com 5 processos ele irá mandar o seguinte número de linhas para os processos: 2, 2, 2, 1, 1
o método de comunicação foi send/recieve, foi feito um send/recieve para cada linha (com exceção das multiplicadas pelo processo 0) para os processos conforme a distribuição calculada anteriormente.

Código da solução: [parte2.c](entrega/mult_mat_mpi2.c)

## Referências
http://mpitutorial.com

