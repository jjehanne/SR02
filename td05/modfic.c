#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv){
	
	int fd, i = 0;
	int *adr;

	/* On commence par ouvrir, en testant la réussite de l'ouverture, le fichier titi.dat en ecriture/lecture avec les permissions adéquates (0666) */
	if ((fd=open("titi.dat", O_RDWR)) == -1){
		perror("Erreur lors de l'ouverture de titi.dat");
		exit(-1);
	}

	//On mappe
	
	adr = (int *) mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	//Vérification que le mapping s'est bien passé
	if (adr==MAP_FAILED){
		perror("Erreur lors du mmap");
		exit(-1);
	}
	
	while(1){
	
		printf("Veuillez entrez la valeur de i : \n");
		scanf("%d", &i);
	
		if(i == 99){
			// Pour sortir du programme
			exit(0);
		}
		else{
			//On incremente la valeur adr[i] qui correspond au i+1 ième élément du fichier titi.dat
			adr[i]++;
			printf("Nouvelle valeur pour tab2[%d]: %d\n\n",i, adr[i]);
		}
	}

	return 0;
}
