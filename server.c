#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
// for librequest
#include "request.h"

// for parser

// this will declare internal type used by the parser
#include "httpparser.h"
#include "api.h"

#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\nHey Bro why did you send me this:\r\n"
#define M501 "HTTP/1.0 501 Not implemented\r\n\r\n"
#define M404 "HTTP/1.0 404 Not Found\r\n\r\n"


char convAscii (char hexa[3]){
	char c = 0;
	int i = 0;
	for (i = 0; i<2; i++){
		if ((hexa[i] >= 65) && (hexa[i] <= 70)) hexa[i] = hexa[i] - 55;
		else hexa[i] = hexa[i] - 48;
		c += hexa[i] * pow(16,1-i);
	}
	return c;
}


int message501(Lnode *root){

	_Token *r, *tok;
	r = searchTree(root,"method");
	tok = r;
	int retour = 1;
	Lnode* ln = (Lnode*)tok->node;
	if ((strcmp(ln->value, "GET") != 0) && (strcmp(ln->value, "POST") != 0)){
		retour = 0;

	}
	purgeElement(&r);
	return retour;



}

void init(char *chaine){
	int i = 0;
	while (chaine[i] != '\0'){
		chaine[i] = '\0';
		i++;
	}

}


char *dotSegmentRemoval(char *chaine){


	int score = 0;
	char *valeur = chaine;
	int taille = strlen(valeur);
	char *chaineDecode = malloc(taille * sizeof(char));
	char *sousChaine = malloc(taille*sizeof(char));
	int ecrire = 0;
	int j = 0;
	int k = 0;
	for (int i = 0; i < taille; i++){

		if ((ecrire) && (valeur[i] != '/')) sousChaine[j] = valeur[i];

		if (valeur[i] == '/'){
			if (strcmp(sousChaine, "..")) score--;
			else if (strcmp(sousChaine, "."));
			else score ++;

			if (score >= 0){
				j = 0;
				while (sousChaine[j] != '\0'){
					chaineDecode[k] = sousChaine[j];
					j++;
					k++;
				}

			}

		}
		init(sousChaine);
		j = 0;

	}

	return chaineDecode;



}


char * percentEncoding(char *chaine){

	int taille = strlen(chaine);
	char *chaineDecode = malloc(taille*sizeof(char));
	int i = 0;
	int j = 0;
	while (i < taille){

		if (chaine[i] == '%'){
			char hexa[3];
			hexa[0] = chaine[i+1];
			hexa[1] = chaine[i+2];
			chaineDecode[j] = convAscii(hexa); //fonction de grammaire.c
			i += 2;

		}
		else chaineDecode[j] = chaine[i];

		i++;
		j++;

	}

	return chaineDecode;

}


char * referenceTarget (Lnode *root){
	_Token *tok, *r;
	r = searchTree(root,"request-target");
	tok = r;
	Lnode* ln = (Lnode*)tok->node;
	char *chaine1 = ln->value;
	char *chaine2 = dotSegmentRemoval(chaine1);
	char *chaineTarget = percentEncoding(chaine2);
	char *chaineBase = "/var/www/";
	int tailleTarget = strlen(chaineTarget);
	int tailleBase = strlen(chaineBase);
	char *chaineComplete = malloc(tailleTarget + tailleBase);

	for (int i = 0; i < tailleBase; i++){
		chaineComplete[i] = chaineBase[i];
	}
	for (int i = 0; i < tailleTarget; i++){
		chaineComplete[tailleBase + i] = chaineTarget[i];
	}

	return chaineComplete;

}




int main(int argc, char *argv[])
{
	message *requete;
	_Token *r, *tok;
	void *root;
	while ( 1 ) {
		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
		if ((requete=getRequest(8080)) == NULL ) return -1;

		// Affichage de debug
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId);
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);
		if ((parseur(requete->buf,requete->len))==0) {

			// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h
			//Lnode *root;
			writeDirectClient(requete->clientId,ERROR,strlen(ERROR));
			root=getRootTree();

			/*//Erreur 501:
			if (message501(root)){
				writeDirectClient(requete->clientId,M501,strlen(M501));
			}*/



			}
			else{
				r = searchTree(root,"method");
				tok = r;

				Lnode* ln = (Lnode*)tok->node;
				if (strcmp(ln->value, "GET") == 0){


					char *chaineComplete = referenceTarget(root);
					FILE * target;
					if ((target = fopen(chaineComplete, "r"))!=NULL){




					}
					else{
						writeDirectClient(requete->clientId,M404,strlen(M404));

					}




				}
				else if (strcmp(ln->value, "POST") == 0){

				}

			}



			r=searchTree(root,"HTTP_message");
			tok=r;
			while (tok) {
				// node is no longer opaque
				Lnode *node;
				node=(Lnode *)tok->node;
				writeDirectClient(requete->clientId,node->value,node->len);
				tok=tok->next;
			}
			purgeElement(&r);
		purgeTree(root);
		//} else {
		writeDirectClient(requete->clientId,ERROR,strlen(ERROR));
		//}
		endWriteDirectClient(requete->clientId);
		requestShutdownSocket(requete->clientId);
	// on ne se sert plus de requete a partir de maintenant, on peut donc liberer...
	freeRequest(requete);
	}
	return (1);
}
