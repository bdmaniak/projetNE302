#ifndef LECTURE_H
#define LECTURE_H


char lire (FILE *lu, int indice);

int rechercheMot(char *mot, FILE *fic);

int rechercheString(string *mot, FILE *fic);

int pointeurLigne (FILE *fic, int n);

string *recupereMot (int ligne, int n, FILE *fic);

char recupChar (string *chaine, int n);

int compareChaineStr (string *s1, char *chaine);

int compareString(string *s1, string *s2);

void afficherString(string *chaine);

int IndiceDernierElement(int ligne, FILE *grammaire);

void afficherArbreBasic(noeud *n1);

#endif
