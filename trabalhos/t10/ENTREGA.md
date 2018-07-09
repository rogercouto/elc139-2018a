[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2018a) > Trabalhos

# T10: Resolvendo um (segundo) problema da maratona de programação paralela

Disciplina: ELC139 - Programação Paralela <br/>
Aluno: Roger Eduardo de Couto

## Trabalho

Para o trabalho final foi escolhido último problema da maratona: Shortest Superstring

descrição desde e dos demais problemas [aqui](http://lspd.mackenzie.br/marathon/current/problemset.pdf)

Parâmetros: ./sss {threads} {filename}

## Metodologia

Código fonte do trabalho: [sss.cpp](entrega/sss.cpp)

Os testes a seguir foram feitos usando como entrada o arquivo: [input2.txt](entrega/input2.txt)

Tempo médio em segundos:

Sequencial: 14,57<br>
2 Threads: 7,65<br>
4 Threads: 5,88<br>
8 Threads: 6,14<br>

Speedup:

2 Threads: 1,9<br>
4 Threads: 2,49<br>
8 Threads: 2,37<br>


## Referências

Tutorial OpenMP: https://www.ibm.com/developerworks/br/aix/library/au-aix-openmp-framework/index.html


