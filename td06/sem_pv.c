#include "sem_pv.h"

static int semid = -1;

union semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
};


int init_semaphore(void){
	// Si le sémaphore est positif, alors il a déjà été créé. On doit donc sortir de la fonction
	if (semid > -1)
		return -1;

	//sinon on le crée
	semid = semget(IPC_PRIVATE, N_SEM, 0600);
	
	if (semid > 0)
	{	
		// initialisation de l'union semun
		union semun valeur;
		// on veut initialiser tous les sémaphores à 0
		ushort mesValeurs[N_SEM] = {0, 0, 0, 0, 0};
		valeur.array = mesValeurs;
		
		// On initialise effectivement les N_SEM premiers sémaphores (donc tous) du groupe de sémaphores semid à 0 (valeur.array)
		semctl(semid, N_SEM, SETALL, valeur.array);
		printf("Création du sémaphore effectuée\n");
		return 0;
	} 
	// si semid vaut toujours -1, alors erreur lors de la création du groupe de sémaphore
	else if (semid == -1)
	{
		fprintf(stderr, "Erreur init_semaphore: semget\n");
	 	return -2;
	}

}

int detruire_semaphore(){
	// si semid vaut -1, soit init_semaphore n'a pas été appelée, soit il y a eu une erreur => Le groupe "semid" n'existe donc pas
	if (semid == -1){
		fprintf(stderr, "Erreur detruire_semaphore : Sémaphore non créé\n");
		return -1;
	}
	//si le groupe existe, on le détruit
	else{
		printf("Destruction du sémaphore\n");
		return semctl(semid,0, IPC_RMID);
	}
}

int val_sem(int sem, int val_sem){
	// si semid vaut -1, soit init_semaphore n'a pas été appelée, soit il y a eu une erreur => Le groupe "semid" n'existe donc pas
	if (semid == -1){
		fprintf(stderr, "Erreur val_sem : Sémaphore non créé\n");
		return -1;
	}
	
	// on initialise une union pour attribuer une valeur à un sémaphore
	union semun value;
	value.val = val_sem;
	
	int retCtl = semctl(semid, sem, SETVAL, value.val);
	
	// si la valeur retournée est -1, il y a eu une erreur
	if (retCtl == -1){
		fprintf(stderr, "Erreur val_sem : SETVAL\n");
		return -2;
	}
	else{
		//sinon, c'est bon le sémaphore sem a été initialisé à val_sem
		printf("Valeur de semid[%d]: %d\n",sem, semctl(semid, sem, GETVAL));
		return retCtl;
	}
}

int P(int sem){
	// si semid vaut -1, soit init_semaphore n'a pas été appelée, soit il y a eu une erreur => Le groupe "semid" n'existe donc pas
	if (semid == -1){
		fprintf(stderr, "Erreur P : Sémaphore non créé\n");
		return -1;
	}
	
	while (semctl(semid, sem, GETVAL) == 0); // Attente que la valeur du sémaphore sem soit positive

	//on initialise sembuf avec sem_op = -1 , négatif pour faire un P
	struct sembuf op;
	op.sem_num = sem;
	op.sem_op = -1;
	op.sem_flg = 0;
	
	int retOp = semop(semid, &op, 1);
	if (retOp == -1){
		// si la valeur retournée est -1, il y a eu une erreur
		fprintf(stderr, "Erreur P : semop\n");
		return -2;
	}
	else{
		//sinon c'est bon, on retourne la valeur de retour de semop
		//printf("Valeur de semid[%d]: %d\n",sem, semctl(semid, sem, GETVAL));
		return retOp;
	}
}

int V(int sem){
	// si semid vaut -1, soit init_semaphore n'a pas été appelée, soit il y a eu une erreur => Le groupe "semid" n'existe donc pas
	if (semid == -1){
		fprintf(stderr, "Erreur V : Sémaphore non créé\n");
		return -1;
	}
	
	//on initialise sembuf avec sem_op = 1 , positif pour faire un V
	struct sembuf op;
	op.sem_num = sem;
	op.sem_op = 1;
	op.sem_flg = 0;
	
	int retOp = semop(semid, &op, 1);  // Si pas &op dans semop mais struct sembuf *op, faire une initialisation op[1] et après, op[0].sem_num = etc...
	
	if (retOp == -1){
		// si la valeur retournée est -1, il y a eu une erreur
		fprintf(stderr, "Erreur V : semop\n");
		return -2;
	}
	
	else{
		//sinon c'est bon, on retourne la valeur de retour de semop
		//printf("Valeur de semid[%d]: %d\n",sem, semctl(semid, sem, GETVAL));
		return retOp;
	}
}


