/************************************************************************
Information utile
*************************************************************************
Pour acceder a un site depuis un navigateur (teste avec Firefox et Edge),
taper dans la barre d'adresse :
127.0.0.1/<adresse de la ressource dans le repertoire DOSSIER_SERVER/127.0.0.1>
Exemple : pour acceder au site www.serpent.com il faut taper dans la
barre d'adresse http://127.0.0.1/www.serpent.com/index.html , si le site
est declare dans le dossier DOSSIER_SERVER.
Exemple 2 : Pour accéder au site où le index.html est dans DOSSIER_SERVER/127.0.0.1/site,
taper dans la barre d'addresse : http://127.0.0.1/site/index.html 
************************************************************************/


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

// Pour socket.h
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Pour gérer le php
#include "fastcgi.h"
#include "socket.h"

// Test
#define sendParams(fd,id,params,len) sendWebData(fd,FCGI_PARAMS,id,params,len)
#define sendStdin(fd,id,stdin,len) sendWebData(fd,FCGI_STDIN,id,stdin,len)
#define sendData(fd,id,data,len) sendWebData(fd,FCGI_DATA,id,data,len)


// Constantes

#define DOSSIER_SERVER "./www/" //Dossier dans lequel les sites du serveur sont stockes
#define MAX_SIZE 100000000 // Définit une taille maximale pour l'envoit de fichier (on ne peut pas envoyer un fichier de 10 Go)
#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\nHey Bro why did you send me this:\r\n"
#define M501 "HTTP/1.0 501 Not implemented\r\n"
#define M400 "HTTP/1.0 400 Bad Request\r\n"
#define M404 "HTTP/1.0 404 Not Found\r\n"
#define M200 "HTTP/1.0 200 OK\r\n"


