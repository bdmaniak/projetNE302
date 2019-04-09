#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "api.h"
#include "grammaire.h"
#include "lecture.h"


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
	if((*r)->next){
		purgeElement(&(*r)->next);
	}
	free(*r);
}


void purgeTree(void *root) {

	noeud *n1;
	n1 = root;


	if(n1 != NULL){
		purgeTree(n1->frere);

		purgeTree(n1->fils);

		free(n1);

	}
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
	FILE *grammaire;
	
	char req[]="GET / HTTP/1.1\r\nHost: www.cvedetails.com\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0\r\nAccept:		 text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language:	fr,fr-FR;q=0.8,en-US;q=0.5,en;q=0.3\r\nAccept-Encoding: *   ; q=0.876\r\nConnection: tryit\r\n\r\n";
	grammaire = fopen("grammaire.txt", "r");
	

	//Si on veut tester directement une requete qui est dans un fichier
	//FILE *lu;
	//lu = fopen("test.txt", "r");
	//
	//getRootTree = creerArbre(grammaire,lu, &indice, 0, 0, 0);


	//Appel du parseur
	if (parseur(req, 15) == -1){
		exit(1);
	}

	//Creer le token de tous les case_insensitive_tree
	_Token *t1;
	t1 = searchTree(getRootTree, "case_insensitive_string");
	noeud *n2;
	int *len = malloc(sizeof(int));
	*len = 0;

	//Permet d'affichr chaque element du token avec son nom, sa valeur et sa taille
	while (t1 != NULL){
		n2 = t1 -> node;
		printf("nom: %s\n",getElementTag(n2, len));
		printf("valeur: %s\n",getElementValue(n2, len));
		printf("taille: %d\n", *len);
		t1 = t1 -> next;
	}

	//Affiche l'arbre puis libère la mémoire
	afficherArbre(getRootTree);
	printf("Appel à la fonction purgeTree ...\n\n");
	purgeTree(getRootTree);
	getRootTree = NULL; 	//Seul le noeud racine reste afficher si l'on rajoute pas cette ligne.
	printf("Affichage de l'arbre ...\n");
	afficherArbre(getRootTree);
	fclose(grammaire);
	//fclose(lu);
	return 0;
}
