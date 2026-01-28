#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// liste chainee qui contient les mots de code 
typedef struct mot {
    char *chaine;
    int taille;
    int ligne;
    struct mot *next;
} mot;

// liste chainne qui contient les   identifients 
typedef struct identifient{
    char nom;
    char type[10];
    int ligne;
    struct identifient * next;
}identifient;

// Fonction qui crée une liste vide
mot *creerListe() {
    return NULL;
}


// Fonction qui ajoute une chaine de caractère dans la fin d'une liste
mot *ajouterMot(mot *liste, char *m, int l) {
    mot *nouvMot = (mot *)malloc(sizeof(mot));    
    nouvMot->chaine = (char *)malloc(strlen(m) + 1);
    strcpy(nouvMot->chaine, m);
    nouvMot->taille = strlen(m);
    nouvMot->ligne = l;
    nouvMot->next = NULL;
    
    if (liste == NULL) {
        return nouvMot;
    } else {
        mot *courant = liste;
        while (courant->next != NULL) {
            courant = courant->next;
        }
        courant->next = nouvMot;
        return liste;
    }
}

// Fonction qui affiche une liste
void afficheListe(mot *liste) {
    mot *courant = liste;
    while (courant != NULL) {
        printf("%s (ligne %d) -> ", courant->chaine, courant->ligne);  
        courant = courant->next;
    }
    printf("NULL\n");
}

// Fonction pour libérer la mémoire de la liste
void libererListe(mot *liste) {

    mot *courant = liste;
    mot *temp;
    
    while (courant != NULL) {
        temp = courant;
        courant = courant->next;
        free(temp->chaine);
        free(temp);
    }

}
// fonction qui decoupe une suite de phrases en mots par ligne  
mot *decoupeMot(FILE *fptr1, FILE *fptr2, mot *liste) {
    char ch;
    char tab[100];
    int i = 0;
    int ligne = 1;

    while ((ch = fgetc(fptr1)) != EOF) {
        // pour verifier est ce que c'est un separateur 
        if (ch == '(' || ch == ')' || ch == ';' || ch=='{' || ch=='}' || ch==':') {

            if (i > 0) {
                tab[i] = '\0';
                fprintf(fptr2, "%s\n", tab);
                liste = ajouterMot(liste, tab, ligne);
                i = 0;
            }
            
            tab[0] = ch;
            tab[1] = '\0';
            fprintf(fptr2, "%s\n", tab);
            // ajouter le mot dans la liste 
            liste = ajouterMot(liste, tab, ligne);
        }

        else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {

            if (i > 0) {
                tab[i] = '\0';
                fprintf(fptr2, "%s\n", tab);
                liste = ajouterMot(liste, tab, ligne);
                i = 0;
            }

            if (ch == '\n') {
                ligne++;
            }
        }

        else {
            tab[i++] = ch;
        }
    }

    if (i > 0) {
        tab[i] = '\0';
        fprintf(fptr2, "%s\n", tab);
        liste = ajouterMot(liste, tab, ligne);
    }

    return liste;
}
// fonction qui cherche une chaine de caractere dans le dictionnaire  
void check_dictionnaire(mot *ch, FILE *fptr1, FILE *fptr2) {
    char mot[100];
    int trouve;
    
    while ( ch != NULL) {

        trouve = 0;  
        rewind(fptr1);
        
        while (fscanf(fptr1,"%s",mot) != EOF) {
            if (strcmp(ch->chaine, mot) == 0) {

                trouve = 1;  // mot trouvee 
                break;
            }
        }
    
        if (trouve == 0) {   
            fprintf(fptr2, "Erreur lexicale   | Ligne %d | '%s' n'existe pas dans le dictionnaire.\n",ch->ligne, ch->chaine);
        }
        ch = ch->next;
    }
}


void check_pointvirgule(mot *ch, FILE *fptr) {

    /* sécurité */
    if (ch == NULL || ch->next == NULL)
        return;

    /* fin de ligne détectée */
    if (ch->ligne != ch->next->ligne) {

        /* mots qui ne nécessitent pas ; */
        if (strcmp(ch->chaine, "Debut") == 0 ||strcmp(ch->chaine, "Fin") == 0 ||strcmp(ch->chaine, "Alors") == 0 ||strcmp(ch->chaine, "Sinon") == 0 ||strcmp(ch->chaine, "FinSi") == 0) {
            return;
        }

        /* vérification du ; */
        if (strcmp(ch->chaine, ";") != 0) {
            fprintf(fptr,
                "Erreur Syntaxique | Ligne %d | point-virgule ';' manquant en fin de ligne\n",
                ch->ligne);
        }
    }
}

