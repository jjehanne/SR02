COMPTE RENDU du TD n°2 de SR02
==============================

Exercice n°1
------------
Cet exercice consiste à étudier un code, dont l'objectif est de faire communiquer deux processus. Le premier, le père, se sert de l'instruction « alarm » pour s'envoyer un signal, qui déclenche une routine. Dans cette routine, un autre signal est envoyé, mais au fils, cette fois. Le fils affiche une valeur tirée au hasard, et se met en pause, attendant le prochain signal,

Le main se compose de la façon suivante : on effectue un fork(), qui permet la création du processus fils. Le processus fils, une fois créé, se rend réceptif au signal « SIGUSR1 » grâce à l'instruction « sigaction ». Recevoir ce signal déclenche la routine associée, en l'occurence « handler-thermometre ». Dans cette routine, une valeur est tirée au hasard, puis affichée. Puis, elle se termine, et on revient au fils, qui se met en pause.
Le processus père, lui, se rend réceptif au signal « SIGALRM », via l'instruction « sigaction ». Une fois cela fait, il lance l'instruction « alarm(5) », qui aura pour effet, au bout de 5 secondes, d'envoyer un signal SIGALRM au processus ayant lancé l'instruction, en l'occurrence, le processus père. Le signal déclenche la routine « handler-alarme », Dans celle-ci, on envoie un signal SIGUSR1 au processus fils (dont le pid est passé en paramètre) via l'instruction « kill ». Puis, on relance une instruction alarm(5). Pendant ces 5 secondes, le processus père affiche la barre de téléchargement, via un sleep, l'affichage d'un tiret, puis un fflush(stdout).

On obtient ainsi le fonctionnement désiré : une prise de température fictive toutes les 5 secondes, les prises étant temporisées grâce à alarm, et le processus fils sachant qu'il doit en effectuer une grâce au signal qu'il reçoit, signal envoyé par le processus père.

Exercice n°2
------------

Dans cet exercice il s'agit de faire communiquer un processus père et un processus fils par le biais de signaux.
Le processus fils va commencer par afficher la première lettre de l'alphabet en minuscule puis signifier au processus père que son travail est terminé. Le processus père va ensuite affiché la première lettre de l'alphabet en majuscule cette fois puis envoyer un signal au fils qui va alors afficher les deux lettres suivantes, toujours en minuscule. Ces échanges vont continuer jusqu'à ce que l'alphabet soit écrit en double, en alternant minuscules et majuscules et en affichant à chaque étape un caractère de plus.

Nous devons donc avoir aAbcBCdefDEFghijGHIJklmnoKLMNOpqrstuPQRSTUvwxyzVWXYZ

L'idée est de commencer avec un processus fils en pause et un processus père qui va envoyer un signal au fils , puis se mettre en pause. Cet envoi de signal constituera le point de départ de tout le programme.

Une fois le travail du fils terminé, il enverra un signal au père qui va à son tour effectuer son travail et renvoyer un signal au fils pour lui signifier à son tour qu'il a terminé et que le fils peut continuer sa partie du travail.

La routine de traitement du SIGUSR1 est presque la même pour le père et le fils, à l'exception prés que l'on va commencer avec un 'a' pour le fils , et 'A' pour le père.

On établit un compteur dans une variable globale pour savoir combien de caractère on doit afficher.
On établit aussi des variables globales pour les caractères à afficher de sorte que l'on sache d'où reprendre d'une exécution à une autre de la routine.
Il faut donc bien penser à incrémenter les variable lettrep et lettref (lettre père et lettre fils).
Enfin une condition d'arrêt est nécessaire car l'alphabet n'est pas infini.

Lorsqu'on arrive a la lettre 'z' chez le fils, on affiche le buffer, et on envoie un signal via la primitive kill() pour annoncer au père que le travail est fini, puis on sort de la fonction.
La routine de traitement du père s’exécutera une dernière fois et lorsqu'on arrive à la lettre 'Z', on vide le buffer sur la sortie standard, on n'envoie pas, cette fois, de signal au processus fils et on sort de la fonction.


Problèmes rencontrés:

Notre printf ne marchait que si l'on mettait un '\n'. Après quelques recherches nous nous sommes rendus compte que le '\n' imposait au buffer de se vider.
Nous avons donc ajouté un fflush(stdout)


Exercice n°3
------------

Le but de cet exercice est de tester l'accessibilité en lecture d'adresses. Pour se faire, on définit un tableau de 1000 entiers, que l'on parcourt. Pour chaque « case » du tableau, on teste l'adresse via la fonction probe. Si i renvoie 0, c'est qu'on a rencontré une violation d'accès mémoire. Sinon, on incrémente la variable liée au parcours du tableau et on continue ce parcours.
Notons qu'au début du main, le processus se rend réceptif au signal SIGSEGV.

La fonction probe fonctionne de la façon suivante : elle reçoit une adresse, et stocke dans statut la valeur de retour de la fonction setjmp. Setjmp permet d'effectuer une sauvegarde du contexte, et renvoie 0 si l'on « revient » du setjmp directement (et donc, pas via un longjmp). Si c'est le cas, cela signifie qu'on a fait la sauvegarde, et qu'on a pas rencontré de problème. On peut alors afficher la valeur contenu dans l'adresse (qui est accessible en lecture).

Si, à la lecture, on effectue une violation d'accès mémoire, un signal SIGSEGV sera envoyé. Le processus étant réceptif à ce signal, il va déclencher la routine associée (ici, handler-segv). On affiche alors un message précisant qu'on a reçu pareil signal, et on effectue un longjmp, qui nous permet de revenir au dernier setjmp effectué, avec pour valeur retour de ce dernier la valeur passée en paramètre du longjmp (ici, 1). On se retrouve alors dans l'état du dernier setjmp, à l'unique différence que la variable statut contient cette fois 1, et non 0, comme pour tous les cas ou on est « revenu » directement du setjmp. Le test de la variable statut déclenche l'instruction return 0, on retourne alors dans le main, où l'on test cette valeur (attribuée à la variable i). Elle vaut 0, on affiche donc un message d'erreur, et on met fin au programme.
