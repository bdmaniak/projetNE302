var DIRECTION = {
	"HAUT" : 0,
	"DROITE" : 1,
	"BAS" : 2,
	"GAUCHE" : 3
}

function Snake(x,y,largeur,map) {
	this.x = x + largeur;
	this.y = y;
	this.map = map;
	this.longueur = 3;
	this.largeur = largeur;
	this.dx = largeur;
	this.dy = 0;
	this.body = [[x-largeur,y],[x,y],[x+largeur,y]];
	
	this.add = function(elmt) {
		this.body.push(elmt);
		return true;
	}
	
	this.remove = function() {
		this.body.reverse();
		this.body.pop();
		this.body.reverse();
		return true;
	}
	
	this.direction = function(d) {
		switch(d) {
			case DIRECTION.HAUT:
				this.dx = 0;
				this.dy = -this.largeur;
				break;
			case DIRECTION.DROITE:
				this.dx = this.largeur;
				this.dy = 0;
				break;
			case DIRECTION.BAS:
				this.dx = 0;
				this.dy = this.largeur;
				break;
			case DIRECTION.GAUCHE:
				this.dx = -this.largeur;
				this.dy = 0;
				break;
		}
	}
	
	this.modifier = function() {
		//Il faudrait le mettre dans la classe map quand elle sera créée
		this.x += this.dx;
		this.y += this.dy;
		n = this.map*this.largeur; // Il s'agit de la taille de la map
		if (this.x >= n) {
			this.x %= n;
		}
		if (this.x < 0) {
			this.x = n - this.largeur;
		}
		if (this.y >= n) {
			this.y %= n;
		}
		if (this.y < 0) {
			this.y = n - this.largeur;
		}
		
		if (this.collision()) {
			this.reset();
			return true
		}
		this.add([this.x,this.y]);
		if (!this.manger()) {
			this.remove();
		}
	}
	
	this.dessiner = function(ctx) {
		ctx.fillStyle = 'black';
		ctx.fillRect(0,0,this.largeur * this.map, this.largeur * this.map);
		for (i=0 ; i<this.body.length ; i++) {
			ctx.fillStyle = 'white';
			ctx.fillRect(this.body[i][0],this.body[i][1],this.largeur,this.largeur);
		}
		ctx.fillStyle = 'red';
		ctx.fillRect(pomme.x,pomme.y,this.largeur,this.largeur);
	}
	
	this.manger = function() {
		if (this.x == pomme.x && this.y == pomme.y) {
			pomme.x = -1;
			pomme.y = -1;
			pomme.creer();
			return true;
		}
		return false;
	}
	
	this.collision = function() {
		if (inclusionArray([this.x,this.y],this.body)) {
			return true;
		}
		return false;
	}
	
	this.reset = function() {
		this.x = (this.map/2)*this.largeur + this.largeur;
		this.y = (this.map/2)*this.largeur;
		this.dx = this.largeur;
		this.dy = 0;
		this.body = [[this.x-2*this.largeur,this.y],[this.x-this.largeur,this.y],[this.x,this.y]];
	}
}
