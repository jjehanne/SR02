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

    //attend 1/2s
    usleep(500); 
    // Le shrmem_r affiche tab2[]
    for(i = 0; i < 10; i++){
       printf("shrmem_r - tab2[%d] = %d\n", i, tab2[i]);
    }
	printf("\n\n");
    // Le shrmem_r copie le segment de mémoire partagée dans tab2
    for(i = 0; i < 10; i++){
        tab2[i] = ptr[i];
    }
    // Le shrmem_r affiche tab2[]
    for(i = 0; i < 10; i++){
        printf("shrmem_r - tab2[%d] = %d\n", i, tab2[i]);
    }
    printf("\n\n");
    
    // on effectue le détachement du segment
    shmdt(ptr);
    

    return 0;
}
