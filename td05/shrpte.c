#include "sharemem1.h"

void printTab(int tab[], int size){
  		int i;
  		for(i=0; i<size; i++)
   			printf("tab[%d] = %d\n", i, tab[i]);
	}

int main(int argc, char* argv[]){
    int i, shmid, j;
    int* ptr;
    key_t clef;

	printf("\n");
	
    // Creation et attachement d'un segment mémoire
    // On crée une clef unique
    if((clef = ftok("sr02p009.h",10)) == -1){
    	perror("ftok() failed");
    	exit(-1);
    }

    // On crée le segment a partir de la clef
    if((shmid = shmget(clef, 100, IPC_CREAT | 0600 )) == -1){
    	perror("shmget() failed");
    	exit(-1);
    }
    //on  attache ce segment à l'espace virtuel du processus
    if((ptr = shmat(shmid, 0, 0)) == -1){
    	perror("shmat() failed");
    	exit(-1);
    }

	// on crée dans le segment de mémoire partagé les 5 vecteurs de 3 entiers (de 5 à 19, donc 15 entiers tous différents)
 	for (i = 5; i < 20; i++){
  		ptr[i] = i;
  	}
  	
  	// on crée le tableau de pointeurs. L'adresse de début de segment n'étant pas la même dans les deux processus, on met des adresses relatives (déplacement depuis la première case du segment). Ici, le premier élément du premeir vecteur est à l'adresse &ptr[0] + 5, le second à &ptr[0]+8 etc.
  	for(i=0;i<5;i++)
  	{	
  		j = 5 + 3*i;
  		ptr[i] = j;
  	}
  	
  	// Affichage de l'adresse du premier pointeur du tableau de pointeurs.
  	printf("(shrpte) Adresse du premier élément du segment de mémoire partagée (&ptr[0]) : %d. Equivaut à l'adresse de la valeur du premier élément du tableau de pointeurs.\n", &ptr[0]);
  	printf("\n");
	
	// Affichage du contenu du tableau de pointeurs. Les adresses sont relatives, et représentent les déplacements depuis le début du segment de mémoire partagé.
	for(i=0;i<5;i++)
  	{	
  		printf("Pointeur n°%d : %d\n", i, ptr[i]); 
  	}
  	printf("\n");
  	
  	// Affichage du premier élément des vecteurs
  	printf("Premier élément du premier vecteur d'entiers : %d\n", ptr[5]);
  	printf("\n");
  	
  	// Affichage de l'ensemble du contenu des vecteurs
  	for(j=0 ; j<5 ; j++)
    {
    	for(i=0 ; i<3 ; i++)
    	{
    		printf("tab%d[%d] : %d\n", j+1, i, *((&ptr[0]+ptr[j])+i));
    	}
    	printf("\n");
    }   
    printf("\n");

	printf("Début du sleep \n");

    sleep(20);

    printf("\n");

	// on effectue le détachement du segment
    if ((shmdt(ptr)) == -1){
    	perror("shmdt() failed");
    	exit(-1);
    }
    
    // Suppression du segment de mémoire partagé
	if ((shmctl(shmid, IPC_RMID, 0)) == -1){
    	perror("shmctl() failed");
    	exit(-1);
    }

    return 0;
}
