#include <pthread.h>
#include <signal.h>
#include <sys/signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define nth 3 /* nbre de threads a lancer */

#define ifer(is,mess) if (is==-1) perror(mess)

pthread_t threads[nth];

pthread_t thsig;
int count =0;

void *th_fonc (void *arg) {

    int is, numero, i;
    
    numero = (int) arg;
    
    while(count < 80){
    	sleep(1);
    	printf("Thread : %d \n", numero);
    	count++;
    }
    
}

void *th_sig (void *arg) {
	int is, i, numero;
	
	sigset_t masque; /* accepter certains signaux */
	
	sigemptyset(&masque); /*vide*/
	sigaddset(&masque, SIGINT); /* accepter Ctrl + C */
	sigaddset(&masque, SIGQUIT); /* accepter Ctrl + \ */
	
	while(1){
		sigwait(&masque, &numero);
		switch(numero){
			case SIGINT :
				printf("reception de SIGINT \n");
				break;
			case SIGQUIT:
				printf("reception de SIGQUIT \n");
				break;
		}
	}
}

int init(){
	int is,i;
	
	
    /* creer le thread signaux */
	is = pthread_create(&thsig, NULL, th_sig, NULL);
	ifer (is,"err. création thread signaux");
	
	/*masque */
	
	sigset_t masque;
    
    sigfillset(&masque); /*inclure tous les signaux */
    
    pthread_sigmask (SIG_BLOCK, &masque, NULL); /* les bloquer  */
    
	/* créer les threads */

    for(i=0; i<nth; i++) {
        printf("ici main, création thread %d\n",i);
        is = pthread_create( &threads[i], NULL, th_fonc, (void *)i );
        ifer (is,"err. création thread");
    }
	

}

main(){
	int is,i;
	void *val=0;
	init();
	
	for(i=0; i<nth; i++) {
    	is = pthread_join( threads[i], &val);
   		ifer (is,"err. join thread");
    	printf("ici main, fin thread %d\n",(int)val);
    }

}