int main(int argc, char *argv[]) {
	message *requeteRecu; 
	int res; 
	int taille = 0;
	char *buf = malloc(MAX_SIZE * sizeof(char));
	
	// On alloue de la memoire pour les en-tetes
	enTete *et1 = malloc(sizeof(enTete));
	
	while (1) {

		init(buf, MAX_SIZE);
		_Token *tok = NULL;
		_Token *r = NULL;
		noeud *n = NULL; 

		// Fonction d'initialisation des en-tetes
		initEnTete(et1);
		ajouterHeure(et1); // Pour le champs Date de l'entête		

		// on attend la reception d'une requete HTTP requete pointera vers une ressource allouÃ©e par librequest. 
		if ((requeteRecu=getRequest(8080)) == NULL ) return -1; 

		// Affichage informatif du serveur 
		printf("#########################################\nDemande recue depuis le client %d\n",requeteRecu->clientId); 
		printf("Client [%d] [%s:%d]\n",requeteRecu->clientId,inet_ntoa(requeteRecu->clientAddress->sin_addr),htons(requeteRecu->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requeteRecu->len,requeteRecu->buf);


		//Si syntaxiquement faux: erreur 400  
		if ((res=parseur(requeteRecu->buf,requeteRecu->len)) == 0) {			
			// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h 
			//void *root; 
			reponseServeur(400, et1, "Syntax error\n", requeteRecu, 0);
		}

		//Verifications en-tetes:
		else if (verification(et1, requeteRecu)/*!searchTree(getRootTree, "Host-header")*/); //reponseServeur(400, et1, "Host-header is missing\n", requeteRecu);
	
		
		//Sinon requete valable syntaxiquement
		else {
			void *root = getRootTree;
			r = searchTree(root,"method");
			tok = r;
			n = (noeud *) tok->node;
			

			//Methode GET
			if (compareChaineStr(n->valeurChamp, "GET") == 1) {
				_Token * r1 = searchTree(root, "message-body");
				noeud * n1 = (noeud *) r1->node;
				if ((((noeud *)(searchTree(root, "message-body")->node))->valeurChamp)->taille) reponseServeur(400, et1, "Message body in a GET request\n", requeteRecu, 0);

				else {
					char *chaineComplete = referenceTarget(root);
					int mLen = 0;
					if (chaineComplete != NULL) mLen = strlen(chaineComplete);

					// AJOUT PHP ATTENTION CA PEUT EXPLOSER
					if ((chaineComplete[mLen-1] == 'p' && chaineComplete[mLen-2] == 'h' && chaineComplete[mLen-3] == 'p' && chaineComplete[mLen-4] == '.')) {
						printf("\nON EST DANS LE IF PHP AHAHAH\n");
						// Là ça va exploser
						int fd;
						if((fd = createSocket(9000)) == -1) {
							printf("ERREUR lors de la création de la socket.\n");
						}
						printf("ON A CREE LA SOCKET AHAHAH\n");

						sendBeginRequest(fd,1,FCGI_RESPONDER,FCGI_KEEP_CONN);  // A voir s'il faut FCGI_KEEP_CONN
						printf("ON A BEGIN LA REQUEST AHAHAH\n");

						// On crée un FCGI_Header pour les paramètres
						FCGI_Header h;
						h.version = FCGI_VERSION_1; 
						h.type = FCGI_PARAMS;
						h.requestId = htons(1); 
						h.contentLength = 0;
						h.paddingLength = 0;

						printf("LE DATA AVANT : %s\n", h.contentData);
						addNameValuePair(&h, "HTTP_HOST", "127.0.0.1");
						addNameValuePair(&h, "HTTP_PLOP", "HIHIHI");
						printf("LE DATA APRES : %s\n", h.contentData);
						printf("LA TAILLE DU DATA EST DE %d AHAHAH\n", h.contentLength);
						
						sendParams(fd, 1, h.contentData, h.contentLength);
						printf("ON A ENVOYE LES PARAMETRES AHAHAH\n");
						sendParams(fd, 1, "", 0);
						printf("ON A ENVOYE LES PARAMETRES VIDES AHAHAH\n");

						sendStdin(fd, 1 , "", 0);
						printf("ON A ENVOYE DU VIDE DANS LE STDIN AHAHAH\n");
						printf("JE SAIS PAS COMMENT RECUP STDOUT MAIS ON TEST AHAHAH\n");
						ssize_t lect = -1;
						lect = read(fd, h.contentData, FASTCGILENGTH);
						printf("On a lu %d caractères et on a \"%s\"...\n", lect, h.contentData);
						printf("TROP FORT AHAHAH\n");

						_Token * rMB = NULL;
						rMB = searchTree(getRootTree, "message-body");
						noeud * nMB = NULL;
						if (rMB != NULL) nMB = (noeud *) (rMB->node);
						char *MB = NULL;
						if (nMB != NULL) MB = convString(nMB->valeurChamp);

					}
					

					// On gère le Content-Type
					//On reconnait un fichier par son extension. Ce programme ne fonctionnera pas si l'extension n'est pas correcte.
					contentType(chaineComplete, mLen, et1);
			
					FILE * target;
					if ((target = fopen(chaineComplete, "rb"))) {
						size_t taille;
						fseek(target, 0, SEEK_END);
						taille = ftell(target);
						rewind(target);

						fread(buf, 1, taille, target);
					
						reponseServeur(200, et1, buf, requeteRecu, taille);
						fclose(target);
					}
					else{
						reponseServeur(404, et1, NULL, requeteRecu, taille);
					}
				}	
			}

			//Methode POST
			else if (compareChaineStr(n->valeurChamp, "POST") == 1){
				reponseServeur(501, et1, NULL, requeteRecu, 0);
			}

			//Methode HEAD
			else if (compareChaineStr(n->valeurChamp, "HEAD") == 1){

				if ((((noeud *)(searchTree(root, "message-body")->node))->valeurChamp)->taille) reponseServeur(400, et1, "Message body in a HEAD request\n", requeteRecu, 0);

				else{

					char *chaineComplete = referenceTarget(root);
					FILE * target;
					if ((target = fopen(chaineComplete, "rb"))){
						size_t taille; // Taille du body que l'on aurait envoyé si c'était un GET (sert pour Content-Length)
						fseek(target, 0, SEEK_END);
						taille = ftell(target);
						rewind(target);

						reponseServeur(200, et1, NULL, requeteRecu, taille);
						fclose(target);
					}
					else{
						reponseServeur(404, et1, NULL, requeteRecu, taille);
					}
				}
			}
			else{
				reponseServeur(501, et1, NULL, requeteRecu, taille);
			}
			if (r != NULL) purgeElement(&r); 
			if (root != NULL) purgeTree(root);
		}
			
		endWriteDirectClient(requeteRecu->clientId); 
		requestShutdownSocket(requeteRecu->clientId); 
		// On ne se sert plus de requete a partir de maintenant, on peut donc liberer... 
		freeRequest(requeteRecu);
		if (grammaire != NULL) {
			fclose(grammaire);
		}
		if (requete != NULL) {
			fclose(requete);
		}
	}


	// On pense à libérer la mémoire à la fin
	free(et1);
	free(buf);
	
	return (1);
}





void init(char *chaine, int taille){
	int i = 0;
	for(i = 0; i < taille; i++){
		chaine[i] = '\0';
	}
}


void init_buf(char *chaine){
	int i = 0;
	for(i = 0; i <= MAX_SIZE; i++){
		chaine[i] = '\0';
	}
}


char *dotSegmentRemoval(char *chaine) {
	char *chaineDecode = NULL;
	
	if (chaine != NULL) {
		int score = 1;
		
		int taille = strlen(chaine) + 1;
		chaineDecode = malloc(taille * sizeof(char));
		init(chaineDecode, taille);
		char *sousChaine = malloc(taille*sizeof(char));
		init(sousChaine, taille);
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
				init(sousChaine, taille);
				j = 0;
			}
		}

		chaineDecode[k] = '\0';
		free(sousChaine);
	}

	return chaineDecode;
}


