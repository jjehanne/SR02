#include "sem_pv.h"
#include <sys/signal.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> 

int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}

int main(){
	int shmid;
	int* ptr;
	int i = 0, A=0;
	
	/*
	init_semaphore();
	printf("Initialisation du 2ème sémaphore à 1\n");
	val_sem(2,1);
	printf("P(2)\n");
	P(2);
	sleep(20);
	printf("V(2)\n");
	V(2);
	detruire_semaphore();
	*/
	
	// Creation et attachement d'un segment mémoire
    // On crée le segment en utilisant IPC_PRIVATE car le segment de mémoire est partagé entre un processus père et un processus fils
    shmid = shmget(IPC_PRIVATE, 100, IPC_CREAT | 0600 );

    //on  attache ce segment à l'espace virtuel du processus
    ptr = shmat(shmid, 0, 0);
    
    // Création du sémaphore
    init_semaphore();
    
    // Initialisation du premier sémaphore à 1 (mutex)
    val_sem(1,1);
    
    // On initialise l'entier E à 0 en mémoire partagée
    ptr[0] = 0;

    // Création du processus fils
    pid_t pidChild = fork();
    if (pidChild < 0) {
        // fork failed.
        perror("Fork of pidChild failed.");
        return -1;
    } 
    else if (pidChild == 0) {
        // Processus fils
        for(i = 0 ; i<100 ; i++){
        P(1);
	    A = ptr[0];
	    //attendre entre 20 et 100ms (Utilisez les fonctions usleep(3) et rand(3V))
	    usleep(rand_a_b(20, 100));
	    A++;
	    ptr[0] = A;
		V(1);
		usleep(rand_a_b(20, 100));
		}
		shmdt(ptr);
    }
    else {
		//processus père
		for(i = 0 ; i<100 ; i++){
		P(1);
		A = ptr[0];
	    //attendre entre 20 et 100ms (Utilisez les fonctions usleep(3) et rand(3V))
	    usleep(rand_a_b(20, 100));
	    A++;
	    ptr[0] = A;
	    V(1);
	    usleep(rand_a_b(20, 100));
		printf("%dème valeur de E : %d\n", i, ptr[0]);
		}
		
		//On attend la fin du processus fils
		if (waitpid(pidChild, 0, 0) == -1) {
            // Si echec du waitpid, envoie d'un signal d'interruption
            kill(pidChild, SIGKILL);
            perror("Wait child failed");
            return -1;
    	}
    	printf("valeur finale de E : %d\n", ptr[0]);
		detruire_semaphore();
		// on effectue le détachement du segment
		shmdt(ptr);
		// Suppression du segment de mémoire partagée
   		shmctl(shmid, IPC_RMID, 0);
    }

    return 0;
}
