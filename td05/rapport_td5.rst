William DEPRET
Jonathan JEHANNE

COMPTE RENDU TD n°5 de SR02 SEGMENT MEMOIRE

0) Création de sharemem1.c
---------------------------

Dans ce programme, il s'agit tout d'abord d'initialiser un tableau de 10 éléments qui va constituer le support des données à échanger grâce au segment mémoire. On crée et attache donc ensuite un segment mémoire dont l’accès se fera par un pointeur d'entier ptr. Ce segment mémoire possède pour clef IPC_PRIVATE car le segment de mémoire partagée sera utilisé entre un processus père et un processus fils. On effectue dans ce premier programme un fork() afin de créer un processus fils. Le père affiche les valeurs de tab2[] (qui sont les valeurs initiales) et effectue un sleep() de sorte que le processus fils ait la CPU et puisse effectuer son travail : modifier tab2 et mettre les nouvelles valeurs dans le segment mémoire. On prend soin de détacher le segment mémoire du fils à la fin de ses instructions. Le père reprend ensuite la main et copie les valeurs du segment de mémoire partagée dans tab2[] et affiche ensuite ce dernier. Les valeurs de tab2[] ont bien été modifiées, ce qui confirme que l'on a bien échangé des données grâce au segment de mémoire partagée. On termine le programme en attendant que le fils se termine (primitive waitpid()), avec un envoi de signal SIGINT si une erreur se produit, puis on détache le segment mémoire du père, et enfin, on le supprime via la primitive shmctl().Ojn vérifie bien que le segment de mémoire partagée a bien été supprimé via la commande ipcs (si elle n'avait pas été supprimée, il aurait fallu entrer la commande ipcrm -m <ID_Segment_Mémoire>).


Question 1) 
-----------

Lorsque le père imprime tab2[] pour la première fois, le résultat est différent de celui du fils car le fils a déjà modifié les valeurs de tab2 étant donné qu'il est le premier à s'exécuter, le père faisant un sleep. Il est identique à la deuxième impression car on a copié le contenu de tab2[] dans le segment de mémoire partagée et on a ensuite modifié tab2[] du père avec les valeurs récupérées dans ce segment.


Question 2)
-----------

 Dans cette partie, il n'ait désormais plus possible d'utiliser IPC_PRIVATE car nous voulons utiliser un segment de mémoire partagée entre deux processus distincts. Il nous faut donc créer une clef unique, qui sera identique pour les deux processus afin qu'ils partagent bien le même segment de mémoire. On utilise la primitive ftok() avec un fichier nommé sr02p009.h pour que la clé soit « unique ». La seule différence avec le premier programme est que la primitive shmget() utilise donc désormais la clef et non plus IPC_PRIVATE. Le reste du programme possède la même structure, la partie du fils étant exécuté par shrmem_e.c et la partie du père par shrmem_r.c. Ici aussi, on prend bien soin de détacher le segment mémoire et de le supprimer via les primitives shmdt() et shmctl().

Question 3)
-----------

 Dans cette partie du TD, nous devons reprendre les programmes précédents et les modifier afin de créer, dans le segment de mémoire partagée, un tableau de 5 pointeurs suivi de 5 vecteurs de 3 entiers chacun. Chaque pointeur est initialisé avec l'adresse du premier élément d'un vecteur. On initialise les vecteurs avec des nombres tous différents (de 5 à 19, leur position, pour simplifier). Le changement apporté par cette partie est qu'il n'est désormais plus question d'accéder aux éléments du tableau par leur indice, mais grâce à un pointeur. Le problème soulevé est que l'on ne peut pas lire les valeurs des tableaux à travers les pointeurs dans le programme shrptr.c. En effet, seul un segment mémoire est créé en mémoire physique mais les différents processus y accèdent à travers leurs adresses logiques (virtuelles) qui peuvent être différents selon l'adresse d'attachement. Les adresses sauvegardées par shrpte.c dans les cinq premières cases du segment partagé sont donc des adresses logiques, ce qui entraîne l'erreur de segmentation lors de l'utilisation de ces adresses par shrptr.c. On résout ce problème en mettant dans les cinq premières cases, des déplacements relatifs au début de segment (en nombre d'entiers) au lieu des adresses absolues. En effet le premier élément du premier vecteur se situe à ptr[5], le premier élément du deuxième vecteur à ptr[8], celui du troisième à ptr[11] etc... Nous pouvons donc de cette façon accéder aux vecteurs dans le programme shrptr.c en bouclant sur i de 0 à 2 pour afficher les trois éléments d'un vecteur, le tout imbriqué dans une boucle sur j (de 0 à 4, pour chaque vecteur). Le contenu de chaque vecteur est récupéré grâce à *((ptr+tab_rcv[j])+i)) où ptr est &ptr[0], tab_rcv[j] correspond à ptr[j] où j est la variable correspondant au nombre de vecteur (5 ici), et i correspond à l'élément du vecteur à afficher (3 éléments par vecteur).

Question 4)et 5)
----------------

 Nous n'avons pas constaté d'erreur car nous sommes directement passés par des pointeurs relatifs, solution au problème que nous aurions dû rencontrer ici. La méthode de base aurait été d'attacher le segment de mémoire partagée à la même adresse dans les deux programmes shrptr.c et shrpte.c. Dans ce cas, les deux programmes auraient eu les mêmes adresses logiques (la même vision virtuelle) du segment partagé. Ainsi, les instructions de cette question permettent de forcer un décalage pour avoir des adresses d'attachement différentes et ainsi constater les erreurs de segmentation.

Question 6)
-----------

Implémentation de inific.c et de lirfic.c
Il suffit pour cette question de bien séparer le code donné dans l’énoncé. Le programme inific.c va donc ouvrir le fichier titi.dat et y écrire le contenu de tab1[] avant de le refermer. Nous avons ajouté des test pour rendre le code plus robuste . Ensuite, dans lirfic.c, nous ouvrons titi.dat nous mettons le contenu dans tab2[] et nous refermons le fichier. Une boucle permettant l’affichage de tab2[] est ensuite implémentée.

Question 7) Implémentation de modfic.c et showfic.c
---------------------------------------------------

Dans le programme modfic.c, on ouvre le fichier titi.dat et on en fait une projection en mémoire grâce à la primitive mmap() :

adr = (int *) mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

Cette primitive sera appelée avec l’option MAP_SHARED afin que titi.dat soit modifié de la même manière que lorsqu’on modifie les données du segment « mmap ». On vérifie par un test que la projection a fonctionné et on entre ensuite dans une boucle infinie où l’utilisateur est invité à entrer une valeur pour modifier adr[i]. Il peut quitter le programme en entrant le nombre 99.

Pour le programme showfic.c, on fait la même chose que précédemment, on ouvre le fichier titi.dat, on mappe de la même manière et on boucle sur i de 0 à 9 pour afficher adr[i]. 
