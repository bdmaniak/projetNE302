#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "api.h"
#include "grammaire.h"
#include "lecture.h"














void *getRootTree() {

	

}


_Token *searchTree(void *start,char *name) {

	static _Token *t1 = NULL;
	static _Token *debut = NULL;
	noeud *n1 = start;
	static int depart = 0;

	//TEST
	static int numFils = 0;
	int numFrere = 0;
	numFils ++;
	

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
		numFrere++;


	}

	return debut;

}



char *getElementTag(void *node,int *len) {


}


char *getElementValue(void *node,int *len) {


}


void purgeElement(_Token **r) {


}


void purgeTree(void *root) {


}




int main() {
	
	FILE *grammaire;
	FILE *lu;
	int indice = 0;
	grammaire = fopen("grammaire.txt", "r");

	lu = fopen("test.txt", "r");

	noeud *n1;
	n1 = creerArbre(grammaire,lu, &indice, 0, 0, 0);

	_Token *t1;
	t1 = searchTree(n1, "case_insensitive_string");
	printf("ALPHA :%p\n",t1);
	noeud *n2;
	while (t1 != NULL){
		n2 = t1 -> node;
		afficherString(n2->valeurChamp);
		t1 = t1 -> next;
	}

	afficherArbreBasic(n1);
	
	
	return 0;
}
