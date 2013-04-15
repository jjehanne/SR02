#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define maxsize 5000


double A[maxsize][maxsize], X[maxsize], Y[maxsize], D;
double B[maxsize][maxsize], C[maxsize][maxsize];

void initaxy(int size){
    int i=0, j=0;
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
            A[i][j] = (double)(i+j);
            B[i][j] = A[i][j];
            C[i][j] = 0.0;
        }
    }
}

void mulabc(int size){
    int i=0, j=0, k=0;
    double R;
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
            R = 0;
            for(k=0; k<size; k++){
                R = R + A[i][k] * B[k][j];        
            }
            C[i][j] = R;
        }
    }
}

void sumdiac(int size){
    int i=0, j=0;
    double R = 0;
    for(i=0; i<size; i++){
        R = R + C[i][i];
    }
    D = R;
}

void affiche_matrice(int size){
    int i=0, j=0;
    printf("Matrice A\n");
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
            printf("%.2f ", A[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");
    printf("Matrice B\n");    
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
        printf("%.2f ", B[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");
    printf("Matrice C\n");
    for(i=0; i<size; i++){
        for(j=0; j<size;j++){
        printf("%.2f ", C[i][j]);
        }
        printf("\n");
    }

    printf("Fin Affichage\n\n");
}

int main(int argc,char *argv[]){

    int size = atoi(argv[1]);
    initaxy(size);
    mulabc(size);
    //affiche_matrice(size);
    sumdiac(size);
    printf("Somme diagonale: %f\n", D);

    return 0;
} 