// fonction qui test est ce qu'une chaine de caractere est un operateur 
int isOperateur(char *s) {
    return strcmp(s, "==") == 0 || strcmp(s, "<") == 0 || strcmp(s, ">") == 0 || strcmp(s, "=<") == 0 || strcmp(s, ">=") == 0 || strcmp(s, "!=") == 0;   
}

int estIdentifiant(const char *s) {
    // Vérifier que la chaîne existe et a exactement 1 caractère
    if (!s || s[0] == '\0' || s[1] != '\0') return 0;
    
    // Vérifier que c'est une lettre (majuscule ou minuscule)
    if ((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')) {
        return 1;
    }
    
    return 0;
}
// ------------------------ Analyse Lire/Ecrire ------------------------
// fonction qui fais l'analyse synthaxique des instructions Lire / Ecrire 
mot *analyser_lire_Ecrire(mot *courant, FILE *fptr) {

    if (courant == NULL ||(strcmp(courant->chaine, "Lire") != 0 &&strcmp(courant->chaine, "Ecrire") != 0))
        return courant;

    int ligne = courant->ligne;
    courant = courant->next;

    // Vérifier "("
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | '(' manquant\n", ligne);
        return NULL;
    }
    if (strcmp(courant->chaine, "(") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | '(' manquant\n", ligne);
    else
        courant = courant->next;

    // Vérifier identifiant
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | identifiant manquant\n", ligne);
        return NULL;
    }
    if (!estIdentifiant(courant->chaine))
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | identifiant invalide\n", ligne);
        else{
    courant = courant->next;
        }   

    // Vérifier ")"
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ')' manquant\n", ligne);
        return NULL;
    }
    if (strcmp(courant->chaine, ")") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ')' manquant\n", ligne);
    else
        courant = courant->next;

    // Vérifier ";"
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ';' manquant\n", ligne);
        return NULL;
    }
    if (strcmp(courant->chaine, ";") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ';' manquant\n", ligne);
    else
        courant = courant->next;

    return courant;
}

// ------------------------ Analyse Variable ------------------------
// fonction qui fais l'analyse synthaxique de declaration des variables :
mot *analyser_Variable(mot *courant, FILE *fptr) {

    if (courant == NULL || strcmp(courant->chaine, "Variable") != 0)
        return courant;

    int ligne = courant->ligne;
    courant = courant->next;

    // Identifiant
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | identifiant manquant\n", ligne);
        return NULL;
    }

    if (!estIdentifiant(courant->chaine))
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | identifiant invalide\n", courant->ligne);
    else
        courant = courant->next;

    // ":"
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ':' manquant\n", ligne);
        return NULL;
    }

    if (strcmp(courant->chaine, ":") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ':' manquant\n", courant->ligne);
    else
        courant = courant->next;

    // Type
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | type manquant\n", ligne);
        return NULL;
    }

    if (strcmp(courant->chaine, "Entier") != 0 && strcmp(courant->chaine, "Reel") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | type invalide (Entier ou Reel attendu)\n", courant->ligne);
    
        courant = courant->next;

    // ";"
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ';' manquant\n", ligne);
        return NULL;
    }

    if (strcmp(courant->chaine, ";") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ';' manquant\n", courant->ligne);
    else
        courant = courant->next;

    return courant;
}

