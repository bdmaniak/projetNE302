#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "api.h"







noeud *creerNoeud(string *s1, string *s2){
	noeud *n1 = malloc(sizeof(noeud));
	n1 -> nomChamp = s1;
	n1 -> valeurChamp = s2;
	return n1;


}

void ajouteFils(noeud *n1, noeud *n2){

	if (n1 -> fils == NULL){
		
		n1 -> fils = n2;
		
	}
	else{
		noeud *repere = n1 -> fils;
		while (repere -> frere != NULL){
			repere = repere -> frere;			
		}
		repere -> frere = n2;

	}
	


}



void ajouteFrere(noeud *n1, noeud *n2){

	if (n1 == NULL){
		n1 = n2;
		
	}

	else if (n1 -> frere == NULL){
		n1 -> frere = n2;
		
	}
	else{

		
		noeud *repere = n1 -> frere;
		while (repere -> frere != NULL){
			
			repere = repere -> frere;			
		}
		repere -> frere = n2;

	}
	


}


int tailleNoeud (noeud *n1){
	int taille = 0;
	if (n1 == NULL) taille = 0;

	else{
		noeud *repere = n1;
		taille = (repere -> valeurChamp)->taille;
		while (repere -> frere != NULL){
			repere = repere -> frere;
			taille += (repere -> valeurChamp)->taille;			
		}

	}
	return taille;

}


noeud *etoile(noeud *(*fonction)(char *, int *), char *chaine, int *indice, int i, int j){


	if (j == 0) j = 10000000;
	int compteur = 0;
	noeud *nfils, *nfrere;
	nfils = NULL;
	nfils = fonction(chaine, indice);
	
	while ((nfils != NULL) && ((nfrere = fonction(chaine, indice)) != NULL) && (compteur < j)){
		ajouteFrere(nfils, nfrere);
		compteur++;
	}
	if ((compteur < i) || (compteur > j)){
		nfils = NULL;
		//Fonction qui supprime tout

	}	
	return nfils;


}

char lire (FILE *lu, int indice){
	fseek(lu, indice, SEEK_SET);
	return getc(lu);

}

string *creerString(FILE *fic, int start, int size){
	string *st = malloc(sizeof(string));
	st -> fichier = fic;
	st->depart = start;
	st-> taille = size; 

	return st;

}


int rechercheMot(char *mot, FILE *fic){

	int indice = 0;
	string *st = NULL;
	while (((st = recupereMot(indice, 0, fic)) != NULL) && (!compareChaineStr(st, mot))){
		indice++;
		
	}
	
	if (!compareChaineStr (st, mot)) indice = -1;

	return indice;



}


