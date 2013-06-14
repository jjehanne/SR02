/*-------------------------------------------------------*/

/* > gcc -o thrdv_broadcast thrdv_broadcast.c -L/usr/X11R6/lib/ -lpthread */ 

/*-------------------------------------------------------*/

#include <pthread.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define nth 3 /* nbre de threads a lancer */

#define ifer(is,mess) if (is==-1) perror(mess)

pthread_t threads[nth];
char buf[20];

int count=0;
pthread_mutex_t mutex,mutex2;
    
pthread_cond_t rdv;

/* routine exécutée dans les threads */

void rdv_fonc(){
    
    pthread_mutex_lock(&mutex2);
    count++;
    while(count < 3) pthread_cond_wait(&rdv,&mutex2);
	if(count == 3) pthread_cond_broadcast(&rdv);
	pthread_mutex_unlock(&mutex2);
}

void *th_fonc (void *arg) {

    int is, numero, i,j, m1;

	numero = (int)arg;

	m1 = 20 + numero*10;

	i = m1;

	printf("numero= %d, i=%d \n",numero,i);
	
	
	switch(numero)
	{
		case 0 : {	pthread_mutex_lock(&mutex);
		
					drawstr (30,125, "_0_", 3);

					drawrec (100,100,100+m1*10,30);
					
					pthread_mutex_unlock(&mutex);

					for (j=1;j<=m1;j++) {
					    
						if (j==10) rdv_fonc();
						    
						printf("num %d j=%d\n",numero,j);
						
						pthread_mutex_lock(&mutex);
						fillrec (100,102,100+j*10,26,"yellow");
						pthread_mutex_unlock(&mutex);
						
						usleep(500000);
						}

					flushdis ();
					return ( (void *)(numero+100) );
					break;
					}
		
		case 1 : {
					pthread_mutex_lock(&mutex);
					drawstr (30, 175, "_1_", 3);

					drawrec (100,150,100+m1*10,30);
					pthread_mutex_unlock(&mutex);
					
					for (j=1;j<=m1;j++) {

						if (j==10) rdv_fonc();
						printf("num %d j=%d\n",numero,j);
						
						pthread_mutex_lock(&mutex);
						fillrec (100,152,100+j*10,26,"white");
						pthread_mutex_unlock(&mutex);
						
						usleep(700000);
						}
					flushdis ();
					return ( (void *)(numero+100) );
					break;
					}
			
		case 2 : {
					pthread_mutex_lock(&mutex);
					drawstr (30, 225, "_2_", 3);

					drawrec (100,200,100+m1*10,30);
					pthread_mutex_unlock(&mutex);

					for (j=1;j<=m1;j++) {
					    
						if (j==10) rdv_fonc();
						
						printf("num %d j=%d\n",numero,j);
						
						pthread_mutex_lock(&mutex);
						fillrec (100,202,100+j*10,26,"green");
						pthread_mutex_unlock(&mutex);
						
						usleep(300000);
						}
					flushdis ();
					return ( (void *)(numero+100) );
					break;
					}
	}
}

int liretty (char *prompt, char *buffer) {

	int i;
	printf("\n%s",prompt);

	i = scanf ("%s",buffer);

	return strlen(buffer);
}

main() {

    void *val=0;
    
    int nlu, is,i=0;

    is = pthread_mutex_init(&mutex, NULL);
	ifer (is,"err. init thread_mutex");
	
	is = pthread_cond_init (&rdv, NULL);
	ifer (is,"err. init thread_cond");
    
	/* creer rectangle rouge */
	initrec();
	
	/* creer barre RDV */
	pthread_mutex_lock(&mutex);
	
    drawrec (290,50,2,200);
	
	pthread_mutex_unlock(&mutex);

    /* créer les threads */

    for(i=0; i<nth; i++) {
        printf("ici main, création thread %d\n",i);
        is = pthread_create( &threads[i], NULL, th_fonc, (void *)i );
        ifer (is,"err. création thread");
    }

    /* attendre fin des threads */

    for(i=0; i<nth; i++) {
        is = pthread_join( threads[i], &val);
        ifer (is,"err. join thread");
        printf("ici main, fin thread %d\n",(int)val);
    }
	nlu = liretty("sortir ?",buf);

	printf("--fin--\n");

	detruitrec();

	/* detruire la fenetre rectangle */

	exit(EXIT_SUCCESS);
}

