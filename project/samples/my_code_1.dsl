/* This computes matrix multiplication */

main () {
	
	Int maxIndex;
	maxIndex = 5;

	Matrix base = readMatrix ( "../samples/my_data_AA.dat" ) ;
	Matrix data = readMatrix ( "../samples/my_data_AB.dat" ) ;
    Matrix buffer[ maxIndex, maxIndex ] i, j = 
	let
		Int k;
		k = 0;
	in
		k
	end;
	
	
	Int i;
	Int j;
	Int k;
	
	for(i=0 : maxIndex-1){
		for(j=0 : maxIndex-1){
			for(k=0 : maxIndex-1){
				buffer[i,j] = buffer[i,j] + base[i,k] * data[k,j];
			}
		}
	}
	
	print(buffer);
}


