#ifndef GRAMMAIRE_H
#define GRAMMAIRE_H

/*Ce module contient toutes les fonctions qui permettent de creer l'arbre à partir de la grammaire*/


//Creer un noeud à partir d'un string s1 qui correspond au nom du champ et s2 qui correspond à la valeur du champ
noeud *creerNoeud(string *s1, string *s2);

//Ajoute un noeud fils n2 à un noeud n1 qui correspond au pere
void ajouteFils(noeud *n1, noeud *n2);

//Ajoute un noeud frere n2 à un noeud n1 qui correspond à son grand frere
void ajouteFrere(noeud *n1, noeud *n2);

//Permet de determiner la taille d'un noeud, cette taille correspond à la somme des tailles de ses fils
int tailleNoeud (noeud *n1);

//Permet à partir d'un curseur et d'une taille de mettre un element de la grammaire dans une structure string
string *creerString(FILE *fic, int start, int size);

//Cette fonction permet de creer le sous arbre representant l'operateur * dans la grammaire
noeud *etoile(noeud *(*fonction)(FILE*, FILE*, int*, int, int, int),FILE* grammaire, FILE* lu, int *indice, int ligne, int i, int j , int borne1, int borne2);


//CAS DE BASE

//Creer un noeud si la grammaire signale un espace et que le caractère lu est un espace
noeud *SP (FILE *grammaire, FILE *lu, int *indice);


noeud *HTAB (FILE *grammaire, FILE *lu, int *indice);

noeud *DIGIT (FILE *grammaire, FILE *lu, int *indice);

noeud *ALPHA (FILE *grammaire, FILE *lu, int *indice);

noeud *caseInsensitive (FILE *grammaire, FILE *lu, int *indice, string *element);

noeud *CRLF (FILE *grammaire, FILE *lu, int *indice);

char convAscii (char hexa[3]);


noeud *valAscii (FILE *grammaire, FILE *lu, int *indice, string *element);

noeud *creerArbre(FILE *grammaire, FILE *lu, int *indice, int ligne, int i, int j);


#endif