char * percentEncoding(char *chaine) {
	char *chaineDecode = NULL;

	if(chaine != NULL) {
		int taille = strlen(chaine) + 1;
		chaineDecode = malloc(taille*sizeof(char));
		init(chaineDecode, taille);
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
	}

	return chaineDecode;
}


char * referenceTarget (void *root) {
	_Token *r1 = NULL;
	noeud * n1 = NULL;
	
	_Token *r2 = NULL;
	noeud * n2 = NULL;
	char * chaineHost;
	r1 = searchTree(root,"request-target");
	n1 = (noeud *) r1->node;
	r2 = searchTree(root, "host");
	if (r2 == NULL) {
		// S'il n'y a pas de champs Host, on fait comme si celui-ci existe mais est nul (ça cherche le fichier à la racine du dossier www)
		// Si on est en HTTP/1.1 on renvoit 400 Bad Request mais c'est géré plus tard
		chaineHost = malloc(2 * sizeof(char));
		sprintf(chaineHost, "");
	} else {
		n2 = (noeud *) r2->node;
		chaineHost = convString(n2 -> valeurChamp);
	}

	char *chaine1 = convString(n1->valeurChamp);

	char *chaine2 = dotSegmentRemoval(chaine1);
	if (strcmp(chaine2, "/") == 0) {
		// Si on veut aller à /, on remplace par /index.html car c'est l'endroit par défaut pour lancer notre site.
		strcat(chaine2, "index.html");
	}

	char *chaineTarget = percentEncoding(chaine2);
	char *chaineBase = DOSSIER_SERVER;
	int tailleTarget = strlen(chaineTarget) + 1;
	int tailleBase = strlen(chaineBase);
	int tailleHost = strlen(chaineHost);
	char *chaineComplete = malloc(tailleTarget + tailleBase + tailleHost);
	init(chaineComplete, (tailleTarget + tailleBase + tailleHost));

	for (int i = 0; i < tailleBase; i++){
		chaineComplete[i] = chaineBase[i];
	}
	for(int i = 0; i < tailleHost; i++){
		chaineComplete[tailleBase + i] = chaineHost[i]; 
	}
	for (int i = 0; i < tailleTarget; i++){
		chaineComplete[tailleBase + tailleHost + i] = chaineTarget[i];
		
	}

	return chaineComplete;
}

