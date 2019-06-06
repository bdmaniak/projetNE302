// CONSTANTES
var TAILLE_MAP = 20; // Taille de la carte en blocs
var TAILLE_BLOC = 25; // Taille d'un bloc en px

var snake = new Snake((TAILLE_MAP/2)*TAILLE_BLOC,(TAILLE_MAP/2)*TAILLE_BLOC,TAILLE_BLOC,TAILLE_MAP);
var pomme = new Aliment(TAILLE_BLOC,TAILLE_MAP);

window.onload = function() {
	var canvas = document.getElementById('canvas');
	var ctx = canvas.getContext('2d');
	
	// Taille du jeu
	canvas.width = TAILLE_BLOC * TAILLE_MAP;
	canvas.height = TAILLE_BLOC * TAILLE_MAP;
	
	// Rafraichissement de l'image
	setInterval(function() {
		snake.modifier();
		snake.dessiner(ctx);
	}, 50);
	
	// Gestion du clavier
	window.onkeydown = function(event) {
		var e = event || window.event;
		var key = e.which || e.keyCode;
		switch(key) {
			case 38 : case 122 : case 119 : case 90 : case 87 : // Flèche haut, z, w, Z, W
				snake.direction(DIRECTION.HAUT);
				break;
			case 40 : case 115 : case 83 : // Flèche bas, s, S
				snake.direction(DIRECTION.BAS);
				break;
			case 37 : case 113 : case 97 : case 81 : case 65 : // Flèche gauche, q, a, Q, A
				snake.direction(DIRECTION.GAUCHE);
				break;
			case 39 : case 100 : case 68 : // Flèche droite, d, D
				snake.direction(DIRECTION.DROITE);
				break;
			default : 
				return true;
		}
	}
}
