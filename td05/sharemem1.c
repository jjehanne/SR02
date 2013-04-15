#include "sharemem1.h"

int main(int argc, char* argv[]){
    int tab1[10], tab2[10],i, shmid;
    int* ptr;

    // initialisation des 2 tableaux à 1
    for (i=0; i< 10; i++){
        tab1[i] = 1;
	tab2[i] = 1;
    }

    // Creation et attachement d'un segment mémoire
    // On crée le segment en utilisant IPC_PRIVATE car le segment de mémoire est partagé entre un processus père et un processus fils
    shmid = shmget(IPC_PRIVATE, 100, IPC_CREAT | 0600 );

    //on  attache ce segment à l'espace virtuel du processus
    ptr = shmat(shmid, 0, 0);

    // Création du processus fils
    pid_t pidChild = fork();
    if (pidChild < 0) {
        // fork failed.
        perror("Fork of pidChild failed.");
        return -1;
    } 
    else if (pidChild == 0) {
        // Processus fils
		// Le fils met tab2[] à 2
        for(i = 0; i < 10; i++){
            tab2[i] = 2;
        }
		// Le fils affiche tab2[]
        for(i = 0; i < 10; i++){
            printf("Fils - tab2[%d] = %d\n", i, tab2[i]);
        }
		printf("\n\n");
		// Le fils copie tab2 dans le segment de mémoire partagée
        for(i = 0; i < 10; i++){
            ptr[i] = tab2[i];
        }
	shmdt(ptr);
    }
    else {
		//processus père
		//attend 1/2s
		usleep(500); 
        // Le père affiche tab2[]
        for(i = 0; i < 10; i++){
            printf("Pere - tab2[%d] = %d\n", i, tab2[i]);
        }
		printf("\n\n");
        //attend 1/2s
		usleep(500);
        // Le père copie le segment de mémoire partagée dans tab2
        for(i = 0; i < 10; i++){
            tab2[i] = ptr[i];
        }
		// Le père affiche tab2[]
        for(i = 0; i < 10; i++){
            printf("Pere - tab2[%d] = %d\n", i, tab2[i]);
        }
	printf("\n\n");
	//On attend la fin du processus fils
	if (waitpid(pidChild, 0, 0) == -1) {
            // Si echec du waitpid, envoie d'un signal d'interruption
            kill(pidChild, SIGKILL);
            perror("Wait child failed");
            return -1;
    }
	// on effectue le détachement du segment
	shmdt(ptr);
	// Suppression du segment de mémoire partagée
    shmctl(shmid, IPC_RMID, 0);
    }

    return 0;
}