void reponseServeur(int code, enTete *et1, char *msgBody, message *requeteRecu, int taille) {
	// Remplit les champs de l'en-tete de la reponse avec les valeurs appropriees
	
	// Code d'erreur
	et1->contentLength = malloc(30 * sizeof(char));
	sprintf(et1->contentLength, "Content-Length: %d\r\n", taille);

	if (code == 200) writeDirectClient(requeteRecu->clientId,M200,strlen(M200));
	if (code == 400) writeDirectClient(requeteRecu->clientId,M400,strlen(M400));
	if (code == 404) writeDirectClient(requeteRecu->clientId,M404,strlen(M404));
	if (code == 501) writeDirectClient(requeteRecu->clientId,M501,strlen(M501));
	
	// Encodage (on ne fait pas d'encodage donc pas très utile pour le moment...)
	if (et1->contentEncoding != NULL) writeDirectClient(requeteRecu->clientId,et1->contentEncoding,strlen(et1->contentEncoding)); // Pour dire en quoi on encode (ex : "Content-Encoding: gzip\r\n") mais on encode en rien donc il n'y a rien à mettre.
	// Langue du contenu (fr pour notre serveur)
	writeDirectClient(requeteRecu->clientId,"Content-Language: fr\r\n",strlen("Content-Language: fr\r\n"));
	// Longueur du body
	if (et1->contentLength != NULL) writeDirectClient(requeteRecu->clientId,et1->contentLength,strlen(et1->contentLength));
	// Type du contenu envoyé (jpeg, png, gif, pdf, html, css, txt, json, etc.)
	if (et1->contentType != NULL) writeDirectClient(requeteRecu->clientId,et1->contentType,strlen(et1->contentType));
	// Date d'envoit de la réponse
	if (et1->date != NULL) writeDirectClient(requeteRecu->clientId,et1->date,strlen(et1->date));
	// Date d'expiration, on considère que ce n'est pas utile pour notre serveur
	if (et1->expires != NULL) writeDirectClient(requeteRecu->clientId,et1->expires,strlen(et1->expires)); // On pourrait le faire mais on n'a pas vraiment l'utilité de mettre une date d'expiration sur nos réponses dans l'état actuel des choses
	// Inutilisé
	if (et1->forwarded != NULL) writeDirectClient(requeteRecu->clientId,et1->forwarded,strlen(et1->forwarded)); // Contient des infos sur le client. A priori à utiliser pour faire des stats ou du debbuging (donc pas dans notre cas)
	// Inutilisé
	if (et1->location != NULL) writeDirectClient(requeteRecu->clientId,et1->location,strlen(et1->location)); // Pour rediriger l'utilisateur vers une page donnée. Peut servir pour des erreurs 404 afin de rediriger vers une page spécifique. (ne nous sert à rien pour les code 200 OK)
	// Nom du serveur (ici numéro de notre groupe)
	writeDirectClient(requeteRecu->clientId,"Server: Groupe14\r\n",strlen("Server: Groupe14\r\n"));
	writeDirectClient(requeteRecu->clientId,"\r\n",strlen("\r\n"));
	// Corps du message
	if (msgBody != NULL) writeDirectClient(requeteRecu->clientId,msgBody,min(taille, MAX_SIZE));
}

void initEnTete(enTete *et1){
	// Initialisation de et1
	et1->contentEncoding = NULL;
	et1->contentLanguage = NULL;
	et1->contentLength = NULL;
	et1->contentType = NULL;
	et1->date = NULL;
	et1->expires = NULL;
	et1->forwarded = NULL;
	et1->location = NULL;
	et1->server = NULL;
}



