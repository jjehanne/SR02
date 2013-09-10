#include "sharemem1.h"


int main(int argc, char* argv[]){
    int i, shmid,j;
    int* ptr;
    key_t clef;
    int tab_rcv[5]={};

    // Creation et attachement d'un segment mémoire
    // On crée une clef unique
    clef = ftok("sr02p009.h",10);

    // On crée le segment a partir de la clef
    shmid = shmget(clef, 100, IPC_CREAT | 0600 );

    //on  attache ce segment à l'espace virtuel du processus
    ptr = shmat(shmid, 0, 0);

    //attend 1/2s
    usleep(500); 
   
   printf("\n");
    
    // Affichage de l'adresse du premier pointeur du tableau de pointeurs.
    printf("(shrptr) Adresse du premier élément du segment de mémoire partagée (&ptr[0]), différent de celui de shrpte : %d. Equivaut à l'adresse de la valeur du premier élément du tableau de pointeurs.\n", &ptr[0]);
    printf("\n");
    
    // Le shrptr copie les pointeurs relatifs depuis le segment de mémoire partagé
    for(i=0 ; i<5 ; i++)
    {
    	tab_rcv[i] = ptr[i];
    }
    printf("\n");
    
    // Affichage du contenu du tableau de pointeurs. Les adresses sont relatives, et représentent les déplacements depuis le début du segment de mémoire partagé.
	for(i=0;i<5;i++)
  	{	
  		printf("Pointeur n°%d : %d\n", i, tab_rcv[i]); 
  	}
  	printf("\n");
    
    // Affichage du premier élément des vecteurs
  	printf("Premier élément du premier vecteur d'entiers : %d\n", *(&ptr[0]+tab_rcv[0]));
  	printf("\n");
  	
  	// Affichage de l'ensemble du contenu des vecteurs
    for(j=0 ; j<5 ; j++)
    {
    	for(i=0 ; i<3 ; i++)
    	{
    		printf("tab%d[%d] : %d\n", j+1, i, *((ptr+tab_rcv[j])+i));  //ptr ou &ptr[0]
    	}
    	printf("\n");
    }   
  	
    printf("\n\n");
    
    // on effectue le détachement du segment
    shmdt(ptr);
    

    return 0;
}
