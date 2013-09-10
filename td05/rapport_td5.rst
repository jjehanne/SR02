William DEPRET
Jonathan JEHANNE

COMPTE RENDU TD n�5 de SR02 SEGMENT MEMOIRE

0) Cr�ation de sharemem1.c
---------------------------

Dans ce programme, il s'agit tout d'abord d'initialiser un tableau de 10 �l�ments qui va constituer le support des donn�es � �changer gr�ce au segment m�moire. On cr�e et attache donc ensuite un segment m�moire dont l�acc�s se fera par un pointeur d'entier ptr. Ce segment m�moire poss�de pour clef IPC_PRIVATE car le segment de m�moire partag�e sera utilis� entre un processus p�re et un processus fils. On effectue dans ce premier programme un fork() afin de cr�er un processus fils. Le p�re affiche les valeurs de tab2[] (qui sont les valeurs initiales) et effectue un sleep() de sorte que le processus fils ait la CPU et puisse effectuer son travail : modifier tab2 et mettre les nouvelles valeurs dans le segment m�moire. On prend soin de d�tacher le segment m�moire du fils � la fin de ses instructions. Le p�re reprend ensuite la main et copie les valeurs du segment de m�moire partag�e dans tab2[] et affiche ensuite ce dernier. Les valeurs de tab2[] ont bien �t� modifi�es, ce qui confirme que l'on a bien �chang� des donn�es gr�ce au segment de m�moire partag�e. On termine le programme en attendant que le fils se termine (primitive waitpid()), avec un envoi de signal SIGINT si une erreur se produit, puis on d�tache le segment m�moire du p�re, et enfin, on le supprime via la primitive shmctl().Ojn v�rifie bien que le segment de m�moire partag�e a bien �t� supprim� via la commande ipcs (si elle n'avait pas �t� supprim�e, il aurait fallu entrer la commande ipcrm -m <ID_Segment_M�moire>).


Question 1) 
-----------

Lorsque le p�re imprime tab2[] pour la premi�re fois, le r�sultat est diff�rent de celui du fils car le fils a d�j� modifi� les valeurs de tab2 �tant donn� qu'il est le premier � s'ex�cuter, le p�re faisant un sleep. Il est identique � la deuxi�me impression car on a copi� le contenu de tab2[] dans le segment de m�moire partag�e et on a ensuite modifi� tab2[] du p�re avec les valeurs r�cup�r�es dans ce segment.


Question 2)
-----------

 Dans cette partie, il n'ait d�sormais plus possible d'utiliser IPC_PRIVATE car nous voulons utiliser un segment de m�moire partag�e entre deux processus distincts. Il nous faut donc cr�er une clef unique, qui sera identique pour les deux processus afin qu'ils partagent bien le m�me segment de m�moire. On utilise la primitive ftok() avec un fichier nomm� sr02p009.h pour que la cl� soit � unique �. La seule diff�rence avec le premier programme est que la primitive shmget() utilise donc d�sormais la clef et non plus IPC_PRIVATE. Le reste du programme poss�de la m�me structure, la partie du fils �tant ex�cut� par shrmem_e.c et la partie du p�re par shrmem_r.c. Ici aussi, on prend bien soin de d�tacher le segment m�moire et de le supprimer via les primitives shmdt() et shmctl().

Question 3)
-----------

 Dans cette partie du TD, nous devons reprendre les programmes pr�c�dents et les modifier afin de cr�er, dans le segment de m�moire partag�e, un tableau de 5 pointeurs suivi de 5 vecteurs de 3 entiers chacun. Chaque pointeur est initialis� avec l'adresse du premier �l�ment d'un vecteur. On initialise les vecteurs avec des nombres tous diff�rents (de 5 � 19, leur position, pour simplifier). Le changement apport� par cette partie est qu'il n'est d�sormais plus question d'acc�der aux �l�ments du tableau par leur indice, mais gr�ce � un pointeur. Le probl�me soulev� est que l'on ne peut pas lire les valeurs des tableaux � travers les pointeurs dans le programme shrptr.c. En effet, seul un segment m�moire est cr�� en m�moire physique mais les diff�rents processus y acc�dent � travers leurs adresses logiques (virtuelles) qui peuvent �tre diff�rents selon l'adresse d'attachement. Les adresses sauvegard�es par shrpte.c dans les cinq premi�res cases du segment partag� sont donc des adresses logiques, ce qui entra�ne l'erreur de segmentation lors de l'utilisation de ces adresses par shrptr.c. On r�sout ce probl�me en mettant dans les cinq premi�res cases, des d�placements relatifs au d�but de segment (en nombre d'entiers) au lieu des adresses absolues. En effet le premier �l�ment du premier vecteur se situe � ptr[5], le premier �l�ment du deuxi�me vecteur � ptr[8], celui du troisi�me � ptr[11] etc... Nous pouvons donc de cette fa�on acc�der aux vecteurs dans le programme shrptr.c en bouclant sur i de 0 � 2 pour afficher les trois �l�ments d'un vecteur, le tout imbriqu� dans une boucle sur j (de 0 � 4, pour chaque vecteur). Le contenu de chaque vecteur est r�cup�r� gr�ce � *((ptr+tab_rcv[j])+i)) o� ptr est &ptr[0], tab_rcv[j] correspond � ptr[j] o� j est la variable correspondant au nombre de vecteur (5 ici), et i correspond � l'�l�ment du vecteur � afficher (3 �l�ments par vecteur).

Question 4)et 5)
----------------

 Nous n'avons pas constat� d'erreur car nous sommes directement pass�s par des pointeurs relatifs, solution au probl�me que nous aurions d� rencontrer ici. La m�thode de base aurait �t� d'attacher le segment de m�moire partag�e � la m�me adresse dans les deux programmes shrptr.c et shrpte.c. Dans ce cas, les deux programmes auraient eu les m�mes adresses logiques (la m�me vision virtuelle) du segment partag�. Ainsi, les instructions de cette question permettent de forcer un d�calage pour avoir des adresses d'attachement diff�rentes et ainsi constater les erreurs de segmentation.

Question 6)
-----------

Impl�mentation de inific.c et de lirfic.c
Il suffit pour cette question de bien s�parer le code donn� dans l��nonc�. Le programme inific.c va donc ouvrir le fichier titi.dat et y �crire le contenu de tab1[] avant de le refermer. Nous avons ajout� des test pour rendre le code plus robuste . Ensuite, dans lirfic.c, nous ouvrons titi.dat nous mettons le contenu dans tab2[] et nous refermons le fichier. Une boucle permettant l�affichage de tab2[] est ensuite impl�ment�e.

Question 7) Impl�mentation de modfic.c et showfic.c
---------------------------------------------------

Dans le programme modfic.c, on ouvre le fichier titi.dat et on en fait une projection en m�moire gr�ce � la primitive mmap() :

adr = (int *) mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

Cette primitive sera appel�e avec l�option MAP_SHARED afin que titi.dat soit modifi� de la m�me mani�re que lorsqu�on modifie les donn�es du segment � mmap �. On v�rifie par un test que la projection a fonctionn� et on entre ensuite dans une boucle infinie o� l�utilisateur est invit� � entrer une valeur pour modifier adr[i]. Il peut quitter le programme en entrant le nombre 99.

Pour le programme showfic.c, on fait la m�me chose que pr�c�demment, on ouvre le fichier titi.dat, on mappe de la m�me mani�re et on boucle sur i de 0 � 9 pour afficher adr[i]. 
