Rapport SR02 du TD n°8 - Threads
================================

Exercice 1 et 2
---------------

Les exercices n°1 et n°2 consistent en une simple exécution du code. Celle-ci a été effectuée, et nous avons pu observer le fonctionnement de chacun d'entre eux, qu'il a ensuite fallu associer dans les exercices suivants (n°3 et n°4).

Exercice n°3 : thx.c
--------------------

Le but de cet exercice est la création d'un programme thx.c, dans lequel 3 threads sont exécutés. A chacun des threads, une barre de progression est associée, représentée par un rectangle se remplissant au fur et à mesure. Les trois barres de progression peuvent avancer à des vitesses différentes.
Pour cela, on effectue d'abord l'initialisation d'un mutex, qui servira à assurer les exclusions mutuelles nécessaires dans l'ensemble du programme (notamment l'utilisation des différentes fonctions de création de rectangle, qui s'avèrent être des fonctions non ré-entrantes, et nécessitent donc l'utilisation de mutex pour éviter les erreurs si plusieurs threads accèdent simultanément à ces fonctions).
Dans le main, on crée le rectangle rouge, qui sert de fond, puis on crée l'ensemble des threads (ici, 3) via la fonction pthread_create(). C'est à cet endroit que l'on définit que le comportement des threads sera la fonction th_fonc.
Toujours dans le main, on attend la fin de l'exécution des threads, via autant d'appels à la fonction pthread_join que de threads.

Une fois cette « structure » de création/destruction de threads mise en place, il reste encore à définir ce que les threads vont effectuer. Cela se fait dans th_fonc. Dans cette fonction, on définit trois cas, pour chacun des threads (on les identifie par un numéro, entre 0 et 2), chacun des cas du switch représentant un thread, et donc, l'avancée d'une barre de progression.
Dans chacun des cas, on effectue la création du rectangle (drawrec) représentant la barre, ainsi que la chaîne de caractères (drawstr) à côté de la barre. Ces deux fonctions ne sont pas ré-entrantes, et par conséquent, il est nécessaire de locker un mutex avant leur utilisation, et d'effectuer un unlock une fois que l'appel à ces fonctions est fait. Suite à cela, on remplit les trois barres de progression créées (fillrec) grâce à une boucle for. Là encore, la fonction nécessite l'utilisation d'un mutex, pour éviter que plusieurs threads n'effectuent fillrec simultanément. En mettant des usleep de longueurs différentes dans chaque threads, on peut obtenir des barres de progression se remplissant à des vitesses différentes.

On a ici le cas de trois threads s'exécutant en parallèle. Il est nécessaire de gérer les éventuelles « lieux » de conflits, via des exclusions mutuelles.


Exercice n°4 : Thread avec rendez-vous
---------------------------------------

Cette partie du TD consiste à implémenter du code afin de bloquer les threads ayant fait progresser la barre 10 fois, et ce jusqu'à ce que le dernier thread ait fait progresser la barre 10 fois. A ce moment, les threads se débloquent et finissent leur exécution.

Pour se faire nous devons implémenter une fonction de rendez-vous nommée th_rdv() qui utilisera une condition pthread_cond_t rdv ainsi qu'un autre mutex qu'on appelera mutex2 afin de protéger la variable partagée count.

Le principe est le suivant : Si un thread a atteint une progression de 10 il augmente la variable count et se met en attente du signal rdv. Lorsque le 3ème thread atteint une progression de 10, la variable count passe à 3 ce qui permet de vérifier la condition qui enverra un signal pour débloquer les deux autres threads.
L'envoi du signal peut se faire de deux façons. Soit avec un broadcast (pthread_cond_broadcast) qui envoie un signal à tous les threads en attente, soit en les débloquant en cascade avec  pthread_cond_signal. Chaque thread en se débloquant enverra un signal à un autre pour le débloquer. Il est préférable de veiller à ce que le dernier qui se réveille n'envoie pas de signal inutile. Cela est assurée par la condition « if(count >1) » qu'il est nécessaire de remplir pour envoyer ce signal. Si count est inférieur ou égal à 1, cela signifie que le thread actuel est le dernier, et qu'il n'y en a plus d'autres à réveiller.

1) Avec broadcast

Cette méthode est la plus simple à implémenter. Elle consiste à bloquer un mutex pour que la variable partagée count ne soit utilisée que par un thread à la fois, ensuite tant que la condition count < 3 n'est pas vérifiée, on met le thread en attente avec pthread_cond_wait. Si la condition est vérifiée, on effectue pthread_cond_broadcast qui va libéré tous les threads d'un coup. On débloque à la fin le mutex.

2) Avec signal

Dans la fonction th_rdv de rendez vous, si l'on saute la condition et donc que le thread ne se met pas en attente, c'est que la variable partagée count vaut 3 et à ce moment on va envoyer un signal à un autre thread et décrémenter la variable count. Le thread qui se réveille poursuit son exécution et si
count >1 cela veut dire que il reste des threads à réveiller. Si la variable count vaut 1 c'est que le dernier thread est réveillé et il n'est pas nécessaire d'envoyer un signal (il serait de toute façon perdu).

Exercice supplémentaire
-----------------------

Nous avons, dans cet exercice, pu mettre en place une structure particulière de gestion des signaux avec des threads. Dans le cas général, les signaux sont communs à tous les threads, mais pas le masque des signaux. Chaque thread a son propre masque de signaux. Ainsi, de base, tous les threads peuvent être perturbés par des signaux  externe (SIGKILL, SIGINT , ...). On peut néanmoins masquer, dans un thread en particulier, un ou plusieurs signaux. C'est là-dessus que s'appuie cet exercice : on souhaite qu'un thread unique soit assigné à la gestion des signaux. Pour cela, on bloque, dans tous les threads, sauf un, tous les signaux. Le thread qui n'a pas bloqué les signaux est celui qui sera assigné à leur gestion. On peut donc, dans ce thread, bloquer certains signaux, ou les laisser tous recevables. On se met ensuite en attente d'éventuels signaux, et on boucle sur cette attente (sigwait), pour être toujours en mesure de les traiter.

Dans le programme, on commence donc par créer le thread « signaux ». Puis, on masque tous les signaux, pour enfin créer l'ensemble des autres threads. Tous ces threads, créés dans un second temps, ne peuvent recevoir aucun signaux. Ceux-ci déclenchent la fonction th_fonc, dans laquelle ils affichent un texte. Le thread « signaux », lui, déclenche la fonction th_sig, dans laquelle on définit les signaux que l'on souhaite recevoir (ici, « Ctrl+C » et « Ctrl+\ »). Et pour s'assurer du bon fonctionnement, à la réception d'un de ces deux signaux, on affiche un texte.
Le programme fonctionne correctement, dans la mesure où l'envoi de signaux ne stoppe aucun thread, mais affiche le « printf » du thread « signaux ». On parvient donc bien, comme souhaité, à une situation dans laquelle un thread est dédié à la gestion des signaux, et les autres en sont complètement coupés.
