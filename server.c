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
#define M501 "HTTP/1.0 501 Not implemented\r\n"
#define M400 "HTTP/1.0 400 Bad Request\r\n"
#define M404 "HTTP/1.0 404 Not Found\r\n"
#define M200 "HTTP/1.0 200 OK\r\n"


int main(int argc, char *argv[])
{
	message *requeteRecu; 
	int res; 
	actualisation = malloc(sizeof(int));
	*actualisation = 0;
	while ( 1 ) {


		_Token *tok = NULL;
		_Token *r = NULL;
		noeud *n = NULL; 
		enTete *et1 = malloc(sizeof(enTete));
		initEnTete(et1);
		

		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requeteRecu=getRequest(8080)) == NULL ) return -1; 

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requeteRecu->clientId); 
		printf("Client [%d] [%s:%d]\n",requeteRecu->clientId,inet_ntoa(requeteRecu->clientAddress->sin_addr),htons(requeteRecu->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requeteRecu->len,requeteRecu->buf);  
		if ((res=parseur(requeteRecu->buf,requeteRecu->len)) == 0) {
			
			
			// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h 
			//void *root; 
			printf("OOOOOOK\n");
			reponseServeur(400, et1, NULL, requeteRecu);
			

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

			if (compareChaineStr(n->valeurChamp, "GET") == 1){
					
				printf("OK3\n");
				char *chaineComplete = referenceTarget(root);
				printf("chaine: %s\n", chaineComplete);
				FILE * target;
				if (target = fopen(chaineComplete, "r")){
					printf("OK4\n");
					reponseServeur(200, et1, NULL, requeteRecu);
					



				}
				else{
					printf("OK5\n");
					reponseServeur(404, et1, NULL, requeteRecu);

				}
					

					
					
			}
			else if (compareChaineStr(n->valeurChamp, "POST") == 1){
				printf("OK6\n");
			}
			else{
				reponseServeur(501, et1, NULL, requeteRecu);
			}
			printf("OK6Bis\n");
		
		}
			
		void *root = getRootTree;
		printf("OK7\n");
		r=searchTree(root,"HTTP_message"); 
		n= (noeud *) r->node; 
		printf("valeur2: %s\n", convString(n->valeurChamp));
		while (tok) {
			int l; 
			char *s; 
			// node is no longer opaque 
			writeDirectClient(requeteRecu->clientId,convString(n->valeurChamp),(n->valeurChamp)->taille);
			tok=tok->next; 
		}
		purgeElement(&r); 
		purgeTree(root); 
		/*if (1 test des erreurs) {
			writeDirectClient(requeteRecu->clientId,ERROR,strlen(ERROR)); 
		}*/
		endWriteDirectClient(requeteRecu->clientId); 
		requestShutdownSocket(requeteRecu->clientId); 
		// on ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requeteRecu);
		fclose(grammaire);
		fclose(requete);
	}
	return (1);
}




int message501(void *root){

	_Token *r = NULL;
	noeud *n = NULL;
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
	

	int continuer = 1;
	int score = -1;
	
	int taille = strlen(chaine) + 1;
	char *chaineDecode = malloc(taille * sizeof(char));
	init(chaineDecode);
	char *sousChaine = malloc(taille*sizeof(char));
	init(sousChaine);
	int j = 0;
	int k = 0;
	for (int i = 0; i < taille; i++){

		sousChaine[j] = chaine[i];
		j++;


		if ((chaine[i] == '/') || ((chaine[i] == '\0') && (chaine[i - 1] != '/'))){
			printf("sousChaine: %s\n", sousChaine);
			
			if (strcmp(sousChaine, "../") == 0) score--;
			else if (strcmp(sousChaine, "./") == 0);
			else if (strcmp(sousChaine, "/") == 0);
			else if (strcmp(sousChaine, "") == 0);
			else score ++;
			printf("score: %d\n", score);
			if (score >= 0){
				j = 0;
				while (sousChaine[j] != '\0'){
					chaineDecode[k] = sousChaine[j];
					j++;
					k++;
				}

			}
			init(sousChaine);
			j = 0;

		}
		

	}

	return chaineDecode;
	


}


char * percentEncoding(char *chaine){
	
	
	int taille = strlen(chaine) + 1;
	char *chaineDecode = malloc(taille*sizeof(char));
	init(chaineDecode);
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


char * referenceTarget (void *root){

	_Token *r = NULL;
	noeud * n = NULL;
	
	*actualisation = 0;
	r = searchTree(root,"request-target");
	n = (noeud *) r->node;
	afficherString(n->valeurChamp);
	char *chaine1 = convString(n->valeurChamp);
	printf("target: %s\n", chaine1);
	char *chaine2 = dotSegmentRemoval(chaine1);
	printf("DSR: %s\n", chaine2);
	char *chaineTarget = percentEncoding(chaine2);
	printf("PE: %s\n", chaineTarget);
	char *chaineBase = "/var/www/";
	int tailleTarget = strlen(chaineTarget) + 1;
	int tailleBase = strlen(chaineBase);
	char *chaineComplete = malloc(tailleTarget + tailleBase);
	init(chaineComplete);

	for (int i = 0; i < tailleBase; i++){
		chaineComplete[i] = chaineBase[i];
	}
	for (int i = 0; i < tailleTarget; i++){
		chaineComplete[tailleBase + i] = chaineTarget[i];
		
	}

	return chaineComplete;

}

void reponseServeur(int code, enTete *et1, char * msgBody, message *requeteRecu){
	if (code == 200) writeDirectClient(requeteRecu->clientId,M200,strlen(M200));
	printf("ok: %d\n", (code == 200));
	if (code == 404) writeDirectClient(requeteRecu->clientId,M404,strlen(M404));
	if (code == 501) writeDirectClient(requeteRecu->clientId,M501,strlen(M501));

	if (et1->contentEncoding != NULL) writeDirectClient(requeteRecu->clientId,et1->contentEncoding,strlen(et1->contentEncoding));
	if (et1->contentLanguage != NULL) writeDirectClient(requeteRecu->clientId,et1->contentLanguage,strlen(et1->contentLanguage));
	if (et1->contentLength != NULL) writeDirectClient(requeteRecu->clientId,et1->contentLength,strlen(et1->contentLength));
	if (et1->contentType != NULL) writeDirectClient(requeteRecu->clientId,et1->contentType,strlen(et1->contentType));
	if (et1->date != NULL) writeDirectClient(requeteRecu->clientId,et1->date,strlen(et1->date));
	if (et1->expires != NULL) writeDirectClient(requeteRecu->clientId,et1->expires,strlen(et1->expires));
	if (et1->forwarded != NULL) writeDirectClient(requeteRecu->clientId,et1->forwarded,strlen(et1->forwarded));
	if (et1->location != NULL) writeDirectClient(requeteRecu->clientId,et1->location,strlen(et1->location));
	if (et1->server != NULL) writeDirectClient(requeteRecu->clientId,et1->server,strlen(et1->server));
	writeDirectClient(requeteRecu->clientId,"\r\n",strlen("\r\n"));
	if (msgBody != NULL) writeDirectClient(requeteRecu->clientId,msgBody,strlen(msgBody));
	


}

void initEnTete(enTete *et1){
	et1->contentEncoding = NULL;
	et1->contentLanguage = NULL;
	et1->contentLength = NULL;
	et1->contentType = NULL;
	et1->date = NULL;
	et1->expires = NULL;
	et1->forwarded == NULL;
	et1->location == NULL;
	et1->server == NULL;
}







