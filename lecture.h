#ifndef LECTURE_H
#define LECTURE_H

/*Ces fonctions permettent la lecture et l'affichage dans des fichiers ou des structures string*/

//Permet de lire le indice-ième caractere d'un fichier
char lire (FILE *lu, int indice);


//Retourne la ligne de grammaire.txt correspondant à la definition de ce mot-clé
int rechercheMot(char *mot, FILE *fic);


//Même principe mais à partir d'une structure string au lieu de char*
int rechercheString(string *mot, FILE *fic);


//Permet de retourner l'indice correspondant à la n-ième ligne du fichier
int pointeurLigne (FILE *fic, int n);


//Permet de stocker le n-ième mot de la ligne-ième ligne dans une structure string
string *recupereMot (int ligne, int n, FILE *fic);


//Permet de recuperer le n-ième char dans une structure string
char recupChar (string *chaine, int n);

//Compare un element de type string avec un char* (comme un strcmp)
int compareChaineStr (string *s1, char *chaine);

//Même principe avec deux string
int compareString(string *s1, string *s2);


//Affiche un élément de type string
void afficherString(string *chaine);


//Retourne la position du dernier mot sur une ligne (sera utile pour creerArbre)
int IndiceDernierElement(int ligne, FILE *grammaire);


//Fonction simple permettant d'afficher une partie de l'arbre
void afficherArbreBasic(noeud *n1);


void afficherArbre(noeud *n1);

#endif
