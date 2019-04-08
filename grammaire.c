#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "api.h"
#include "grammaire.h"
#include "lecture.h"


noeud *creerNoeud(string *s1, string *s2){
	noeud *n1 = malloc(sizeof(noeud));
	n1 -> nomChamp = s1;
	n1 -> valeurChamp = s2;
	return n1;


}

void ajouteFils(noeud *n1, noeud *n2){

	if (n1 -> fils == NULL){
		
		n1 -> fils = n2;
		
	}
	else{
		noeud *repere = n1 -> fils;
		while (repere -> frere != NULL){
			repere = repere -> frere;			
		}
		repere -> frere = n2;

	}
	


}



void ajouteFrere(noeud *n1, noeud *n2){

	if (n1 == NULL){
		n1 = n2;
		
	}

	else if (n1 -> frere == NULL){
		n1 -> frere = n2;
		
	}
	else{

		
		noeud *repere = n1 -> frere;
		while (repere -> frere != NULL){
			
			repere = repere -> frere;			
		}
		repere -> frere = n2;

	}
	


}


int tailleNoeud (noeud *n1){
	int taille = 0;
	if (n1 == NULL) taille = 0;

	else{
		noeud *repere = n1;
		taille = (repere -> valeurChamp)->taille;
		while (repere -> frere != NULL){
			repere = repere -> frere;
			taille += (repere -> valeurChamp)->taille;			
		}

	}
	return taille;

}


string *creerString(FILE *fic, int start, int size){
	string *st = malloc(sizeof(string));
	st -> fichier = fic;
	st->depart = start;
	st-> taille = size; 

	return st;

}



noeud *etoile(noeud *(*fonction)(FILE*, FILE*, int*, int, int, int),FILE* grammaire, FILE* lu, int *indice, int ligne, int i, int j , int borne1, int borne2){


	if (borne2 == 0) borne2 = 10000000;
	int compteur = 0;
	noeud *nfils, *nfrere;
	nfils = fonction(grammaire, lu, indice, ligne, i, j);
	if (nfils != NULL) compteur++;
	
	
	while ((nfils != NULL) && (compteur < borne2) && ((nfrere = fonction(grammaire, lu, indice, ligne, i, j)) != NULL)){
		
		ajouteFrere(nfils, nfrere);
		compteur++;
	}

	if (((compteur < borne1) || (compteur > borne2)) ){

		nfils = NULL;
		//Fonction qui supprime tout

	}
	if((borne1 == 0) && (nfils == NULL)){
		string *s1 = recupereMot(ligne, i, grammaire);
		string *s2 = creerString(lu, *indice, 0);
		nfils = creerNoeud(s1, s2);

	}

	
	return nfils;


}



