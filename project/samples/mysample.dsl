main () {
	Int Sum;
	Sum = 0 ;
	
	Int Num;
	Num = 1 ;
	
	Bool Flag;
	Flag = True ;

	while ( Flag ) {
		Sum = Sum + Num ;
		Num = Num + 1 ;
		if ( Num > 10 ) {
			Flag = False ;
		}
	}
}
