# Disciplina: Programação Paralela
# Aluno: Roger Eduardo de Couto

# Parte 1

## sem profiling 3000 10

Produto escalar = 1500.000000
Tempo de calculo = 246 usec

real	0m0,002s
user	0m0,000s
sys	0m0,002s

## sem profiling 30000000 90

Produto escalar = 15000000.000000
Tempo de calculo = 8982806 usec

real	0m9,253s
user	0m9,100s
sys	0m0,128s

## com profiling 3000 10

Produto escalar = 1500.000000
Tempo de calculo = 254 usec

real	0m0,002s
user	0m0,000s
sys	0m0,002s

## resultados do gprof (irrelevante com uma configuração pequena)

 %   cumulative   self              self     total
 time   seconds   seconds    calls  Ts/call  Ts/call  name
  0.00      0.00     0.00        2     0.00     0.00  wtime
  0.00      0.00     0.00        1     0.00     0.00  dot_product
  0.00      0.00     0.00        1     0.00     0.00  init_vectors

## com profiling 30000000 90

Produto escalar = 15000000.000000
Tempo de calculo = 9417664 usec

real	0m9,699s
user	0m9,492s
sys	0m0,193s

## resultados do gprof

%   cumulative   self              self     total
 time   seconds   seconds    calls   s/call   s/call  name
 98.17      9.47     9.47        1     9.47     9.47  dot_product
  2.50      9.71     0.24        1     0.24     0.24  init_vectors
  0.00      9.71     0.00        2     0.00     0.00  wtime

## Resultados
a) Quanto maior a configuração mais tempo leva para executar, porém habilitar o profiling não muda o tempo de execução significativamente
b) Não tem nenhuma função que seria beneficiada por paralelismo, pois a função que mais demora pra ser executada só é chamada uma vez

# Segunda parte - Programa escolhido: Programa que busca numeros primos em um vetor e numero processos/threads passado como parâmetro
## Criado na disciplina de Sistemas Operacionais
## Observação: o programa já continha funções para verificar o tempo de execução, e já paralelizava a função is_primo
## Os 2 profilers utilizados foram o gprof e o valgrind

## Resultados com gprof
## Utilizando 1 processo/thread - Tamanho do vetor: 100000000

Sequencial -> 16797479 numeros primos!

Tempo de execução: 6.850131 seg.

Thread 0 -> 16797479 numeros primos!

Tempo de execução: 6.908519 seg.

Processo PID: 7921 -> 16797479 numeros primos!

Tempo de execução: 6.855015 seg.


real	0m21,858s
user	0m21,683s
sys	0m0,172s

Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total
 time   seconds   seconds    calls   s/call   s/call  name
 66.04      4.40     4.40 200000000     0.00     0.00  is_primo
 15.95      5.46     1.06        2     0.53     2.73  cont_primos_seq
  7.22      5.94     0.48        1     0.48     0.48  gera_vetor_rand
  1.05      6.01     0.07                             main
  0.00      6.01     0.00        3     0.00     0.00  end_timer
  0.00      6.01     0.00        3     0.00     0.00  start_timer
  0.00      6.01     0.00        3     0.00     0.00  timedif
  0.00      6.01     0.00        1     0.00     0.00  procura_primos_processos
  0.00      6.01     0.00        1     0.00     2.73  procura_primos_sequencial
  0.00      6.01     0.00        1     0.00     0.00  procura_primos_threads
  0.00      6.01     0.00        1     0.00     0.00  seed

## Utilizando 4 Processos/Threads - Tamanho do vetor: 

Sequencial -> 16795425 numeros primos!

Tempo de execução: 6.790088 seg.

Thread 0 -> 4199792 numeros primos!
Thread 1 -> 4197232 numeros primos!
Thread 2 -> 4199214 numeros primos!
Thread 3 -> 4199187 numeros primos!

Tempo de execução: 3.520576 seg.

Processo PID: 7964 -> 4199792 numeros primos!
Processo PID: 7969 -> 4197232 numeros primos!
Processo PID: 7971 -> 4199214 numeros primos!
Processo PID: 7972 -> 4199187 numeros primos!

Tempo de execução: 2.813325 seg.


