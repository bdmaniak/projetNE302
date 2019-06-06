#ifndef __SERVER_H
#define __SERVER_H

typedef struct EnTete{
	char *contentEncoding;
	char *contentLanguage;
	char *contentLength;
	char *contentType;
	char *date;
	char *expires;
	char *forwarded;
	char *location;
	char *server;

} enTete;


void init(char *chaine, int taille);
void init_buf(char *chaine);
char *dotSegmentRemoval(char *chaine);
char * percentEncoding(char *chaine);
char * referenceTarget (void *root);
void reponseServeur(int code, enTete *et1, char * msgBody, message *requeteRecu, int taille);
void initEnTete(enTete *et1);
void ajouterHeure(enTete *et1);
int verification();
int min(int a, int b);
void contentType(char * chaineComplete, int mLen, enTete * et1);
void envoyerPHP (enTete *et1, message* requeteRecu, int method);

#endif
