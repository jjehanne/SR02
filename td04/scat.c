#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <time.h>
#define maxsize 5000

double A[maxsize][maxsize], X[maxsize], Y[maxsize], D, S=0;
double B[maxsize][maxsize], C[maxsize][maxsize];



void initaxy(int size){
    int i=0, j=0;
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
            A[i][j] = (double)(1000*i+j);
	    B[j][i] = (double)(1000*i+j);
	    C[i][j] = 0.0;
        }
    }
}


void wrscat(int size)
{
	int fd = open("mat.dat", O_RDWR);
	int i = 0, j = 0;
	if (fd == -1)
	{
	   printf("Echec lors de l'ouverture du fichier (ecriture)");
	   exit(0);
	}
	else
	{
	   struct iovec vect[size];
	   for(j = 0 ; j< size ; j++)
		{
		for(i = 0 ; i<size ; i++)
	  	 {
		     vect[i].iov_base = &A[i][j];
		     vect[i].iov_len = sizeof(double);
	  	 } 
	   writev(fd, vect, size);
	         }
	}
	close(fd);


}

void rdscat(int size)
{
	int fd = open("mat.dat", O_RDONLY);
	int i = 0;
	int test;
	if (fd == -1)
	{
	   printf("Echec lors de l'ouverture du fichier (lecture)");
	   exit(0);
	}
	else
	{
	   struct iovec vect[size];
		for(i=0 ; i<size ; i++)
		   {
			vect[i].iov_base = C[i];
	   		vect[i].iov_len = size*sizeof(double);
		   }
			readv(fd, vect, size);
			   
	}
	close(fd);
} 



void diffbc(int size) 
{
	int i = 0, j = 0;
	for(i = 0 ; i< size ; i++)
	     {
		for(j = 0 ; j<size ; j++)
	  	 {
		     S = S + B[i][j] - C[i][j];
	  	 } 
	     }
	printf("Valeur de S : %.2f\n", S);
}

void affiche_matrice(int size){
    int i=0, j=0;
    printf("Matrice A\n");
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
	    printf("%7.2f ", A[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    printf("\n");
    printf("Matrice B\n");	
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
	    printf("%7.2f ", B[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    printf("\n");
    printf("Matrice C\n");
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
	    printf("%7.2f ", C[i][j]);
        }
        printf("\n");
    }

    printf("Fin Affichage\n");
    printf("\n");
}





int main(int argc,char *argv[]){

    int size = atoi(argv[1]);
    initaxy(size);
    wrscat(size);
    rdscat(size);
    affiche_matrice(size);
    diffbc(size);

    
	

    return 0;
} 
