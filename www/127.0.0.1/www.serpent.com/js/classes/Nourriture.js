function Aliment(tBloc,tMap) {
	this.tailleBloc = tBloc;
	this.tailleMap = tMap;
	this.x = -1;
	this.y = -1;
	
	this.creer = function() {
		if (this.x == -1 || this.y == -1) {
			this.x = Math.floor(Math.random() * (this.tailleMap))*this.tailleBloc;
			this.y = Math.floor(Math.random() * (this.tailleMap))*this.tailleBloc;
			while (this.x == snake.x && this.y == snake.y) {
				this.x = Math.floor(Math.random() * (this.tailleMap))*this.tailleBloc;
				this.y = Math.floor(Math.random() * (this.tailleMap))*this.tailleBloc;
			}
		}
	}
	this.creer();
}