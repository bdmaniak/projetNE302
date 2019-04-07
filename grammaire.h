#ifndef GRAMMAIRE_H
#define GRAMMAIRE_H

noeud *creerNoeud(string *s1, string *s2);

void ajouteFils(noeud *n1, noeud *n2);


void ajouteFrere(noeud *n1, noeud *n2);


int tailleNoeud (noeud *n1);


string *creerString(FILE *fic, int start, int size);


noeud *etoile(noeud *(*fonction)(FILE*, FILE*, int*, int, int, int),FILE* grammaire, FILE* lu, int *indice, int ligne, int i, int j , int borne1, int borne2);


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
