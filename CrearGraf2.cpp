//es la unica diferencia
    //ara per opcio amb transport de crews
   bool transbord[size][size];
      for (int i=0; i<size; i++){
 	for(int j = 0; j<size; j++){
 		transbord[i][j]=false;
	}
      }

	bool transbords = true;
	while (transbords){
	transbords=false;//per poder sortir abans si eso del bucle
	for(int i = 0; i<size; i++){
            for (int j=0; j<size; j++){
               	 if (transbord[i][j]==true){
                    for(int k=0; k<size; k++){
                        if ((transbord[j][k])&&(!transbord[i][k])){
                            transbords=true;
                            edge nova;
                            nova.final=k+1;
                            nova.min=0;
                            nova.max=1;
                            nova.usat=0;
                            Graf[i+size+4].push_back(nova);
                            transbord[i][k]=true;
                        }
                    }
                }
            }
        }        
	}//pitjor cas de n^4 :(
}