// ------------------------ Analyse Si ------------------------
// fonction qui fais l'analyse synthaxique de instruction Si  
mot *analyser_Si(mot *courant, FILE *fptr) {
    if (courant == NULL || strcmp(courant->chaine, "Si") != 0)
        return courant;

    courant = courant->next; // avancer après "Si"
    
    // Vérifier "("
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | '(' manquant\n");
        return NULL;
    }
    if (strcmp(courant->chaine, "(") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | '(' manquant\n", courant->ligne);
    
        courant = courant->next;

    // Vérifier premier identifiant
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | premier comparant manquant\n");
        return NULL;
    }
    if (!estIdentifiant(courant->chaine))
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | premier comparant invalide\n", courant->ligne);
        courant = courant->next;

    // Vérifier opérateur
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | opérateur manquant\n");
        return NULL;
    }
    if (!isOperateur(courant->chaine))
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | opérateur invalide\n", courant->ligne);
        courant = courant->next;

    // Vérifier deuxième identifiant
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | deuxième comparant manquant\n");
        return NULL;
    }
    if (!estIdentifiant(courant->chaine))
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | deuxième comparant invalide\n", courant->ligne);
        courant = courant->next;

    // Vérifier ")"
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | ')' manquant\n");
        return NULL;
    }
    if (strcmp(courant->chaine, ")") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | ')' manquant\n", courant->ligne);
        courant = courant->next;

    // Vérifier "Alors"
    if (courant == NULL) {
        fprintf(fptr, "Erreur Syntaxique | 'Alors' manquant\n");
        return NULL;
    }
    if (strcmp(courant->chaine, "Alors") != 0)
        fprintf(fptr, "Erreur Syntaxique | Ligne %d | 'Alors' manquant\n", courant->ligne);
        courant = courant->next;

    // Instructions entre Alors et Sinon/FinSi
    while (courant != NULL &&
           strcmp(courant->chaine, "Sinon") != 0 &&
           strcmp(courant->chaine, "FinSi") != 0) {
        if (strcmp(courant->chaine, "Si") == 0)
            courant = analyser_Si(courant, fptr);
        else if (strcmp(courant->chaine, "Lire") == 0 || strcmp(courant->chaine, "Ecrire") == 0)
            courant = analyser_lire_Ecrire(courant, fptr);
        else if (strcmp(courant->chaine, "Variable") == 0)
            courant = analyser_Variable(courant, fptr);
        else
            courant = courant->next;
    }

    // Vérifier Sinon
    if (courant != NULL && strcmp(courant->chaine, "Sinon") == 0) {
        courant = courant->next;
        while (courant != NULL && strcmp(courant->chaine, "FinSi") != 0) {
            if (strcmp(courant->chaine, "Si") == 0)
                courant = analyser_Si(courant, fptr);
            else if (strcmp(courant->chaine, "Lire") == 0 || strcmp(courant->chaine, "Ecrire") == 0)
                courant = analyser_lire_Ecrire(courant, fptr);
            else if (strcmp(courant->chaine, "Variable") == 0)
                courant = analyser_Variable(courant, fptr);
            else
                courant = courant->next;
        }
    }

    // Vérifier FinSi
    if (courant == NULL || strcmp(courant->chaine, "FinSi") != 0) {
        fprintf(fptr, "Erreur Syntaxique | 'FinSi' manquant\n");
        return courant;
    }

    // Avancer après FinSi
    return courant->next;
}


// ------------------------ Analyse syntaxique globale ------------------------
void analyse_syntaxique(mot *liste, FILE *fptr) {
    if (liste == NULL) { fprintf(fptr, "Erreur Syntaxique | La liste est vide\n"); return; }

    mot *courant = liste;
    mot *dernier = NULL;
    int finTrouve=0;

    if (strcmp(courant->chaine, "debut") != 0)
        fprintf(fptr, "Erreur Syntaxique | il faut que le programme commence par 'debut'\n");
    else
        courant = courant->next;

    while (courant != NULL) {
        if (strcmp(courant->chaine, "Fin") == 0) {
            finTrouve = 1;
            break; 
        }

        
         else if (strcmp(courant->chaine, "Si") == 0)
         courant = analyser_Si(courant, fptr);

        else if (strcmp(courant->chaine, "Lire") == 0 || strcmp(courant->chaine, "Ecrire") == 0)
         courant = analyser_lire_Ecrire(courant, fptr);

        else if (strcmp(courant->chaine, "Variable") == 0)
        courant = analyser_Variable(courant, fptr);

        else courant = courant->next;
    }
if (finTrouve == 0) {
    fprintf(fptr,
        "Erreur Syntaxique | il faut que le programme termine par 'Fin'\n");
}

}

// fonction qui fait la recherche d'un identifient dans la liste des identifients 
int search_identifiant(identifient* id,char c){
    identifient *courant=id;

    while(courant!=NULL){
        if(courant->nom==c){
            return 1;
        }
        courant=courant->next;
    }
    return 0;
}
// fonction qui cherche un identifent dans la liste des identifients
identifient * get_identifiant(identifient* id,char c){
    identifient *courant=id;

    while(courant!=NULL){
        if(courant->nom==c){
            return courant;
        }
        courant=courant->next;
    }
    return NULL;
}
// fonction qui ajoute un identifient dans la liste des identifients 
identifient *insert_identifient(identifient *liste,char c,char *type,int ligne,FILE *fptr) {

    // Vérifier double déclaration
    if (search_identifiant(liste, c)) {
        fprintf(fptr,
            "Erreur sémantique | Ligne %d | double déclaration de l'identifiant '%c'\n",
            ligne, c);
        return liste;
    }

    // Création du nouvel identifiant
    identifient *new_id = (identifient *)malloc(sizeof(identifient));
    if (new_id == NULL) {
        printf("Erreur allocation mémoire\n");
        return liste;
    }

    new_id->nom = c;
    strcpy(new_id->type, type);
    new_id->ligne = ligne;

    // Insertion en tête
    new_id->next = liste;
    liste = new_id;

    return liste;
}


