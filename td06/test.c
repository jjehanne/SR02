#include "sem_pv.h"

int main(){
	
	init_semaphore();

	printf("Initialisation du 2ème sémaphore à 1\n");
	val_sem(2,1);
	
	
	printf("P(2)\n");
	P(2);
	
	sleep(20);
	
	printf("V(2)\n");
	V(2);
	
	detruire_semaphore();
	
	return 0;
}

