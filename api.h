// defini un type pour la liste chainee renvoyée en réponse de  la requete de recherche dans l'arbre.
typedef struct _token {
  void *node;
  // node type opaque pointant vers un noeud de votre arbre.
  struct _token *next;
  // next pointe vers le prochain token.
} _Token;

// Fonction qui retourne un pointeur (type opaque) vers la racine de l'arbre construit.
void *getRootTree();

// Fonction qui recherche dans l'arbre tous les noeuds dont l'etiquette est egale à la chaine de caractères en argument.
// Par convention si start == NULL alors on commence à la racine
// sinon on effectue une recherche dans le sous-arbre à partir du noeud start
_Token *searchTree(void *start,char *name);

// fonction qui renvoie un pointeur vers char indiquant l'etiquette du noeud. (le nom de la rulename, intermediaire ou terminal)
// et indique (si len!=NULL) dans *len la longueur de cette chaine.
char *getElementTag(void *node,int *len);

// fonction qui renvoie un pointeur vers char indiquant la valeur du noeud. (la partie correspondnant à la rulename dans la requete HTTP )
// et indique (si len!=NULL) dans *len la longueur de cette chaine.
char *getElementValue(void *node,int *len);

// Fonction qui supprime et libere la liste chainée de reponse.
void purgeElement(_Token **r);

// Fonction qui supprime et libere toute la mémoire associée à l'arbre .
void purgeTree(void *root);

// L'appel à votre parser un char* et une longueur à parser.
int parseur(char *req, int len);

//Permet de faire pointer le curseur de fichier à la n-ième ligne
int pointeurLigne(FILE *fic, int n);

//Permet de recuperer le n-ieme mot sur la ligne de notre choix
string *recupereMot(int ligne, int n, FILE *fic);

// Recupere le n-ieme caractere d'un element string
char recupChar(string *chaine, int n);

//compare un string avec une chaine de caractere
int compareChaineStr(string *s1, char *chaine);

//Compare deux string
int compareString(string *s1, string *s2);

// Affiche un string
void afficherString(string *chaine);

int verifieFormat(string recu, string grammaire);

noeud *creerArbreV2 (/*FILE *recu, */FILE *grammaire, noeud *n1);
