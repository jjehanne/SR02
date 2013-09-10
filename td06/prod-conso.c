#include "sem_pv.h"
#include <sys/signal.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> 

static int index_w=0, index_r=0;

int rand_a_b(int a, int b){
    return rand()%(b-a) +a;
}

int main(){
	int shmid, semid;
	int* ptr;
	int i = 0, j=0;

    // Creation et attachement d'un segment mémoire
    // On crée le segment en utilisant IPC_PRIVATE car le segment de mémoire est partagé entre un processus père et un processus fils
    shmid = shmget(IPC_PRIVATE, 100, IPC_CREAT | 0600 );
    
    
    
    // Création du sémaphore
    init_semaphore();
    
    // Initialisation des sémaphores à 5 (places libres ==> 1) et à 0 (places occupées ==> 2)
    val_sem(1,5);
    val_sem(2,0);

	 // Création du processus fils
    pid_t pidChild = fork();
    if (pidChild < 0) {
        // fork failed.
        perror("Fork of pidChild failed.");
        return -1;
    } 
    else if (pidChild == 0) {
        // Processus fils

        //on  attache ce segment à l'espace virtuel du processus
   		ptr = shmat(shmid, 0, 0);

		//on initialise les valeurs du segment de mémoire partagée à 0
   		for (i=0; i <  5; i++) ptr[i] = 0;

        while(j<100){  // 100 itérations puis sortie du programme avec suppression du groupe de sémaphores

			// P(Nombre de place libres) car on produit une entrée
		    P(1);

		    usleep(rand_a_b(20, 100)); // On montre avec le sleep qui la production/consommation se fait au fur et à mesure

		    ptr[index_w] = rand_a_b(1, 50); // Un nombre au hasard entre 1 et 50 à la case 'index_w'

		    index_w = (index_w + 1)%5; // On augmente l'index et on utilise modulo 5 car segment circulaire

			// Affichage
		    printf("valeurs émises : ");

		    for(i=0 ; i<5 ; i++)
		    {
		    	printf("%d ", ptr[i]);
		    }
		    printf("\n");
			
			// V(Places occupées) car on a produit une entrée
		    V(2);
		    j++;
		}
		// A la fin des 100 itérations, on détache le segment.
		shmdt(ptr);
    }
        
    else {
		//processus père

		//on  attache ce segment à l'espace virtuel du processus
   		ptr = shmat(shmid, 0, 0);

		int val_retour;

		// On fait 100 itérations (arbitraire)
		while(j<100)
        {
			//P(place occupées) car on va consommer une entrée
			P(2);

			usleep(rand_a_b(20, 100)); // On montre avec le sleep qui la production/consommation se fait au fur et à mesure

       		val_retour = ptr[index_r]; // On lit la valeur a l'index index_r

       		index_r = (index_r + 1) %5; // On augmente l'index et on utilise modulo 5 car segment circulaire de 5 cases

       		// affichage 
      		printf("valeur de retour : %d\n", val_retour);
      	 	printf("\n");
       		
			// V(Place libres) car on a consommé une entrée
			V(1);
        	j++;
		}

		//On attend la fin du processus fils
		if (waitpid(pidChild, 0, 0) == -1) {
            // Si echec du waitpid, envoie d'un signal d'interruption
            kill(pidChild, SIGKILL);
            perror("Wait child failed");
            return -1;
    	}
		// Détachement du segment
		shmdt(ptr);
		// Suppression du segment de mémoire partagée
   		shmctl(shmid, IPC_RMID, 0);

		//Suppression du groupe de semaphore
		detruire_semaphore();
	}

}
