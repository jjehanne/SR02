#include "sharemem1.h"

int main(int argc, char* argv[]){
    int tab1[10], tab2[10],i, shmid;
    int* ptr;
    key_t clef;

    // initialisation des 2 tableaux à 1
    for (i=0; i< 10; i++){
        tab1[i] = 1;
		tab2[i] = 1;
    }

    // Creation et attachement d'un segment mémoire
    // On crée une clef unique
    clef = ftok("sr02p009.h",10);

    // On crée le segment a partir de la clef
    shmid = shmget(clef, 100, IPC_CREAT | 0600 );

    //on  attache ce segment à l'espace virtuel du processus
    ptr = shmat(shmid, 0, 0);

    // shrmem_e met tab2[] à 2
    for(i = 0; i < 10; i++){
        tab2[i] = 2;
    }
    printf("\n\n");

    // shrmem_e affiche tab2[]
    for(i = 0; i < 10; i++){
        printf("shrmem_e - tab2[%d] = %d\n", i, tab2[i]);
    }
    // shrmem_e copie tab2 dans le segment de mémoire partagée
    for(i = 0; i < 10; i++){
        ptr[i] = tab2[i];
    }

    sleep(30);
	// on effectue le détachement du segment
    shmdt(ptr);
    
    // Suppression du segment de mémoire partagée
	shmctl(shmid, IPC_RMID, 0);

    return 0;
}
