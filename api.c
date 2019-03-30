#include <stdio.h>
#include <stdlib.h>
#include "api.h"

typedef struct String {
	FILE * fichier;
	int depart;
	int taille;
} string;


typedef struct Arbre {
  struct Arbre **filsTab; //Tableau de pointeurs vers les fils
  struct Arbre *pere;  //Pointeur vers le pere
  int nbFils; //Pour connaitre le nombre de fils du tableau de pointeurs
  string *nomChamp;
  string *valeurChamp;
} noeud;


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


noeud *creerArbreV2 (/*FILE *recu, */FILE *grammaire, noeud *n1) {


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
}


/*noeud *creerArbre(FILE *recu, FILE *grammaire) {


	noeud *racine = malloc(sizeof(noeud));
	racine -> nomChamp = recupereMot(0,0,grammaire);


	while (rechercheMot(0,i, grammaire) != NULL) {
		fils

	}
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
	char test;
	string* chaineNulle = NULL;
	FILE *grammaire;

	grammaire = fopen("grammaire.txt", "r");

//	string *chaine = recupereMot(3, 2, grammaire);
//	string *chaine2 = recupereMot(2, 2, grammaire);
//	printf("premiere lettre: %d,	taille: %d\n", chaine->depart, chaine->taille);
//	afficherString(chaine);
//	printf("OK");
//	printf("%d\n", compareChaineStr(chaine, "URI"));
//	printf("OK");
	//compareChaineStr(chaineNulle, "segfault");
//	noeud *racine;
//	racine = malloc(sizeof(noeud));
//	printf("OK");
//	racine -> nomChamp = recupereMot(0,0,grammaire);
	afficherString(recupereMot(2196, 0, grammaire));
	printf("%d\n",rechercheMot("der", 6, grammaire));
//	printf("OK");
	//racine = creerArbreV2(grammaire, racine);
	//printf("Champ racine:	");
	//afficherString(racine -> nomChamp);
	
	return 0;
}
