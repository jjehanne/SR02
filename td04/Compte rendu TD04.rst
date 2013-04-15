COMPTE RENDU du TD n°4 de SR02
==============================

Exercice n°1
------------

Cet exercice de TD vise à comparer les temps d'exécution de deux programmes, qui ne diffère que d'une fonction. Dans le premier programme, nous allons implémenter une fonction mulabc() va effectuer le porduit matriciel usuel de A par B et mettre le résultat dans la matrice C. Dans le deuxième programme, nous implémenterons une fonction mulaxc() où l'on va à chaque fois copier la colonne de la matrice B dans un vecteur X avant de faire le produit et de mettre le résultat dans C.

Le deuxième programme est plus optimisé car les matrices sont stockées ligne par ligne. Or le produit matriciel usuel multiplie les coefficients de la ligne de la première matrice par les coefficient de la colonne de la deuxième matrice. Ceci va impliquer de nombreux défauts de page qui vont ralentir l'exécution du programme. Au contraire en mettant les colonnes dans un vecteur, la colonne sera désormais stockée en ligne et le nombre de défauts de page sera bien moins important.

On mesure le temps d'exécution d'un programme avec la commande "time ./mon_programme [arguments]"
Ceci nous donne entre autres le temps passé sur l'utilisateur et le temps passé en entrée/sortie. Il suffit de faire la somme des ces 2 temps pour avoir la durée d'exécution du programme.

Test:

- mat1 1000 -> u:9.253, s:0.020 => total = 9.273s
- mat2 1000 -> u:3.571, s:0.020 => total = 3.591s

On voit donc que le programme mat2 est presque 3 fois plus rapide que le programme mat1.

On peut aussi améliorer le temps d'exécution d'un programme en optimisant sa compilation. Pour ce faire, on utilise la commande "gcc -O2 -o mon_programme mon_programme.c"

Compilés de la sorte, les deux programmes s'exécutent avec ces durées :

- mat1 1000 -> u:8.831, s:0.023 => total = 8.854s
- mat1 1000 -> u:1.266, s:0.025 => total = 1.291s
