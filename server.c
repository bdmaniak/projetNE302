#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

// for librequest 
#include "request.h"   

// for parser 

// this will declare internal type used by the parser

#include "api.h"
#include "./Parseur/lecture.h"
#include "./Parseur/grammaire.h"
#include "server.h"

#define MAX_SIZE 1000000
#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\nHey Bro why did you send me this:\r\n"
#define M501 "HTTP/1.0 501 Not implemented\r\n"
#define M400 "HTTP/1.0 400 Bad Request\r\n"
#define M404 "HTTP/1.0 404 Not Found\r\n"
#define M200 "HTTP/1.0 200 OK\r\n"


int main(int argc, char *argv[]){
	message *requeteRecu;
	int res; 
	char *buf = malloc(MAX_SIZE * sizeof(char));
	
	enTete *et1 = malloc(sizeof(enTete));
	
	while ( 1 ) {//Le serveur attend en permanence des requetes

		init(buf);
		_Token *tok = NULL;
		_Token *r = NULL;
		noeud *n = NULL; 

		
		initEnTete(et1);
		

		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest. 
		if ((requeteRecu=getRequest(8080)) == NULL ) return -1; 

		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requeteRecu->clientId); 
		printf("Client [%d] [%s:%d]\n",requeteRecu->clientId,inet_ntoa(requeteRecu->clientAddress->sin_addr),htons(requeteRecu->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requeteRecu->len,requeteRecu->buf);


		//Si syntaxiquement faux: erreur 400  
		if ((res=parseur(requeteRecu->buf,requeteRecu->len)) == 0) {
			
			
			// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h 
			//void *root; 

			reponseServeur(400, et1, "Syntax error\n", requeteRecu);
			

			

		}
		//Verifications en-têtes:

		/*else if ((searchTree(getRootTree, "Content-Length-header")) && (searchTree(getRootTree, "Transfert-Encoding-header"))){
			reponseServeur(400, et1, "Cannot associate Transfert-Encoding-header with Content-Length-header\n", requeteRecu);
			afficherArbre(getRootTree);
			//printf("pointeur: %ld\n", sizeof(searchTree(getRootTree, "Content-Length-header")));

		}*/
		else if (verification(et1, requeteRecu)/*!searchTree(getRootTree, "Host-header")*/); //reponseServeur(400, et1, "Host-header is missing\n", requeteRecu);
	
		
		//Sinon requete valable syntaxiquement
		else{
			void *root = getRootTree;
			r = searchTree(root,"method");
			tok = r;
			n = (noeud *) tok->node;
			ajouterHeure(et1);

			

			//Methode GET
			if (compareChaineStr(n->valeurChamp, "GET") == 1){
				_Token * r1 = searchTree(root, "message-body");
				noeud * n1 = (noeud *) r1->node;
				if ((((noeud *)(searchTree(root, "message-body")->node))->valeurChamp)->taille) reponseServeur(400, et1, "Message body in a GET request\n", requeteRecu);
				else{
					char *chaineComplete = referenceTarget(root);
					FILE * target;
					if (target = fopen(chaineComplete, "r")){
						int k = 0;
						char c;
						while((c = getc(target)) != EOF){
							//Recopier contenu dans buf
							buf[k] = c;
							k++;	
						}
						reponseServeur(200, et1, buf, requeteRecu);
						fclose(target);
					}
					else{
						reponseServeur(404, et1, NULL, requeteRecu);
					}
				}
			}
			

			//Methode POST
			else if (compareChaineStr(n->valeurChamp, "POST") == 1){
				printf("POST\n");
			}

			
			
			//Methode HEAD
			else if (compareChaineStr(n->valeurChamp, "HEAD") == 1){

				if ((((noeud *)(searchTree(root, "message-body")->node))->valeurChamp)->taille) reponseServeur(400, et1, "Message body in a HEAD request\n", requeteRecu);

				else{

					char *chaineComplete = referenceTarget(root);
					FILE * target;
					if (target = fopen(chaineComplete, "r")){
						reponseServeur(200, et1, NULL, requeteRecu);
						fclose(target);
	
					}
					else{
						reponseServeur(404, et1, NULL, requeteRecu);
	
					}
				}
			}
			
			else{
				reponseServeur(501, et1, NULL, requeteRecu);
			}
			if (r != NULL) purgeElement(&r); 
			if (root != NULL) purgeTree(root);
			

		}
			/*void *root = getRootTree;
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
			}*/

		 
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





void init(char *chaine){
	int i = 0;
	for(i = 0; i < sizeof(chaine); i++){
		chaine[i] = '\0';
	}

}


char *dotSegmentRemoval(char *chaine){
	

	int continuer = 1;
	int score = 1;
	
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
			
			if ((strcmp(sousChaine, "../") == 0) && (score > 0)) score--;
			else if (strcmp(sousChaine, "./") == 0);
			else if (strcmp(sousChaine, "/") == 0);
			else if (strcmp(sousChaine, "") == 0);
			else score ++;
			if (score > 0){
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

	_Token *r1 = NULL;
	noeud * n1 = NULL;
	
	_Token *r2 = NULL;
	noeud * n2 = NULL;
	r1 = searchTree(root,"request-target");
	n1 = (noeud *) r1->node;
	r2 = searchTree(root, "host");
	n2 = (noeud *) r2->node;
	char * chaineHost = convString(n2 -> valeurChamp);
	char *chaine1 = convString(n1->valeurChamp);
	char *chaine2 = dotSegmentRemoval(chaine1);
	char *chaineTarget = percentEncoding(chaine2);
	char *chaineBase = "/var/www/";
	int tailleTarget = strlen(chaineTarget) + 1;
	int tailleBase = strlen(chaineBase);
	int tailleHost = strlen(chaineHost);
	char *chaineComplete = NULL;
	chaineComplete = malloc(tailleTarget + tailleBase + tailleHost);
	init(chaineComplete);
	init(chaineComplete);
	for (int i = 0; i < tailleBase; i++){
		chaineComplete[i] = chaineBase[i];
	}
	for(int i = 0; i < tailleHost; i++){
		chaineComplete[tailleBase + i] = chaineHost[i]; 
	}
	for (int i = 0; i < tailleTarget; i++){
		printf("%x\n",chaineComplete[tailleBase + tailleHost + i] = chaineTarget[i]);//Bug WSL, un caractere 'a' est insere (saut de ligne)
	}
	printf("Retour : %s\n",chaineComplete);
	return chaineComplete;

}

void reponseServeur(int code, enTete *et1, char *msgBody, message *requeteRecu){
	if (code == 200) writeDirectClient(requeteRecu->clientId,M200,strlen(M200));
	if (code == 400) writeDirectClient(requeteRecu->clientId,M400,strlen(M400));
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



void ajouterHeure(enTete *et1){
	const char * nomJour[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

	char * dateCourante = malloc(37 * sizeof(char));

	time_t timestamp;
	struct tm *t;
	timestamp = time(NULL);
	t = localtime(&timestamp);
	dateCourante[0] = 'D';
	dateCourante[1] = 'a';
	dateCourante[2] = 't';
	dateCourante[3] = 'e';
	dateCourante[4] = ':';
	dateCourante[5] = ' ';
	for(int i = 6; i<9 ; i++){
		dateCourante[i] = nomJour[t->tm_wday][i - 6];
	}
	dateCourante[9] = ',';
	dateCourante[10] = ' ';
	dateCourante[11] = ((t->tm_mday)/10) + 48;
	dateCourante[12] = ((t->tm_mday) % 10) + 48;
	dateCourante[13] = ' ';
	dateCourante[14] = ((1 + t->tm_mon)/10) + 48;
	dateCourante[15] = ((1 + t->tm_mon) % 10) + 48;
	dateCourante[16] = ' ';
	dateCourante[17] = ((1900 + t->tm_year) / 1000) + 48;
	dateCourante[18] = (((1900 + t->tm_year) / 100) % 10) + 48;
	dateCourante[19] = (((1900 + t->tm_year) / 10) % 10) + 48;
	dateCourante[20] = ((1900 + t->tm_year) % 10) + 48;
	dateCourante[21] = ' ';
	dateCourante[22] = ((t->tm_hour) / 10) + 48;
	dateCourante[23] = ((t->tm_hour) % 10) + 48;
	dateCourante[24] = ':';
	dateCourante[25] = ((t->tm_min) / 10) + 48;
	dateCourante[26] = ((t->tm_min) % 10) + 48;
	dateCourante[27] = ':';
	dateCourante[28] = ((t->tm_sec) / 10) + 48;
	dateCourante[29] = ((t->tm_sec) % 10) + 48;
	dateCourante[30] = ' ';
	dateCourante[31] = 'G';
	dateCourante[32] = 'M';
	dateCourante[33] = 'T';
	dateCourante[34] = '\r';
	dateCourante[35] = '\n';
	dateCourante[36] = '\0';

	et1 -> date = dateCourante;


}


int verification(enTete *et1, message * requeteRecu){

	int envoye = 0;

	if ((!envoye) && (searchTree(getRootTree, "Content-Length-header")) && (searchTree(getRootTree, "Transfert-Encoding-header"))){
		reponseServeur(400, et1, "Cannot associate Transfert-Encoding-header with Content-Length-header\n", requeteRecu);
		envoye = 1;
		//printf("pointeur: %ld\n", sizeof(searchTree(getRootTree, "Content-Length-header")));

	}

	_Token * rVersion = searchTree((searchTree(getRootTree,"HTTP-version")->node), "DIGIT");
	noeud * nVersion = (noeud * ) rVersion -> node;
	char chiffre1 = (convString(nVersion -> valeurChamp))[0];
	printf("chiffre1: %c\n", chiffre1);
	nVersion = (noeud * ) (rVersion -> next) -> node;
	char chiffre2 = (convString(nVersion  -> valeurChamp))[0];
	printf("chiffre2: %c\n", chiffre2);
	if ((!envoye) && (!searchTree(getRootTree, "Host-header")) && (chiffre1 == '1') && (chiffre2 == '1')){
		reponseServeur(400, et1, "Host-header is missing\n", requeteRecu);
		envoye = 1;
	}
	rVersion = searchTree(getRootTree, "Host");
	if ((!envoye) && (rVersion -> node) && (rVersion -> next)){
		reponseServeur(400, et1, "There is more than one Host\n", requeteRecu);
		envoye = 1;
	}
	return envoye;	
}




