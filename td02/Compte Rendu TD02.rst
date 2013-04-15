COMPTE RENDU du TD n�2 de SR02
==============================

Exercice n�1
------------
Cet exercice consiste � �tudier un code, dont l'objectif est de faire communiquer deux processus. Le premier, le p�re, se sert de l'instruction � alarm � pour s'envoyer un signal, qui d�clenche une routine. Dans cette routine, un autre signal est envoy�, mais au fils, cette fois. Le fils affiche une valeur tir�e au hasard, et se met en pause, attendant le prochain signal,

Le main se compose de la fa�on suivante : on effectue un fork(), qui permet la cr�ation du processus fils. Le processus fils, une fois cr��, se rend r�ceptif au signal � SIGUSR1 � gr�ce � l'instruction � sigaction �. Recevoir ce signal d�clenche la routine associ�e, en l'occurence � handler-thermometre �. Dans cette routine, une valeur est tir�e au hasard, puis affich�e. Puis, elle se termine, et on revient au fils, qui se met en pause.
Le processus p�re, lui, se rend r�ceptif au signal � SIGALRM �, via l'instruction � sigaction �. Une fois cela fait, il lance l'instruction � alarm(5) �, qui aura pour effet, au bout de 5 secondes, d'envoyer un signal SIGALRM au processus ayant lanc� l'instruction, en l'occurrence, le processus p�re. Le signal d�clenche la routine � handler-alarme �, Dans celle-ci, on envoie un signal SIGUSR1 au processus fils (dont le pid est pass� en param�tre) via l'instruction � kill �. Puis, on relance une instruction alarm(5). Pendant ces 5 secondes, le processus p�re affiche la barre de t�l�chargement, via un sleep, l'affichage d'un tiret, puis un fflush(stdout).

On obtient ainsi le fonctionnement d�sir� : une prise de temp�rature fictive toutes les 5 secondes, les prises �tant temporis�es gr�ce � alarm, et le processus fils sachant qu'il doit en effectuer une gr�ce au signal qu'il re�oit, signal envoy� par le processus p�re.

Exercice n�2
------------

Dans cet exercice il s'agit de faire communiquer un processus p�re et un processus fils par le biais de signaux.
Le processus fils va commencer par afficher la premi�re lettre de l'alphabet en minuscule puis signifier au processus p�re que son travail est termin�. Le processus p�re va ensuite affich� la premi�re lettre de l'alphabet en majuscule cette fois puis envoyer un signal au fils qui va alors afficher les deux lettres suivantes, toujours en minuscule. Ces �changes vont continuer jusqu'� ce que l'alphabet soit �crit en double, en alternant minuscules et majuscules et en affichant � chaque �tape un caract�re de plus.

Nous devons donc avoir aAbcBCdefDEFghijGHIJklmnoKLMNOpqrstuPQRSTUvwxyzVWXYZ

L'id�e est de commencer avec un processus fils en pause et un processus p�re qui va envoyer un signal au fils , puis se mettre en pause. Cet envoi de signal constituera le point de d�part de tout le programme.

Une fois le travail du fils termin�, il enverra un signal au p�re qui va � son tour effectuer son travail et renvoyer un signal au fils pour lui signifier � son tour qu'il a termin� et que le fils peut continuer sa partie du travail.

La routine de traitement du SIGUSR1 est presque la m�me pour le p�re et le fils, � l'exception pr�s que l'on va commencer avec un 'a' pour le fils , et 'A' pour le p�re.

On �tablit un compteur dans une variable globale pour savoir combien de caract�re on doit afficher.
On �tablit aussi des variables globales pour les caract�res � afficher de sorte que l'on sache d'o� reprendre d'une ex�cution � une autre de la routine.
Il faut donc bien penser � incr�menter les variable lettrep et lettref (lettre p�re et lettre fils).
Enfin une condition d'arr�t est n�cessaire car l'alphabet n'est pas infini.

Lorsqu'on arrive a la lettre 'z' chez le fils, on affiche le buffer, et on envoie un signal via la primitive kill() pour annoncer au p�re que le travail est fini, puis on sort de la fonction.
La routine de traitement du p�re s�ex�cutera une derni�re fois et lorsqu'on arrive � la lettre 'Z', on vide le buffer sur la sortie standard, on n'envoie pas, cette fois, de signal au processus fils et on sort de la fonction.


Probl�mes rencontr�s:

Notre printf ne marchait que si l'on mettait un '\n'. Apr�s quelques recherches nous nous sommes rendus compte que le '\n' imposait au buffer de se vider.
Nous avons donc ajout� un fflush(stdout)


Exercice n�3
------------

Le but de cet exercice est de tester l'accessibilit� en lecture d'adresses. Pour se faire, on d�finit un tableau de 1000 entiers, que l'on parcourt. Pour chaque � case � du tableau, on teste l'adresse via la fonction probe. Si i renvoie 0, c'est qu'on a rencontr� une violation d'acc�s m�moire. Sinon, on incr�mente la variable li�e au parcours du tableau et on continue ce parcours.
Notons qu'au d�but du main, le processus se rend r�ceptif au signal SIGSEGV.

La fonction probe fonctionne de la fa�on suivante : elle re�oit une adresse, et stocke dans statut la valeur de retour de la fonction setjmp. Setjmp permet d'effectuer une sauvegarde du contexte, et renvoie 0 si l'on � revient � du setjmp directement (et donc, pas via un longjmp). Si c'est le cas, cela signifie qu'on a fait la sauvegarde, et qu'on a pas rencontr� de probl�me. On peut alors afficher la valeur contenu dans l'adresse (qui est accessible en lecture).

Si, � la lecture, on effectue une violation d'acc�s m�moire, un signal SIGSEGV sera envoy�. Le processus �tant r�ceptif � ce signal, il va d�clencher la routine associ�e (ici, handler-segv). On affiche alors un message pr�cisant qu'on a re�u pareil signal, et on effectue un longjmp, qui nous permet de revenir au dernier setjmp effectu�, avec pour valeur retour de ce dernier la valeur pass�e en param�tre du longjmp (ici, 1). On se retrouve alors dans l'�tat du dernier setjmp, � l'unique diff�rence que la variable statut contient cette fois 1, et non 0, comme pour tous les cas ou on est � revenu � directement du setjmp. Le test de la variable statut d�clenche l'instruction return 0, on retourne alors dans le main, o� l'on test cette valeur (attribu�e � la variable i). Elle vaut 0, on affiche donc un message d'erreur, et on met fin au programme.
