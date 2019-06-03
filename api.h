#ifndef API_H
#define API_H


typedef struct String {
	FILE * fichier;
	int depart;
	int taille;
} string;



typedef struct Arbre {
  struct Arbre *fils; //Tableau de pointeurs vers les fils
  struct Arbre *frere;
  string *nomChamp;
  string *valeurChamp;
} noeud;





// defini un type pour la liste chainee renvoyée en réponse de  la requete de recherche dans l'arbre.
typedef struct _token {
  void *node;
  // node type opaque pointant vers un noeud de votre arbre.
  struct _token *next;
  // next pointe vers le prochain token.
} _Token;


/* IMPORTANT: Nous avons remplacer void *getRootTree() par void *getRootTree car sans argument on ne peut pas faire grand chose
et la deuxième formulation est plus adapté pour pointé un element de l'arbre*/

// Pointeur (type opaque) vers la racine de l'arbre construit.
void *getRootTree;

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
void purgeTree(void *root);//normalement void*

// L'appel à votre parser un char* et une longueur à parser.
int parseur(char *req, int len);




#endif
