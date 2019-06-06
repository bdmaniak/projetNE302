function inclusionArray(a,b) {
	// A refaire sans le sort car on ne peut pas différencier [x,y] et [y,x]
	for (var i = 0 ; i < b.length ; i++) {
		if (a.length != b[i].length) {
			continue;
		}
		
		a1 = a;
		b1 = b[i];
		test = 1;
		
		for (var j = 0 ; j < a1.length ; j++) {
			if (a1[j] != b1[j]) {
				test = 0;
			}
		}
		
		if (test) {
			return true;
		}
	}
	return false;
}