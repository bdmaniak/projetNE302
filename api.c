#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "api.h"
#include "grammaire.h"
#include "lecture.h"














void *getRootTree;



_Token *searchTree(void *start,char *name) {

	static _Token *t1 = NULL;
	static _Token *debut = NULL;
	noeud *n1 = start;
	static int depart = 0;


	

	while (n1 != NULL){


	
		if (compareChaineStr(n1->nomChamp,name)){


			if (!depart){
				t1 = malloc(sizeof(_Token));
				t1 -> node = n1;

			
				debut = t1;
				depart = 1;

			}
			else{
				t1 -> next = malloc(sizeof(_Token));
				t1 = t1 -> next;
				t1 -> node = n1; 


			}
			

			


		}
		debut = searchTree(n1->fils, name);
		n1 = n1 -> frere;



	}

	return debut;

}



char *getElementTag(void *node,int *len) {

	
	noeud *n1 = node;
	char *valeur = NULL;
	int taille = 0;
	if (n1 != NULL){
		taille = (n1->nomChamp)->taille;
		if (len != NULL) *len = taille;
		valeur = malloc(taille*sizeof(char));
		for (int i=0; i < taille; i++){
			valeur[i] = lire((n1->nomChamp)->fichier, (n1->nomChamp)->depart + i);

		}
	}
	return valeur;


}


char *getElementValue(void *node,int *len) {

	noeud *n1 = node;
	char *valeur = NULL;
	int taille = 0;
	if (n1 != NULL){
		taille = (n1->valeurChamp)->taille;
		if (len != NULL) *len = taille;
		valeur = malloc(taille*sizeof(char));
		for (int i=0; i < taille; i++){
			valeur[i] = lire((n1->valeurChamp)->fichier, (n1->valeurChamp)->depart + i);

		}
	}
	return valeur;


}


void purgeElement(_Token **r) {


}


void purgeTree(void *root) {


}


int parseur(char *req, int len){


	FILE *requete;
	FILE *grammaire;
	requete = fopen("requete.txt","w+");
	grammaire = fopen("grammaire.txt","r");

	int indice = 0;
	fprintf(requete, "%s", req);
	getRootTree = creerArbre(grammaire, requete, &indice, 0, 0, 0);

	return 0;


}




int main() {
	
	//FILE *grammaire;
	//FILE *lu;
	int indice = 0;
	char req[]="GET / HTTP/1.0\r\nHost: www.google.com\r\nTransfer-Encoding: gzip\r\n\r\n";
	//grammaire = fopen("grammaire.txt", "r");

	//lu = fopen("test.txt", "r");

	//noeud *n1;
	//n1 = creerArbre(grammaire,lu, &indice, 0, 0, 0);
	printf("OK1\n");
	if (parseur(req, 15) == -1){
		exit(1);
	}
	printf("OK2\n");

	_Token *t1;
	t1 = searchTree(getRootTree, "case_insensitive_string");
	printf("ALPHA :%p\n",t1);
	noeud *n2;
	int *len = malloc(sizeof(int));
	*len = 0;
	while (t1 != NULL){
		n2 = t1 -> node;
		printf("nom: %s\n",getElementTag(n2, len));
		
		printf("valeur: %s\n",getElementValue(n2, len));
		printf("taille: %d\n", *len);
		t1 = t1 -> next;
	}

	afficherArbreBasic(getRootTree);
	//fclose(grammaire);
	//fclose(lu);
	
	return 0;
}
