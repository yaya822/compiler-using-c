MINI COMPILATEUR EN C 
Description 
-----------
 Ce projet est un mini compilateur développé en langage C.
  Il permet d’analyser un programme écrit dans un langage pseudo-algorithmique simplifié. 
  
  Le compilateur réalise :
   - une analyse lexicale 
   - une analyse syntaxique
    - une analyse sémantique 

    Les erreurs détectées sont enregistrées dans un fichier de sortie. 
    Objectifs du projet
     ------------------- 
     - Comprendre les principes de base de la compilation 
     - Manipuler les listes chaînées en langage C -
      Travailler avec les fichiers -
       Détecter les erreurs lexicales, syntaxiques et sémantiques 
       Fonctionnalités ---------------
        1) Analyse lexicale 
        - Découpage du code source en mots (tokens) 
        - Reconnaissance des séparateurs (; ( ) { } :) 
        - Vérification des mots à l’aide d’un dictionnaire 
        - Détection des erreurs lexicales 
        2) Analyse syntaxique 
        - Le programme doit commencer par "debut" 
        - Le programme doit se terminer par "Fin"
         - Vérification des instructions : Variable, Lire, Ecrire, Si, Alors, Sinon, FinSi - 
         Vérification des parenthèses et des points-virgules 
         3) Analyse sémantique 
         - Gestion des identifiants 
         - Détection des erreurs suivantes : 
         * double déclaration 
         * variable non déclarée 
          * incompatibilité de types
           Fichiers utilisés 
          ----------------- 
          main.c : code source du compilateur
           execute.txt : programme à analyser
            lexer.txt : dictionnaire lexical
            output.txt : sortie de l’analyse lexicale 
            erreur.txt : rapport des erreurs
             Exemple de programme supporté 
             ----------------------------- 
             debut
            Variable x : Entier;
            Variable y : Entier;
            Lire(x); 
            Lire(y); 
                Si (x > y) Alors
                 Ecrire(x);
                Sinon
                    Ecrire(y);
                FinSi
             Fin




