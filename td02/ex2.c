#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

struct sigaction act_fils, act_pere;
pid_t pid;
int j = 1;
int lettref = 'a', lettrep = 'A';

void handler_fils(int x){
	int i = 0;
	for (i ; i<j ; i++){
		printf("%c", lettref);
		if (lettref == 'z'){
			fflush(stdout);
			kill(getppid(), SIGUSR1);
			return ;
		}
		lettref++;	
	} 
	fflush(stdout);
	j++;

	kill(getppid(), SIGUSR1);
}

void handler_pere(int x){
	int i = 0;
	for (i ; i<j ; i++){
		printf("%c", lettrep);
		if (lettrep == 'Z'){
			fflush(stdout);
			return ;
		}
		lettrep++;
	}
	fflush(stdout);
	j++;
	
	kill(pid,SIGUSR1);
}

void main(){
	pid=fork();
	if(pid==0){
		/*processus fils: lettre minuscule*/
		act_fils.sa_handler=handler_fils;
		sigaction(SIGUSR1,&act_fils,NULL);
		while(1){
			pause();
		}
	}else{
		/*processus pere: lettre majuscule*/
		act_pere.sa_handler=handler_pere;
		sigaction(SIGUSR1,&act_pere,NULL);
		sleep(1);
		kill(pid, SIGUSR1);

		while(1){
			pause();
		}
	}
}