real	0m14,353s
user	0m29,605s
sys	0m0,236s


Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total 
 time   seconds   seconds    calls   s/call   s/call  name 
 64.85      4.12     4.12 135569999     0.00     0.00  is_primo
 14.52      5.04     0.92        3     0.31     1.68  cont_primos_seq
  6.78      5.47     0.43        1     0.43     0.43  gera_vetor_rand
  0.47      5.50     0.03                             main
  0.00      5.50     0.00        3     0.00     0.00  end_timer
  0.00      5.50     0.00        3     0.00     0.00  start_timer
  0.00      5.50     0.00        3     0.00     0.00  timedif
  0.00      5.50     0.00        1     0.00     0.00  procura_primos_processos
  0.00      5.50     0.00        1     0.00     1.68  procura_primos_sequencial
  0.00      5.50     0.00        1     0.00     0.00  procura_primos_threads
  0.00      5.50     0.00        1     0.00     0.00  seed

# Utlizando o segundo profiler: Valgrind
## Foi necessário reduzir o tamanho do vetor para 10000000 pois o valgrind impacta no desmpenho do programa fazendo o mesmo levar muito mais tempo

## saida do terminal:
==8220== Memcheck, a memory error detector
==8220== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==8220== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==8220== Command: ./busca_primos 10000000 1
==8220== 
Sequencial -> 1681425 numeros primos!

Tempo de execução: 11.785948 seg.

Thread 0 -> 1681425 numeros primos!

Tempo de execução: 11.727406 seg.

==8220== Syscall param wait4(pid) contains uninitialised byte(s)
==8220==    at 0x4E4EA4A: waitpid (waitpid.c:29)
==8220==    by 0x40192B: procura_primos_processos (in /home/roger/GIT/programacaoparalela/t2/busca_primos/busca_primos)
==8220==    by 0x400F51: main (in /home/roger/GIT/programacaoparalela/t2/busca_primos/busca_primos)
==8220== 
==8223== 
==8223== HEAP SUMMARY:
==8223==     in use at exit: 40,000,000 bytes in 1 blocks
==8223==   total heap usage: 5 allocs, 4 frees, 40,008,620 bytes allocated
==8223== 
==8223== LEAK SUMMARY:
==8223==    definitely lost: 0 bytes in 0 blocks
==8223==    indirectly lost: 0 bytes in 0 blocks
==8223==      possibly lost: 0 bytes in 0 blocks
==8223==    still reachable: 40,000,000 bytes in 1 blocks
==8223==         suppressed: 0 bytes in 0 blocks
==8223== Rerun with --leak-check=full to see details of leaked memory
==8223== 
==8223== For counts of detected and suppressed errors, rerun with: -v
==8223== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
Processo PID: 8223 -> 1681425 numeros primos!

Tempo de execução: 12.054167 seg.

==8220== 
==8220== Process terminating with default action of signal 27 (SIGPROF)
==8220==    at 0x54C799E: write_gmon (gmon.c:350)
==8220==    by 0x54C8119: _mcleanup (gmon.c:418)
==8220==    by 0x53ECEBF: __run_exit_handlers (exit.c:83)
==8220==    by 0x53ECF19: exit (exit.c:105)
==8220==    by 0x53D21C7: (below main) (libc-start.c:342)
==8220== 
==8220== HEAP SUMMARY:
==8220==     in use at exit: 7,316 bytes in 1 blocks
==8220==   total heap usage: 8 allocs, 7 frees, 40,013,276 bytes allocated
==8220== 
==8220== LEAK SUMMARY:
==8220==    definitely lost: 0 bytes in 0 blocks
==8220==    indirectly lost: 0 bytes in 0 blocks
==8220==      possibly lost: 0 bytes in 0 blocks
==8220==    still reachable: 7,316 bytes in 1 blocks
==8220==         suppressed: 0 bytes in 0 blocks
==8220== Rerun with --leak-check=full to see details of leaked memory
==8220== 
==8220== For counts of detected and suppressed errors, rerun with: -v
==8220== Use --track-origins=yes to see where uninitialised values come from
==8220== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

# Observações
Tentei utilizar também o profile Gperftools, porém para instalar era preciso compilar e estava acusando um erro de biblioteca que não consegui resolvaer.









