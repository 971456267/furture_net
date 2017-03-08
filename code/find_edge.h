void find_edge(int *d,int z,int first_1[] ,int next_1[],int v_1[],int *result){
	  for (int j = 0; j < z; j++){
		  for (int i = first_1[d[j]]; ; i = next_1[i]){
			  if ( d[j + 1] == v_1[i]) {
				  *(result + j) = i;
				  break;
			  }
			  else continue;
		  }

	  }

}
