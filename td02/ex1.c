/******************************
/* Auteur: Yacine CHALLAL    */
/* UTC, SR02, P2009          */
/*****************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

struct sigaction act_thermometre, act_alarme;
pid_t pid;

void handler_thermometre(int x){
	int rnd=(random()/(RAND_MAX*1.0))*40;
	printf("\nTemperature = %d C\n",rnd);
}

void handler_alarme(int x){
	kill(pid,SIGUSR1);
	alarm(5);
}

main(){
	pid=fork();
	if(pid==0){
		/*processus fils: thermometre*/
		act_thermometre.sa_handler=handler_thermometre;
		sigaction(SIGUSR1,&act_thermometre,NULL);
		while(1){
			pause();
		}
	}else{
		/*processus pere: mesure périodique*/
		act_alarme.sa_handler=handler_alarme;
		sigaction(SIGALRM,&act_alarme,NULL);
		alarm(5);
		while(1){
			sleep(1);
			printf("-");
			fflush(stdout);
		}
	}
}
