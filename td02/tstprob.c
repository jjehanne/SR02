#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <sys/types.h>
#include <signal.h>

jmp_buf jb;
struct sigaction act_probe;

int probe (int *adr)
{
	int statut = setjmp(jb);
	if (statut != 0)
		return 0;
	else {
		printf("\nvaleur = %d", *adr);
		return 1;
		}
}

void handler_segv ()
{
	printf ("\nSIGSEGV recu\n");
	fflush (stdout);
	longjmp (jb, 1);
}


int main()
{
	int tab[1000], index, *ia1, i;
        act_probe.sa_handler=handler_segv;
        sigaction(SIGSEGV,&act_probe,NULL);
	index = 0;
	while(1) {
	ia1 = &tab[index];
	usleep(500);
	i = probe (ia1);
	if (i==0) { 
    		printf("probe fail at index=%d ia1=%x\n",index,ia1);
    		break; }
	index++;
}
	
	return 0;
}
