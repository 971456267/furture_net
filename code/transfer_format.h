#include <iostream>
#include <cstring>
#include <stdlib.h>
using namespace std;
 int  transfer_format_topo(char *topo[5000],int edge_num,int *topo_int){
//读topo
	int maxnode = 0;
	for( int i = 0; i < edge_num; i++){
        int j = 0;

        string t1,t2,t3,t4;
		//cout<<topo[i];
		while(*(topo[i]+j) != ','){
		t1 += topo[i]+j;
		j++;
		}
		j++;
		while(*(topo[i]+j)!= ','){
		t2 += topo[i]+j;
		j++;
		}
		j++;
		while(*(topo[i]+j) != ','){
		t3 += topo[i]+j;
		j++;
		}
		j++;
		t4 = topo[i]+j;
		int a = atoi(&t2[0]);
		int b = atoi(&t3[0]);
		int c = atoi(&t4[0]);


		*(topo_int + i*3 + 0) = a;
		*(topo_int + i*3 + 1) = b;
		*(topo_int + i*3 + 2) = c;
		int max = a>b?  a:b;
		maxnode = maxnode > max? maxnode:max;

	}
	return maxnode;

 }

int transfer_format_demand( char *demand ,int *d){
	//读取需要经过的源点和终点
	        string d1,d2;
	        int i = 0;
	       // cout<<demand<<endl;
	        while(demand[i] != ','){
	        	d1 += demand[i];
	        	i++;
	        }
	        i++;
	        while(demand[i] != ','){
	              d2 += demand[i];
	              i++;
	        }
	        i++;
		    int s = atoi(&d1[0]); d[0] = s;
		    int t = atoi(&d2[0]);d[1] = t;

		   // cout<<"s = "<<s<<endl;
		    //cout<<"t = "<<t<<endl;
	//读取必须经过的节点V'
		   string v = demand + i;
		   //cout << v <<endl;
		  //节点的名字以012345命名,打印出必须要经过的V',
		  //int d[50]={0};
		  int len = v.size();
		  string temp;
		  int x = 2; //标记子集下标
		  for (int k = 0; k < len; k++){
			  if(v[k] != '|'){
				  temp += v[k];
			  }

			  else{
			  d[x] = atoi(&temp[0]);
			  temp = "\0";
			  x++;
			  }
		  }
          d[x] = atoi(&temp[0]);
		return x + 1;
}
