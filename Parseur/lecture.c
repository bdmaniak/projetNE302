#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "api.h"
#include "grammaire.h"
#include "lecture.h"


int curseurLigne[300] = {[0 ... 299] = -1};


char lire (FILE *lu, int indice){
	fseek(lu, indice, SEEK_SET);
	return getc(lu);
}


int rechercheMot(char *mot, FILE *fic){
	int indice = 0;
	string *st = NULL;
	while (((st = recupereMot(indice, 0, fic)) != NULL) && (!compareChaineStr(st, mot))){
		indice++;
		free(st);
	}
	if (!compareChaineStr (st, mot)) indice = -1;
	free(st);
	
	return indice;
}


int rechercheString(string *mot, FILE *fic){
	int indice = 0;
	string *st = NULL;
	while (((st = recupereMot(indice, 0, fic)) != NULL) && (!compareString(st, mot))){
		indice++;
		free(st);
	}
	if (!compareString(st, mot)) indice = -1;
	free(st);
	
	return indice;
}


//Permet de faire pointer le curseur de fichier à la n-ième ligne
int pointeurLigne (FILE *fic, int n) {
	int compteur = 0;
	int curseur = 0;
	char c;
	rewind(fic);
	if (curseurLigne[n] != -1) curseur = curseurLigne[n];
	else{
		while (((c = getc(fic)) != '\0') && (compteur < n)) {
			if (c == '\n') {
				compteur++;
			}
			curseur++;
		}
		if (c == '\0') curseur = -1;
		curseurLigne[n] = curseur;
	}
	return curseur;
}


//Permet de recuperer le n-ieme mot sur la ligne de notre choix
string *recupereMot (int ligne, int n, FILE *fic) {
	char c;
	int curseur = 0;
	int compteur = 0;
	int taille = 0;
	string *chaine = NULL;
	ligne = pointeurLigne(fic,ligne);
	if (ligne != -1) {
		fseek(fic, ligne, SEEK_SET);
		while (((c = getc(fic)) != '\0') && (c != '\n') && (compteur < n)) {
			if (c == ' '){
				compteur++;
			}
			curseur++;
		}
		if ((c != '\0') && (c != '\n')) {
			chaine = malloc(sizeof(string));
			chaine->fichier = fic;
			chaine->depart = ligne + curseur;
			while (((c = getc(fic)) != ' ') && (c != '\n') && (c != '\0')) {
				taille ++;
			}
			chaine->taille = taille + 1;
		}
	}
	return chaine;
}


// Recupere le n-ieme caractere d'un element string
char recupChar (string *chaine, int n) {
	fseek(chaine->fichier, chaine->depart + n, SEEK_SET);
	return getc(chaine->fichier);
}


//compare un string avec une chaine de caractere
int compareChaineStr (string *s1, char *chaine) {
	int taille = 0;
	int i = 0;
	int egal = 1;
	while (chaine[taille] != '\0'){
		taille ++;
	}
	if (s1 -> taille != taille ) egal = 0;
	while ((i < s1->taille) && egal){
		if (recupChar(s1, i) != chaine[i]) egal = 0;
		i++;
	}

	return egal;
}


//Compare deux string
int compareString(string *s1, string *s2) {
	int egal = 1;
	int i = 0;
	if (s1 -> taille != s2 -> taille ) egal = 0;
	while ((i < s1->taille) && egal) {
		if (recupChar(s1, i) != recupChar(s2, i)) egal = 0;
		i++;
	}
	return egal;
}


// Affiche un string
void afficherString(string *chaine) {
	if (chaine != NULL){
		fseek(chaine->fichier, chaine->depart, SEEK_SET);
		for (int i = 0; i< chaine -> taille; i++){
			printf("%c",getc(chaine->fichier));
		}
		printf("\n");
	}




}

char *convString(string *chaine) {
	char *chaineRetour = NULL;
	if (chaine != NULL){
		chaineRetour = malloc(sizeof(char) * ((chaine->taille) + 1));
		fseek(chaine->fichier, chaine->depart, SEEK_SET);
		for (int i = 0; i< chaine -> taille; i++){
			chaineRetour[i] = getc(chaine->fichier);
		}
		chaineRetour[(chaine->taille) + 1] = '\0';
	}
	return chaineRetour;
}


int IndiceDernierElement(int ligne, FILE *grammaire){
	int compteur = 0;
	string *st = NULL;
	while ((st = recupereMot(ligne, compteur, grammaire)) != NULL){
		compteur++;
		free(st);
	}
	return (compteur - 1);
}


void afficherArbreBasic(noeud *n1){
	while (n1 != NULL){
		if (n1 != NULL) afficherString(n1 -> nomChamp);
		if (n1 != NULL) afficherString(n1 -> valeurChamp);
		n1 = n1 -> fils;
	}
}


void afficherArbre(noeud *n1){
	if((n1 == NULL) || (n1->nomChamp == NULL)||(n1->valeurChamp == NULL)){
		printf("ERREUR : Noeud inexistant\n");
		exit(-1);
	}
	static int decalage = 0;
	while (n1 != NULL){
		for (int i = 0; i < decalage ; i++) printf("\t");
		if (n1 != NULL) afficherString(n1 -> nomChamp);
		for (int i = 0; i < decalage ; i++) printf("\t");
		if (n1 != NULL) afficherString(n1 -> valeurChamp);
		printf("\n");
		decalage++;
		if (n1 -> fils != NULL) afficherArbre(n1 -> fils);
		decalage--;
		n1 = n1->frere;
	}

}
