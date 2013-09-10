#include <stdio.h>

#include <fcntl.h>

void main() {

	int tab1[10]={11,22,33,44,55,66,77,88,99,1000};

    int i,fd;
	
	/* On commence par ouvrir, en testant la réussite de l'ouverture, le fichier titi.dat en ecriture/lecture
	 O_CREAT O_TRUNC ? avec les permissions adéquates (0666) */
	 
	if ((fd=open("titi.dat",O_RDWR|O_CREAT|O_TRUNC,0666)) == -1){
    	perror("open() failed");
    	exit(-1);
    }
	// On écrit le contenu de tab1 dans le descripteur du fichier
	if ((write (fd,tab1,10*sizeof(int)))== -1){
    	perror("write() failed");
    	exit(-1);
    }
    
	// on ferme le fichier
	if ((close(fd)) == -1){
    	perror("close() failed");
    	exit(-1);
    }

}
