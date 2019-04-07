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

	afficherArbreBasic(n1);
	
	
	return 0;
}
