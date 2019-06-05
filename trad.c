#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

int puiss(int x, int y){
	if(y <= 0) return 1;
	if(y == 1) return x;
	return x*puiss(x, y-1);
}

char convAscii (char hexa[3]){
	char c = 0;
	int i = 0;
	for (i = 0; i<2; i++){
		if ((hexa[i] >= 65) && (hexa[i] <= 70)) hexa[i] = hexa[i] - 55;
		else hexa[i] = hexa[i] - 48;
		c += hexa[i] * puiss(16,1-i);
	}
	return c;
}


int tradPhp(char* chaineBase, int taille){
	FILE* retour = fopen("./phpFormLog.txt", "a+");
	int mallocDelta = 0;
	for(int k = 0 ; k < taille ; k++){
		if(chaineBase[k] == '=') mallocDelta++;
		if(chaineBase[k] == '%') mallocDelta-=2;
	}
	if(taille + mallocDelta <= 0){
		perror("Chaine non valide");
		return(-1);
	}
	char* buffer = malloc((taille + mallocDelta) * sizeof(char));
	int i = 0;
	int j = 0;
	if(retour == NULL) exit(-1);
	while(i < taille){
		if(chaineBase[i] == '+'){
			buffer[j] = ' ';
		}else if(chaineBase[i] == '&'){
			buffer[j] = '\n';
		}else if(chaineBase[i] == '='){
			buffer[j] = ':';
			buffer[j+1] = ' ';
			j++;
		}else if(chaineBase[i] == '%'){
			char hex[3];
			hex[0] = chaineBase[i+1];
			hex[1] = chaineBase[i+2];
			buffer[j] = convAscii(hex);
			i+=2;
		}else buffer[j] = chaineBase[i];
		i++;
		j++;
	}
	printf("%s\n", buffer);
	fprintf(retour, buffer);
	fprintf(retour, "\n\n");
	fclose(retour);
	return(0);
}


int main(int argc, char* argv[]){
	if(argc != 2){
		perror("Une seule chaine de caracteres en argument.");
		return(-1);
	}
	tradPhp(argv[1], strlen(argv[1]));
	return 0;
}