void ajouterHeure(enTete *et1){
	// Remplit le champ Date de la réponse
	const char * nomJour[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	const char * nomMois[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	char * dateCourante = malloc(38 * sizeof(char));

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
	dateCourante[14] = nomMois[t->tm_mon][0];
	dateCourante[15] = nomMois[t->tm_mon][1];
	dateCourante[16] = nomMois[t->tm_mon][2];
	dateCourante[17] = ' ';
	dateCourante[18] = ((1900 + t->tm_year) / 1000) + 48;
	dateCourante[19] = (((1900 + t->tm_year) / 100) % 10) + 48;
	dateCourante[20] = (((1900 + t->tm_year) / 10) % 10) + 48;
	dateCourante[21] = ((1900 + t->tm_year) % 10) + 48;
	dateCourante[22] = ' ';
	dateCourante[23] = ((t->tm_hour) / 10) + 48;
	dateCourante[24] = ((t->tm_hour) % 10) + 48;
	dateCourante[25] = ':';
	dateCourante[26] = ((t->tm_min) / 10) + 48;
	dateCourante[27] = ((t->tm_min) % 10) + 48;
	dateCourante[28] = ':';
	dateCourante[29] = ((t->tm_sec) / 10) + 48;
	dateCourante[30] = ((t->tm_sec) % 10) + 48;
	dateCourante[31] = ' ';
	dateCourante[32] = 'G';
	dateCourante[33] = 'M';
	dateCourante[34] = 'T';
	dateCourante[35] = '\r';
	dateCourante[36] = '\n';
	dateCourante[37] = '\0';

	et1 -> date = dateCourante;
}


int verification(enTete *et1, message * requeteRecu){
	int envoye = 0;

	if ((!envoye) && (searchTree(getRootTree, "Content-Length-header")) && (searchTree(getRootTree, "Transfert-Encoding-header"))){
		reponseServeur(400, et1, "Cannot associate Transfert-Encoding-header with Content-Length-header\n", requeteRecu, 0);
		envoye = 1;

	}

	_Token * rVersion = searchTree((searchTree(getRootTree,"HTTP-version")->node), "DIGIT");
	noeud * nVersion = (noeud * ) rVersion -> node;
	char chiffre1 = (convString(nVersion -> valeurChamp))[0];
	nVersion = (noeud * ) (rVersion -> next) -> node;
	char chiffre2 = (convString(nVersion  -> valeurChamp))[0];
	printf("Version de HTTP : %c.%c\n", chiffre1, chiffre2);

	if((!envoye) && ((chiffre2 > 49) || (chiffre1 != 49))){
        reponseServeur(400, et1, "HTTP version not recognized\n", requeteRecu, 0);
        envoye = 1;
	}

	if ((!envoye) && (!searchTree(getRootTree, "Host-header")) && (chiffre1 == '1') && (chiffre2 == '1')){
		reponseServeur(400, et1, "Host-header is missing\n", requeteRecu, 0);
		envoye = 1;
	}

	rVersion = searchTree(getRootTree, "Host");
	if (rVersion != NULL) {
		if ((!envoye) && (rVersion -> node) && (rVersion -> next)){
			reponseServeur(400, et1, "There is more than one Host\n", requeteRecu, 0);
			envoye = 1;
		}
	}

	return envoye;
}

int min(int a, int b){
	if(a < b) return a;
	return b;
}


void contentType(char * chaineComplete, int mLen, enTete * et1) {
	// Envoit le champ Content-Type de la reponse

	// On envoit un entête pour les png
	if (chaineComplete[mLen-1] == 'g' && chaineComplete[mLen-2] == 'n' && chaineComplete[mLen-3] == 'p' && chaineComplete[mLen-4] == '.') {
	char * type = malloc(25 * sizeof(char));
						
	strcpy(type, "Content-Type: image/png\r\n\0");						

	et1->contentType = type;
	}

	// On envoit un entête pour les jpg
	if ((chaineComplete[mLen-1] == 'g' && chaineComplete[mLen-2] == 'p' && chaineComplete[mLen-3] == 'j' && chaineComplete[mLen-4] == '.') || (chaineComplete[mLen-1] == 'g' && chaineComplete[mLen-2] == 'e' && chaineComplete[mLen-3] == 'p' && chaineComplete[mLen-4] == 'j' && chaineComplete[mLen-5] == '.')) {
	char * type = malloc(26 * sizeof(char));

	strcpy(type, "Content-Type: image/jpeg\r\n\0");

	et1->contentType = type;
	}

	// Pour les gif
	if (chaineComplete[mLen-1] == 'f' && chaineComplete[mLen-2] == 'i' && chaineComplete[mLen-3] == 'g' && chaineComplete[mLen-4] == '.') {
	char * type = malloc(25 * sizeof(char));
						
	strcpy(type, "Content-Type: image/gif\r\n\0");			

	et1->contentType = type;
	}

	// Pour les pdf
	if (chaineComplete[mLen-1] == 'f' && chaineComplete[mLen-2] == 'd' && chaineComplete[mLen-3] == 'p' && chaineComplete[mLen-4] == '.') {
		char * type = malloc(31 * sizeof(char));
						
		strcpy(type, "Content-Type: application/pdf\r\n\0");						

		et1->contentType = type;
	}

	// Pour le js
	if (chaineComplete[mLen-1] == 's' && chaineComplete[mLen-2] == 'j' && chaineComplete[mLen-3] == '.') {
		char * type = malloc(46 * sizeof(char));

		strcpy(type, "Content-Type: text/javascript; charset=UTF-8\r\n\0");	

		et1->contentType = type;
	}

	// Pour les .txt
	if (chaineComplete[mLen-1] == 't' && chaineComplete[mLen-2] == 'x' && chaineComplete[mLen-3] == 't' && chaineComplete[mLen-4] == '.') {
		char * type = malloc(46 * sizeof(char));

		strcpy(type, "Content-Type: text/plain; charset=UTF-8\r\n\0");
						
		et1->contentType = type;
	}

	// Pour les .html
	if (chaineComplete[mLen-1] == 'l' && chaineComplete[mLen-2] == 'm' && chaineComplete[mLen-3] == 't' && chaineComplete[mLen-4] == 'h' && chaineComplete[mLen-5] == '.') {
		char * type = malloc(45 * sizeof(char));

		strcpy(type, "Content-Type: text/html; charset=UTF-8\r\n\0");
						
		et1->contentType = type;
	}

	// Pour les .css
	if (chaineComplete[mLen-1] == 's' && chaineComplete[mLen-2] == 's' && chaineComplete[mLen-3] == 'c' && chaineComplete[mLen-4] == '.') {
		char * type = malloc(44 * sizeof(char));

		strcpy(type, "Content-Type: text/css; charset=UTF-8\r\n\0");
						
		et1->contentType = type;
	}

	// Pour les .json
	if (chaineComplete[mLen-1] == 'n' && chaineComplete[mLen-2] == 'o' && chaineComplete[mLen-3] == 's' && chaineComplete[mLen-4] == 'j' && chaineComplete[mLen-5] == '.') {
		char * type = malloc(52 * sizeof(char));

		strcpy(type, "Content-Type: application/json; charset=UTF-8\r\n\0");
						
		et1->contentType = type;
	}

	// Pour les tiff
	if (chaineComplete[mLen-1] == 'f' && chaineComplete[mLen-2] == 'f' && chaineComplete[mLen-3] == 'i' && chaineComplete[mLen-4] == 't' && chaineComplete[mLen-5] == '.') {
		char * type = malloc(30 * sizeof(char));
		strcpy(type, "Content-Type: image/tiff\r\n\0");                        
		et1->contentType = type;
	}
}

