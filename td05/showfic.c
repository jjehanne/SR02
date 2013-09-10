#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv){
	
	int fd, i, test = 0;
	int * adr;
	
	/* On commence par ouvrir, en testant la réussite de l'ouverture, le fichier titi.dat en ecriture/lecture avec les permissions adéquates (0666) */
	if ((fd=open("titi.dat", O_RDWR)) == -1){
		perror("Erreur lors de l'ouverture de titi.dat");
		exit(-1);
	}
	
	//on mappe
	adr = (int *)mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	//Vérification que le mapping s'est bien passé
	if (adr==MAP_FAILED){
		perror("Erreur lors du mmap");
		exit(-1);
	}
	
	// boucle infinie
	while(1){
	
		printf("Veuillez entrez la valeur de i (99 pour sortir): \n");
		scanf("%d", &test);
	
		if(test == 99){
			// Cas de sortie du programme
			exit(0);
		}
		else{
			//On affiche les élements de la projection mémoire/espace virtuel partagé

			printf("\n");
			
			for (i = 0; i<10; i++){
			
				printf("%d\n", adr[i]);
				
			}
		}
	}
	return 0;
}
