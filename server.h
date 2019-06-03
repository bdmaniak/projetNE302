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


void init(char *chaine);
char *dotSegmentRemoval(char *chaine);
char * percentEncoding(char *chaine);
char * referenceTarget (void *root);
void reponseServeur(int code, enTete *et1, char * msgBody, message *requeteRecu);
void initEnTete(enTete *et1);
void ajouterHeure(enTete *et1);
int verification();
