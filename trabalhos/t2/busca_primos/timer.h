#include <time.h>

/*
* Retorna a diferença entre 2 tempos
*/
double timedif(struct timespec tIni, struct timespec tFim){
	return ((double)tFim.tv_sec - tIni.tv_sec) + ((double)(tFim.tv_nsec-tIni.tv_nsec) * 1e-9);
}

/*
*Estrutura que guarda o tempo inicial e o final 
*/
typedef struct timer{
	struct timespec tIni, tFim;
}Timer;

/*
* Inicia a contagem do tempo, retorna um ponteiro para a estrutura timer
*/
Timer* init_timer(){
	Timer* timer = (Timer*)malloc(sizeof(Timer));
	clock_gettime(CLOCK_MONOTONIC_RAW, &timer->tIni); 
	return timer;
}

/*
* Encerra o timer, retorna tempo em segundos desde o metodo init
*/
double end_timer(Timer *timer){
	clock_gettime(CLOCK_MONOTONIC_RAW, &timer->tFim); 	
	double d = timedif(timer->tIni, timer->tFim);
	printf("\nTempo de execução: %lf seg.\n\n", d);
	free(timer);
	return d;
}