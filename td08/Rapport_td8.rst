Rapport SR02 du TD n�8 - Threads
================================

Exercice 1 et 2
---------------

Les exercices n�1 et n�2 consistent en une simple ex�cution du code. Celle-ci a �t� effectu�e, et nous avons pu observer le fonctionnement de chacun d'entre eux, qu'il a ensuite fallu associer dans les exercices suivants (n�3 et n�4).

Exercice n�3 : thx.c
--------------------

Le but de cet exercice est la cr�ation d'un programme thx.c, dans lequel 3 threads sont ex�cut�s. A chacun des threads, une barre de progression est associ�e, repr�sent�e par un rectangle se remplissant au fur et � mesure. Les trois barres de progression peuvent avancer � des vitesses diff�rentes.
Pour cela, on effectue d'abord l'initialisation d'un mutex, qui servira � assurer les exclusions mutuelles n�cessaires dans l'ensemble du programme (notamment l'utilisation des diff�rentes fonctions de cr�ation de rectangle, qui s'av�rent �tre des fonctions non r�-entrantes, et n�cessitent donc l'utilisation de mutex pour �viter les erreurs si plusieurs threads acc�dent simultan�ment � ces fonctions).
Dans le main, on cr�e le rectangle rouge, qui sert de fond, puis on cr�e l'ensemble des threads (ici, 3) via la fonction pthread_create(). C'est � cet endroit que l'on d�finit que le comportement des threads sera la fonction th_fonc.
Toujours dans le main, on attend la fin de l'ex�cution des threads, via autant d'appels � la fonction pthread_join que de threads.

Une fois cette � structure � de cr�ation/destruction de threads mise en place, il reste encore � d�finir ce que les threads vont effectuer. Cela se fait dans th_fonc. Dans cette fonction, on d�finit trois cas, pour chacun des threads (on les identifie par un num�ro, entre 0 et 2), chacun des cas du switch repr�sentant un thread, et donc, l'avanc�e d'une barre de progression.
Dans chacun des cas, on effectue la cr�ation du rectangle (drawrec) repr�sentant la barre, ainsi que la cha�ne de caract�res (drawstr) � c�t� de la barre. Ces deux fonctions ne sont pas r�-entrantes, et par cons�quent, il est n�cessaire de locker un mutex avant leur utilisation, et d'effectuer un unlock une fois que l'appel � ces fonctions est fait. Suite � cela, on remplit les trois barres de progression cr��es (fillrec) gr�ce � une boucle for. L� encore, la fonction n�cessite l'utilisation d'un mutex, pour �viter que plusieurs threads n'effectuent fillrec simultan�ment. En mettant des usleep de longueurs diff�rentes dans chaque threads, on peut obtenir des barres de progression se remplissant � des vitesses diff�rentes.

On a ici le cas de trois threads s'ex�cutant en parall�le. Il est n�cessaire de g�rer les �ventuelles � lieux � de conflits, via des exclusions mutuelles.


Exercice n�4 : Thread avec rendez-vous
---------------------------------------

Cette partie du TD consiste � impl�menter du code afin de bloquer les threads ayant fait progresser la barre 10 fois, et ce jusqu'� ce que le dernier thread ait fait progresser la barre 10 fois. A ce moment, les threads se d�bloquent et finissent leur ex�cution.

Pour se faire nous devons impl�menter une fonction de rendez-vous nomm�e th_rdv() qui utilisera une condition pthread_cond_t rdv ainsi qu'un autre mutex qu'on appelera mutex2 afin de prot�ger la variable partag�e count.

Le principe est le suivant : Si un thread a atteint une progression de 10 il augmente la variable count et se met en attente du signal rdv. Lorsque le 3�me thread atteint une progression de 10, la variable count passe � 3 ce qui permet de v�rifier la condition qui enverra un signal pour d�bloquer les deux autres threads.
L'envoi du signal peut se faire de deux fa�ons. Soit avec un broadcast (pthread_cond_broadcast) qui envoie un signal � tous les threads en attente, soit en les d�bloquant en cascade avec  pthread_cond_signal. Chaque thread en se d�bloquant enverra un signal � un autre pour le d�bloquer. Il est pr�f�rable de veiller � ce que le dernier qui se r�veille n'envoie pas de signal inutile. Cela est assur�e par la condition � if(count >1) � qu'il est n�cessaire de remplir pour envoyer ce signal. Si count est inf�rieur ou �gal � 1, cela signifie que le thread actuel est le dernier, et qu'il n'y en a plus d'autres � r�veiller.

1) Avec broadcast

Cette m�thode est la plus simple � impl�menter. Elle consiste � bloquer un mutex pour que la variable partag�e count ne soit utilis�e que par un thread � la fois, ensuite tant que la condition count < 3 n'est pas v�rifi�e, on met le thread en attente avec pthread_cond_wait. Si la condition est v�rifi�e, on effectue pthread_cond_broadcast qui va lib�r� tous les threads d'un coup. On d�bloque � la fin le mutex.

2) Avec signal

Dans la fonction th_rdv de rendez vous, si l'on saute la condition et donc que le thread ne se met pas en attente, c'est que la variable partag�e count vaut 3 et � ce moment on va envoyer un signal � un autre thread et d�cr�menter la variable count. Le thread qui se r�veille poursuit son ex�cution et si
count >1 cela veut dire que il reste des threads � r�veiller. Si la variable count vaut 1 c'est que le dernier thread est r�veill� et il n'est pas n�cessaire d'envoyer un signal (il serait de toute fa�on perdu).

Exercice suppl�mentaire
-----------------------

Nous avons, dans cet exercice, pu mettre en place une structure particuli�re de gestion des signaux avec des threads. Dans le cas g�n�ral, les signaux sont communs � tous les threads, mais pas le masque des signaux. Chaque thread a son propre masque de signaux. Ainsi, de base, tous les threads peuvent �tre perturb�s par des signaux  externe (SIGKILL, SIGINT , ...). On peut n�anmoins masquer, dans un thread en particulier, un ou plusieurs signaux. C'est l�-dessus que s'appuie cet exercice : on souhaite qu'un thread unique soit assign� � la gestion des signaux. Pour cela, on bloque, dans tous les threads, sauf un, tous les signaux. Le thread qui n'a pas bloqu� les signaux est celui qui sera assign� � leur gestion. On peut donc, dans ce thread, bloquer certains signaux, ou les laisser tous recevables. On se met ensuite en attente d'�ventuels signaux, et on boucle sur cette attente (sigwait), pour �tre toujours en mesure de les traiter.

Dans le programme, on commence donc par cr�er le thread � signaux �. Puis, on masque tous les signaux, pour enfin cr�er l'ensemble des autres threads. Tous ces threads, cr��s dans un second temps, ne peuvent recevoir aucun signaux. Ceux-ci d�clenchent la fonction th_fonc, dans laquelle ils affichent un texte. Le thread � signaux �, lui, d�clenche la fonction th_sig, dans laquelle on d�finit les signaux que l'on souhaite recevoir (ici, � Ctrl+C � et � Ctrl+\ �). Et pour s'assurer du bon fonctionnement, � la r�ception d'un de ces deux signaux, on affiche un texte.
Le programme fonctionne correctement, dans la mesure o� l'envoi de signaux ne stoppe aucun thread, mais affiche le � printf � du thread � signaux �. On parvient donc bien, comme souhait�, � une situation dans laquelle un thread est d�di� � la gestion des signaux, et les autres en sont compl�tement coup�s.
