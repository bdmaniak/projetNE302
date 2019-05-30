#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// for librequest 
#include "request.h"   

// for parser 

// this will declare internal type used by the parser

#include "api.h"
#include "./Parseur/lecture.h"
#include "./Parseur/grammaire.h"
#include "server.h"

#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\nHey Bro why did you send me this:\r\n"
#define M501 "HTTP/1.0 501 Not implemented\r\n\r\n"
#define M404 "HTTP/1.0 404 Not Found\r\n\r\n"

int main(int argc, char *argv[])
{
	message *requeteRecu; 
	int res; 
	actualisation = malloc(sizeof(int));
	*actualisation = 0;
	while ( 1 ) {


		_Token *tok, *r;
		noeud *n; 
		

		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requeteRecu=getRequest(8080)) == NULL ) return -1; 

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requeteRecu->clientId); 
		printf("Client [%d] [%s:%d]\n",requeteRecu->clientId,inet_ntoa(requeteRecu->clientAddress->sin_addr),htons(requeteRecu->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requeteRecu->len,requeteRecu->buf);  
		if ((res=parseur(requeteRecu->buf,requeteRecu->len)) == 0) {
			
			
			// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h 
			//void *root; 
			writeDirectClient(requeteRecu->clientId,ERROR,strlen(ERROR)); 
			

			/*//Erreur 501:
			if (message501(root)){
				writeDirectClient(requete->clientId,M501,strlen(M501));
			}*/
				


		}
		else{
			void *root = getRootTree;
			printf("OK1\n");
			r = searchTree(root,"method");
			printf("OK2\n");
			tok = r;
			n = (noeud *) tok->node;
			printf("root: %p\n",root);
			printf("n: %p\n",n);
			printf("r: %p\n",r);
			printf("n: %p\n",n);
			printf("OK2Bis\n");
			printf("valeur n: %s\n", convString(n->valeurChamp));
	
				
			if (compareChaineStr(n->valeurChamp, "GET") == 0){
					
				printf("OK3\n");
				char *chaineComplete = referenceTarget(root);
				printf("chaine: %s\n", chaineComplete);
				FILE * target;
				if (target = fopen(chaineComplete, "r")){
					printf("OK4\n");



				}
				else{
					printf("OK5\n");
					writeDirectClient(requeteRecu->clientId,M404,strlen(M404));

				}
					

					
					
			}
			else if (compareChaineStr(n->valeurChamp, "POST") == 0){
				printf("OK6\n");
			}
			printf("OK6Bis\n");
		
		}
			
		void *root = getRootTree;
		printf("OK7\n");
		r=searchTree(root,"HTTP_message"); 
		n= (noeud *) r->node; 
		while (tok) {
			int l; 
			char *s; 
			// node is no longer opaque 
			writeDirectClient(requeteRecu->clientId,convString(n->valeurChamp),(n->valeurChamp)->taille);
			tok=tok->next; 
		}
		purgeElement(&r); 
		purgeTree(root); 
		if (1 /*test des erreurs*/) {
			writeDirectClient(requeteRecu->clientId,ERROR,strlen(ERROR)); 
		}
		endWriteDirectClient(requeteRecu->clientId); 
		requestShutdownSocket(requeteRecu->clientId); 
		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requeteRecu);
		fclose(grammaire);
		fclose(requete);
	}
	return (1);
}


int message501(noeud *root){

	_Token *r;
	noeud *n;
	r = searchTree(root,"method");
	n = (noeud *) r->node;
	int retour = 1;
	if ((compareChaineStr(n->valeurChamp, "GET") != 0) && (compareChaineStr(n->valeurChamp, "POST") != 0)){
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
	
	_Token *r;
	noeud * n;
	noeud * root = getRootTree;
	r = searchTree(root,"request-target");
	n = (noeud *) r->node;
	int continuer = 1;
	int score = 0;
	
	char *valeur = convString(n->valeurChamp);
	int taille = strlen(valeur);
	char *chaineDecode = malloc(taille * sizeof(char));
	char *sousChaine = malloc(taille*sizeof(char));
	int ecrire = 0;
	int j = 0;
	int k = 0;
	for (int i = 0; i < taille; i++){

		if ((ecrire) && (valeur[i] != '/')) sousChaine[j] = valeur[i]; 

		if (valeur[i] == '/'){
			if (strcmp(sousChaine, "..") == 0) score--;
			else if (strcmp(sousChaine, ".") == 0);
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


char * referenceTarget (noeud *root){

	_Token *r;
	noeud * n;
	r = searchTree(root,"request-target");
	n = (noeud *) r;
	char *chaine1 = convString(n->valeurChamp);
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

