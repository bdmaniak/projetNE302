/* 
#include "rfc3986.h" 
#include "rfc7230.h" 
#include "rfc4647.h" 
#include "rfc5646.h" 
#include "rfc7231.h" 
#include "rfc7232.h" 
#include "rfc7233.h" 
#include "rfc7234.h" 
#include "rfc7235.h" 
*/
void *httpgetRootTree(); 

typedef struct node {
		int level; 
		char *name;
		char *value ;
		int len;
		int explored;
		struct node *link;
		} Lnode ;