noeud *SP (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("SP", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (lire(lu, *indice) == ' '){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}


noeud *HTAB (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("HTAB", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (lire(lu, *indice) == '	'){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}

noeud *DIGIT (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("DIGIT", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (isdigit(lire(lu, *indice))){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}


noeud *ALPHA (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("ALPHA", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1); 
	if (isalpha(lire(lu, *indice))){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}



noeud *caseInsensitive (FILE *grammaire, FILE *lu, int *indice, string *element){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("case_insensitive_string", grammaire), 0, grammaire);
	int taille = element -> taille - 2;
	string *s2 = creerString(lu, *indice, taille); 
	int correct = 1;
	int i = 0;
	while ((i < taille) && (correct) && (lire(lu, (*indice) + i) != EOF) ){
		if (lire(lu, (*indice) + i) != recupChar(element, i + 1)) correct = 0;
		else i++;

	}
	if (i == taille){
		
		n1 = creerNoeud(s1, s2);
		(*indice) += taille;

	}



	return n1;

}


noeud *CRLF (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("CRLF", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 2); 
	if ((lire(lu, *indice) == '\r')&&(lire(lu, (*indice)+1) == '\n')){
		
		n1 = creerNoeud(s1, s2);
		(*indice) += 2;

	}
	return n1;

}

noeud *HEXDIG (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	char c = lire(lu, *indice);
	string *s1;
	s1 = recupereMot(rechercheMot("HEXDIG", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1);
	 
	if (((c >= 48) && (c <= 57))||((c >= 65)&&(c <= 70))){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}

noeud *DQUOTE (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	char c = lire(lu, *indice);
	string *s1;
	s1 = recupereMot(rechercheMot("DQUOTE", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1);
	 
	if (c == 34){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}


noeud *VCHAR (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	char c = lire(lu, *indice);
	string *s1;
	s1 = recupereMot(rechercheMot("VCHAR", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1);
	 
	if ((c >= 33) && (c <= 126)){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}


noeud *CHAR (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	char c = lire(lu, *indice);
	string *s1;
	s1 = recupereMot(rechercheMot("CHAR", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1);
	 
	if ((c >= 0) && (c <= 127)){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}

noeud *OCTET (FILE *grammaire, FILE *lu, int *indice){
	noeud *n1;
	n1 = NULL;
	char c = lire(lu, *indice);
	string *s1;
	s1 = recupereMot(rechercheMot("OCTET", grammaire), 0, grammaire);
	string *s2 = creerString(lu, *indice, 1);
	 
	if ((c >= 0) && (c <= 255)){
		
		n1 = creerNoeud(s1, s2);
		(*indice)++;

	}
	return n1;

}



char convAscii (char hexa[3]){

	char c = 0;
	int i = 0;	
	for (i = 0; i<2; i++){
		if ((hexa[i] >= 65) && (hexa[i] <= 70)) hexa[i] = hexa[i] - 55;
		else hexa[i] = hexa[i] - 48;
		c += hexa[i] * pow(16,1-i);
	}

	return c;


}



noeud *valAscii (FILE *grammaire, FILE *lu, int *indice, string *element){
	
	noeud *n1;
	n1 = NULL;
	string *s1;
	s1 = recupereMot(rechercheMot("valAscii", grammaire), 0, grammaire);
	string *s2 = NULL;
	int taille = element -> taille;
	int tailleAscii = 0;
	int correct = 1;
	char val[3];
	char val2[3];
	char valeur;
	char valeur2 = 0;
	char caractereLu = lire(lu, (*indice)); 
	val[0] = recupChar(element, 2);
	val[1] = recupChar(element, 3);
	val[2] = '\0';
	valeur = convAscii(val);
	 
	
	//Si %A
	if (taille == 4){

		tailleAscii = 1;
		if ( caractereLu != valeur) correct = 0;
		

	}
	//si %xA-B
	else if ((taille == 7) && (recupChar(element, 4) == '-')){
		tailleAscii = 1;
		val2[0] = recupChar(element, 5);
		val2[1] = recupChar(element, 6);
		val2[2] = '\0';
		valeur2 = convAscii(val2);
		if((caractereLu < valeur) || (caractereLu > valeur2)) correct = 0;
	}
	//si %xA.B.C.D.E ...
	else if (taille >= 7){
		tailleAscii = (taille - 1)/3;
		for (int i = 0; (i < tailleAscii) && (correct) ; i++){
			val[0] = recupChar(element, 2 + 3*i);
			val[1] = recupChar(element, 3 + 3*i);
			valeur = convAscii(val);
			if (valeur != lire(lu, (*indice + i))) correct = 0;


		}


	}
	else{
		
		correct = 0;

	}

	if (correct) {
		s2 = creerString(lu, *indice, tailleAscii); 
		n1 = creerNoeud(s1, s2);
		(*indice) += tailleAscii;


	}

	return n1;

}







noeud *creerArbre(FILE *grammaire, FILE *lu, int *indice, int ligne, int i, int j){

	noeud *n1 = NULL;
	noeud *n2 = NULL;
	int line = 0;
	int dernierElement = 0;
	int indiceBis = *indice;
	int compteur = 0;
	int ainee = 0;
	int borne1 = 0;
	int borne2 = 0;
	char c;
	int debut = 0;
	int fin = 0;
	int k = 0;
	int continuer = 1;
	int l = 0;


		

	//Cas de base
	if (i == j){


		
		string *mot = recupereMot(ligne, i, grammaire);




		if (compareChaineStr(mot, "ALPHA")){
			
			n1 = ALPHA (grammaire, lu, indice);

		}

		else if (compareChaineStr(mot, "DIGIT")){
			n1 = DIGIT(grammaire, lu, indice);
	
		}

		else if (compareChaineStr(mot, "HTAB")){
			n1 = HTAB(grammaire, lu, indice);	
		}

		else if (compareChaineStr(mot, "SP")){
			n1 = SP(grammaire, lu, indice);	
		}

		else if (compareChaineStr(mot, "CRLF")){
			n1 = CRLF(grammaire, lu, indice);	
		}

		else if (compareChaineStr(mot, "HEXDIG")){
			n1 = HEXDIG(grammaire, lu, indice);	
		}

		else if (compareChaineStr(mot, "DQUOTE")){
			n1 = DQUOTE(grammaire, lu, indice);	
		}

		else if (compareChaineStr(mot, "VCHAR")){
			n1 = VCHAR(grammaire, lu, indice);	
		}

		else if (compareChaineStr(mot, "CHAR")){
			n1 = CHAR(grammaire, lu, indice);	
		}

		else if (compareChaineStr(mot, "OCTET")){
			n1 = OCTET(grammaire, lu, indice);	
		}

		else if (recupChar(mot,0) == '"'){
			n1 = caseInsensitive (grammaire, lu, indice, mot);	
		}

		else if (recupChar(mot,0) == '%'){
			n1 = valAscii(grammaire, lu, indice, mot);
		}


		else{
			line = rechercheString(mot, grammaire);
			dernierElement = IndiceDernierElement(line, grammaire);

			if ((n2 = creerArbre(grammaire, lu, indice, line, 2, dernierElement)) != NULL){
				n1 = malloc(sizeof(noeud));
				n1 -> nomChamp = recupereMot(line, 0, grammaire);


				ajouteFils(n1, n2);
				
				n1 -> valeurChamp = creerString(lu, indiceBis, tailleNoeud(n2));

			}


			
	



		}




	}
	else{

		
		
		
		string *mot = NULL;
		l = i;

		while ((l <= j) && continuer){
			mot = recupereMot(ligne, l, grammaire);
			if (compareChaineStr(mot, "(")) compteur ++;
			if (compareChaineStr(mot, ")")) compteur --;
			if ((compareChaineStr(mot, "/")) && (compteur == 0)){

				continuer = 0;
				
				if ((n1 = creerArbre(grammaire, lu, indice, ligne, i, l-1)) == NULL){
					
					
					n1 = creerArbre(grammaire, lu, indice, ligne, l + 1, j);
					
				}


				
				
			}
			
			l++;

		}



		k = i;
		

		while ((k <= j) && continuer){

			


			mot = recupereMot(ligne, k, grammaire);
			
			borne1 = 0;
			borne2 = 0;
			

			
			if ((recupChar(mot, 0) == '*')||(recupChar(mot, 1) == '*') || (isdigit(c = recupChar(mot, 0)))){

				if (recupChar(mot, 0) == '*'){
					if (isdigit(c = recupChar(mot, 1))) borne2 = c - 48;
				}

				else if ((recupChar(mot, 1)) == '*'){
					borne1 = recupChar(mot, 0) - 48;
					if (isdigit(c = recupChar(mot, 2))) borne2 = c-48;

				}
				else{
					c = recupChar(mot, 0);
					borne1 = c - 48;
					borne2 = c - 48;


				}


				if (compareChaineStr(recupereMot(ligne, k+1, grammaire),"(")){
					compteur = 1;
					k = k + 2;
					debut = k;
					
				
				
					while (compteur != 0){
					
						mot = recupereMot(ligne, k, grammaire);
					
					
						if (compareChaineStr(mot,"(")) compteur ++;
						if (compareChaineStr(mot,")")) compteur --;

						k++;

					}					
					fin = k-2;
					
					k--;



				}
				else{
					debut = k+1;
					fin = k+1;
					k = k +1;

				}

				n2 = etoile(creerArbre,grammaire,lu, indice, ligne, debut , fin, borne1, borne2);
				



				
			

			}

			else{


				if (compareChaineStr(recupereMot(ligne, k, grammaire),"(")){

					compteur = 1;

					k = k + 1;
					debut = k;

				
					while (compteur != 0){
					
						mot = recupereMot(ligne, k, grammaire);

					
					
						if (compareChaineStr(mot,"(")) compteur ++;
						if (compareChaineStr(mot,")")) compteur --;

						k++;

					}					
					fin = k-2;
					k--;

					n2 = creerArbre(grammaire, lu, indice, ligne, debut, fin);
					



				}
				else if (compareChaineStr(recupereMot(ligne, k, grammaire),"[")){


					compteur = 1;

					k = k + 1;
					debut = k;

				
				
					while (compteur != 0){
					
						mot = recupereMot(ligne, k, grammaire);

					
					
						if (compareChaineStr(mot,"[")) compteur ++;
						if (compareChaineStr(mot,"]")) compteur --;

						k++;

					}					
					fin = k-2;
					k--;
					n2 = etoile(creerArbre, grammaire, lu, indice, ligne, debut, fin, 0, 1);
					



				}
				else{
					n2 = creerArbre(grammaire, lu, indice, ligne, k, k);

				}
				

				
				
					
			}
			




			if (n2 == NULL){
				continuer = 0;
				n1 = NULL;

				
				k = j;
			}
			else if (!ainee){
				n1 = n2;

				ainee = 1;
			}
			else{
				ajouteFrere(n1,n2);

			}



			k++;

			


			

		}





	}


	if (n1 == NULL) *indice = indiceBis;

	
	return n1;

}

