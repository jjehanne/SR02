#include <stdio.h>
#include <fcntl.h>

void main() {

	int tab2[10], i, fd;
	
	/* On commence par ouvrir, en testant la réussite de l'ouverture, le fichier titi.dat en ecriture/lecture
	avec les permissions adéquates (0666) */
	
    if ((fd=open("titi.dat",O_RDWR,0666)) == -1){
    	perror("open() failed");
    	exit(-1);
    }
    
    //On lit les données que l'on place dans tab2
    
    if ((read (fd,tab2,10*sizeof(int))) == -1){
    	perror("read() failed");
    	exit(-1);
    }
    
    // on ferme le descripteur du fichier
	if ((close(fd)) == -1){
    	perror("close() failed");
    	exit(-1);
    }
    
    //on imprime les valeurs contenue dans tab2
    for (i=0;i<10;i++) printf("%d\n",tab2[i]);
}