noeud *SP (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("SP", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (lire(lu, *indice) == ' '){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}


noeud *HTAB (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("HTAB", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (lire(lu, *indice) == '	'){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}

noeud *DIGIT (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("DIGIT", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (isdigit(lire(lu, *indice))){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}


noeud *ALPHA (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("ALPHA", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (isalpha(lire(lu, *indice))){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}



noeud *caseInsensitive (FILE *grammaire, FILE *lu, int *indice, char *mot){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("case_insensitive_string", grammaire), 0, grammaire);
	int taille = strlen(mot);
	string *s2 = creerString(lu, *indice, taille); 
	int correct = 1;
	int i = 0;
	while ((i < taille) && (correct) && (lire(lu, (*indice) + i) != EOF) ){
		printf("lu: %x		,mot: %x\n",lire(lu, (*indice) + i), mot[i]);
		if (lire(lu, (*indice) + i) != mot[i]) correct = 0;
		else i++;
		printf("i: %d\n", i);

	}
	if (i == taille){
		
		n1 = creerNoeud(s1, s2);
		(*indice) += taille;

	}
	return n1;

}




//Permet de faire pointer le curseur de fichier à la n-ième ligne
int pointeurLigne (FILE *fic, int n) {
	int compteur = 0;
	int curseur = 0;
	char c;
	rewind(fic);

	while (((c = getc(fic)) != '\0') && (compteur < n)) {
		if (c == '\n') {
			compteur++;
		}
		curseur++;
	}

	if (c == '\0') curseur = -1;

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
		while (((c = getc(fic)) != '\0') && (compteur < n)) {
			if (c == ' '){
				compteur++;
			}
			curseur++;
		}

		if (c != '\0') {
			chaine = malloc(sizeof(string));
			chaine->fichier = fic;
			chaine->depart = ligne + curseur;
			while (((c = getc(fic)) != ' ') && (c != '\0')) {
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
	fseek(chaine->fichier, chaine->depart, SEEK_SET);

	for (int i = 0; i< chaine -> taille; i++){
		printf("%c",getc(chaine->fichier));
	}

	printf("\n");
}


int verifieFormat(string recu, string grammaire) {
	// A faire
	return 1;
}


noeud *creerArbre(FILE *grammaire, FILE *lu, int *indice, int ligne, int i, int j){

	noeud *n1 = NULL;
	noeud *n2 = NULL;	

	//Cas de base
	if (i == j){

		if ((compareChaineStr(recupereMot(ligne, i, grammaire), "ALPHA")) && ((n2 = ALPHA (grammaire, lu, indice)) != NULL)){
			n1 = n2;	
		}
		if ((compareChaineStr(recupereMot(ligne, i, grammaire), "DIGIT")) && ((n2 = DIGIT(grammaire, lu, indice)) != NULL)){
			n1 = n2;	
		}


	}



	return n1;

}



//Fonction recursive qui rajoute des branches si elles sont coherentes
/*noeud *creerNoeud(string grammaire, string recu, noeud *n1){

	int verif = 0;
	string *fils;
	if (recu->taille == 1) {
		if ((compareChaineStr(grammaire, "HTAB")) && (compareChaineStr(recu, "	"))) {
			verif = 1;
		}
		if ((compareChaineStr(grammaire, "SP")) && (compareChaineStr(recu, " "))) {
			verif = 1;
		}
		if ((compareChaineStr(grammaire, "DIGIT")) && (isDigit(recupChar(recu, 0)))) {
			verif = 1;
		}
		if ((compareChaineStr(grammaire, "ALPHA")) && (isAlpha(recupChar(recu, 0)))) {
			verif = 1;
		}


	}
	if (recupereMot(0, 1, recu) == NULL) {
		if((recupChar(grammaire, 0) == '"') && (recupChar(grammaire, (grammaire -> taille) - 1) == '"')) {

		}

	while ()
	if (compareChaineStr("/")) {
		verif = verifieFormat() || verifieFormat()
	}
	if (compareChaineStr("*")){
		while (verif)
	}
	if (isAlpha())


}*/


/*noeud *creerArbreV2 (FILE *recu, FILE *grammaire, noeud *n1) {


	int i = 0;
	string *recherche;
	recherche = recupereMot(i, 0, grammaire);
	printf("OK");

	while ((recherche != NULL) && (!compareString(recherche, n1 -> nomChamp))) {
		printf("OK\n");
		i++;
		afficherString(recherche);
		recherche = recupereMot(i, 0, grammaire);
	}

	printf("OK1");
	int j = 0;
	while (recupereMot(i,j, grammaire) != NULL){
		j++;
	}

	printf("OK2");
	n1 -> nbFils = j;
	n1 -> filsTab = malloc(j*sizeof(*(n1 -> filsTab)));

	for (i=0; i<j; i++) {
		n1 -> filsTab[i] = malloc(sizeof(noeud));
		(n1 -> filsTab[i]) -> pere = n1;
		(n1 -> filsTab[i]) -> nomChamp = recupereMot(i,j, grammaire);
	}

	printf("OK3");
	return n1;
}*/





void *getRootTree() {

}


_Token *searchTree(void *start,char *name) {

}



char *getElementTag(void *node,int *len) {


}


char *getElementValue(void *node,int *len) {


}


void purgeElement(_Token **r) {


}


void purgeTree(void *root) {


}



int parseur(char *req, int len) {
	// A faire
	return 0;
}



int main() {
	
	FILE *grammaire;
	FILE *lu;
	int indice = 0;
	grammaire = fopen("grammaire.txt", "r");

	lu = fopen("test.txt", "r");

	noeud *n1;
	n1 = creerArbre(grammaire,lu, &indice, 12, 2, 2);
	if (n1 != NULL) afficherString(n1 -> nomChamp);
	if (n1 != NULL) afficherString(n1 -> valeurChamp);

	
	
	return 0;
}