// fonction qui fait l'analyse sémantique  de programme 
identifient* analyse_semantique(mot* liste, FILE *fptr, identifient* id) {
    mot *courant = liste;
    identifient *id_1 = NULL;
    identifient *id_2 = NULL;

    while (courant != NULL) {

        /* Déclaration de variable */
        if (strcmp(courant->chaine, "Variable") == 0) {
            courant = courant->next;
            if (courant != NULL && courant->next != NULL && courant->next->next != NULL) {

                    // Vérifier si la variable est déjà déclarée
                if (get_identifiant(id, courant->chaine[0]) != NULL) {
                    fprintf(fptr, "Erreur semantique | ligne %d | La variable '%c' est declaree deux fois\n", 
                    courant->ligne, courant->chaine[0]);
                } else {
                    // Insérer seulement si elle n'existe pas
                    id = insert_identifient(id, courant->chaine[0], courant->next->next->chaine, courant->ligne, fptr);
        }
    }   
    }

        /* Lire / Ecrire */
        else if (strcmp(courant->chaine, "Lire") == 0 ||
                 strcmp(courant->chaine, "Ecrire") == 0) {

            courant = courant->next;
            if (courant != NULL) courant = courant->next;

            if (courant != NULL) {
                if (get_identifiant(id, courant->chaine[0]) == NULL) {
                    fprintf(fptr,
                        "Erreur sémantique | Ligne %d | '%c' variable non declare\n",
                        courant->ligne, courant->chaine[0]);
                }
            }
        }

        // analyse semantique de l'instruction 'Si'
        else if (strcmp(courant->chaine, "Si") == 0) {
            courant = courant->next;

            /* premier identifiant */
            if (courant != NULL) courant = courant->next;

            if (courant != NULL) {
                id_1 = get_identifiant(id, courant->chaine[0]);
                if (id_1 == NULL) {
                    fprintf(fptr,
                        "Erreur sémantique | Ligne %d | '%c' variable non declare\n",courant->ligne, courant->chaine[0]);
                }
            }

            if (courant != NULL) courant = courant->next;
            int ligne =courant->ligne;
            
            if (courant != NULL) {
                courant = courant->next;
            
                if (courant != NULL) {
                    id_2 = get_identifiant(id, courant->chaine[0]);
                    if (id_2 == NULL) {
                        fprintf(fptr,"Erreur sémantique | Ligne %d | '%c' variable non declare\n",courant->ligne, courant->chaine[0]);
                    }
                }
            }

            /* comparaison des types */ 
            if (id_1 != NULL && id_2 != NULL) {
                if (strcmp(id_1->type, id_2->type) != 0) {
                    fprintf(fptr,
                        "Erreur sémantique | Ligne %d | les deux variables sont de types differents\n",
                        ligne);
                }
            }
        }
        courant = courant->next;
    }

    return id;
}

int main() {
    FILE *fptr_execute;
    FILE *fptr_output;
    FILE *fptr_dictionnaire;
    FILE *fptr_error;
    
    fptr_execute = fopen("execute.txt", "r");
    fptr_output = fopen("output.txt", "w");
    fptr_dictionnaire = fopen("lexer.txt", "r");
    fptr_error = fopen("erreur.txt", "w");
    
    mot *chaine = creerListe();
    identifient*id=(identifient*)malloc(sizeof(identifient));
    id=NULL;
    
    if (fptr_execute == NULL || fptr_output == NULL || 
        fptr_dictionnaire == NULL || fptr_error == NULL) {
        printf("Erreur d'ouverture de fichier(s)\n");
        return 1;
    }
    
    chaine = decoupeMot(fptr_execute, fptr_output, chaine);
    
    printf("Liste des mots:\n");
    
    fclose(fptr_execute);  
    fclose(fptr_output);
    
    fptr_output = fopen("output.txt", "r");
    if (fptr_output == NULL) {
        printf("Erreur réouverture output.txt\n");
        libererListe(chaine);
        fclose(fptr_dictionnaire);
        fclose(fptr_error);
        return 1;
    }
        fprintf(fptr_error,"\n----------------- Analyse Lexicale -------------------\n\n");

    check_dictionnaire( chaine, fptr_dictionnaire, fptr_error);
    
    afficheListe(chaine);
            fprintf(fptr_error,"\n----------------- Analyse Synthaxique -------------------\n\n");


    analyse_syntaxique(chaine,fptr_error);
            fprintf(fptr_error,"\n----------------- Analyse semantique  -------------------\n\n");
            analyse_semantique(chaine,fptr_error,id);

    
    // Fermer tous les fichiers
    fclose(fptr_output);
    fclose(fptr_dictionnaire);
    fclose(fptr_error);
    
    // Libérer la mémoire
    libererListe(chaine);
    
    printf("Traitement terminé. Vérifiez erreur.txt pour les erreurs.\n");
    
    return 0;
}


