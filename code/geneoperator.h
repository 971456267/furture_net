/*
 * operator.h
 *
 *  Created on: 2016年3月30日
 *      Author: shijia
 */

#ifndef GENEOPERATOR_H_
#define GENEOPERATOR_H_
#include<cstdlib>
#include "delete_graph.h"
#include "Dijkstra.h"

int  MAX = 100000;
//#include "initpath.h"
int Rtweight(int *a,int *b,int *d,int c) //路径的权重生成函数，a是路径，b是费用矩阵，d是邻接矩阵，c是路径上的节点个数
{
	int i,s=0,j;
	for (i = 0; i < c-1; i++)
	{
		for (j = 0; j < 8; j++)
			if (*(d + *(a + i) * 8 + j) == *(a + i + 1))
			{
				break;
			}
		s = s + *(b + *(a + i) * 8 + j);
	}
	return s;
}

int designedjudge(int *route, int *d, int nodenum,int demandnum)    //比较路径是中是否包含所有必经点，是返回1，否则返回0
{
	int flag=0,i;
	for ( i = 0; i < demandnum; i++)
	{
		flag = 0;
		for (int j =0; j < nodenum; j++)
		{
			if (*(route + j) == *(d + i + 2))
			{
				flag = 1;
				break;
			}
		}
		if (flag != 1)
		{
			i--;
			break;
		}
	}
	if (i == demandnum)
		return 1;
	else
		return 0;
}

void select(double *Weight, int *selectedgene,int crossnum,int groupnum){
	//cout << "进入select";
	double*fit = (double *)malloc(sizeof(double)*crossnum);  //fit 适应度
	double sum = 0;
	double *ratio=(double*)malloc(sizeof(double)*crossnum);//选择概率
	double *accratio = (double*)malloc(sizeof(double)*crossnum); //积累概率
	memset(ratio, 0, crossnum);
	memset(accratio, 0, crossnum);


	//计算适应度
	//cout << "计算适应度";
	for (int i = 0; i < crossnum; i++){
		*(fit + i) = 1 / Weight[i];
		cout << *(fit + i) << " ";
		sum += *(fit + i);
	}
	//cout <<endl<<sum <<endl;

	//计算选择概率
	cout << endl << "计算选择概率";
	for (int i = 0; i <crossnum; i++){
		ratio[i] = (*(fit + i)) / sum;
		cout << ratio[i] << " ";
	}
	cout << endl;
	////计算梯度概率
	cout << "计算梯度概率";
	for (int i = 0; i < crossnum; i++){
		for (int j = 0; j <= i; j++){
			accratio[i] += ratio[j];
		}
		cout << accratio[i] << " ";
	}
	cout << endl << "随机染色体存活机率";
	//随机产生染色体存活机率,因为只保留groupnum条，所以只产生groupnum个概率
	double *pick = (double*)malloc(sizeof(double)*groupnum);
	memset(pick, 0, groupnum);
	for (int i = 0; i < groupnum; i++)
	{
		pick[i] = (((rand()) % 10001) / (10000 + 0.000001));
		//测试用于输出随机产生的概率
		cout << pick[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < groupnum; i++){
		for (int j = 0; j < crossnum; j++){
			if (pick[i] <= accratio[j]){
				*(selectedgene + i) = j;
				cout<<*(selectedgene + i)<<" ";
				break;
			}
			else continue;
		}

	}
	//cout << "应选择的父代的编号";
	for (int i = 0; i < groupnum; i++){
		//cout << *(selectedgene + i) << " ";
	}
}

void find_edge(int *d, int z, int first_1[], int next_1[], int v_1[], int *result)  //根据节点寻找边
{
	for (int j = 0; j < z; j++)
	{
		for (int i = first_1[d[j]];; i = next_1[i])
		{
			if (d[j + 1] == v_1[i])
			{
				*(result + j) = i;
				break;
			}
			else continue;
		}

	}
}

int crossover(int *parent1, int *parent2, int p1nodenum, int p2nodenum, int *offspring1, int *offspring2)   //parent1、2为父代染色体，p1nodenum为父代1的节点个数，offspring1为生成的子代1，返回0表示交叉失败，有路径长度小于3，否则此次交叉成功
{
   // if(p1nodenum == p2nodenum){
    //	if(!routecmpA(parent1,parent2,p1nodenum)) return 0;
    //}
	int crossnode, nodenum;
	if (p1nodenum > p2nodenum)
		nodenum = p2nodenum;
	else
		nodenum = p1nodenum;
	if (nodenum <= 3)        //如果有一条父代的路径小于等于3个节点，则返回交叉不成功
		return 0;
	else
	{

		crossnode = rand() % (nodenum - 3) + 2 ;   //随机输出交叉节点
		for (int i = 0; i < crossnode; i++)
			*(offspring1 + i) = *(parent1 + i);
		for (int i = crossnode; i < p2nodenum; i++)
			*(offspring1 + i) = *(parent2 + i);
		for (int i = 0; i < crossnode; i++)
			*(offspring2 + i) = *(parent2 + i);
		for (int i = crossnode; i < p1nodenum; i++)
			*(offspring2 + i) = *(parent1 + i);
		return 1;
	}
}

int roadjudge(int *road, int roadnum, int *linkmatrix,int *nodematrix) //road为路径，roadnum为路径上的节点个数，linkmatrix为邻接矩阵，nodematrix为节点个数矩阵，路径合法性判断函数，合法返回1，不合法返回0
{
	int flag = 0;
	for (int i = 1; i < roadnum-2; i++)
		for (int j = i + 1; j < roadnum-1;j++)
			if (*(road+i)==*(road+j))
				return 0;
	for (int i = 0; i < roadnum - 1; i++)
	{
		flag = 0;
		for (int j = 0; j < *(nodematrix + *(road + i)); j++)
			if (*(linkmatrix + (*(road + i)) * 8 + j) == *(road + i + 1))
			{
				flag = 1;
				break;
			}
		if (flag == 0)
			return 0;
	}
	return 1;
}


//计算每条路上
void countNum(int **generation,int cnt,int *Gcost,int *Gnodenum,int *d, int demandnum,int *count,int *recordmin)
{
	//count保存每一代经过的必经节点个数
	//recordmin,下标代表必经节点个数，数值代表经过的与下标数目相等的必经节点时所有代中路径最短的花费，初始化化为无穷。
	for(int k = 0; k < cnt; k++ )
	{
		int time = 0;
		for(int i = 1;i < Gnodenum[k] -1; i++)
		{
			for(int j = 0; j < demandnum; j++)
				{
					if((generation[k][i]) == d[2 + j])
					time++;
				}
			}
		count[k] = time;
		if(recordmin[time] > Gcost[k])
		{
			recordmin[time] = Gcost[k];
		}
	}
}

//根据包含的必经节点的个数自适应调整因子，w1*a1(经过n个必经节点的最短路径xan)<w2*a2(经过n-1个必经节点的最短路径xan-1)
void autofit(int *recordmin,double *a)
{
   int i = 50;
   while(recordmin[i] == MAX) i--;
   for(int k = i ; k > 0; )
   {
	   int j = k - 1;
	   while(recordmin[j] == MAX) j--;
	   while((recordmin[j] * a[j] )<= (recordmin[k] * a[k]))
	   {
	   		   a[j] += 0.7;
	   }
	   k = j;
   }
}

int mutation(int *road,int maxnode,int*Gcost,int roadnum,int *d,int demandnum,int adjMatrix[][8],int *adjNumber,int *wMatrix)
{
	int d1[50];// new int[demandnum];//存放road已经过的必经点
	int d2[50];// = new int[demandnum];//存放road没有经过的必经点
	int cnt1 = 0;//经过的个数
	int cnt2 = 0;//未经过的个数
	for(int i = 0;i < demandnum; i++)
	{
		int flag = 0;
		for(int j = 0; j < roadnum; j++)
		{
			if(d[2 + i] == road[j])
			{
				d1[cnt1] = d[2 + i];
				flag = 1;
				cnt1++;
				break;
			}
		}
		if(flag == 0)
		{
			d2[cnt2] = d[2 + i];
			cnt2++;
		}
	}
   if(cnt2 == 0) return 0;
   //if(cnt1 == (roadnum - 2)) return 0;
/**********************************/
  int count = 0;//记录添加了多少次边
  int**adjMatrix_del;
  int *adjNumber_del;
  adjMatrix_del = (int**)malloc(sizeof(int*)*maxnode);
  for(int i = 0; i <= maxnode; i++)
  	{
  		adjMatrix_del[i] = (int*)malloc(sizeof(int)*8);
  	}
  adjNumber_del = (int*)malloc(sizeof(int)*maxnode);
  int *newroad = (int*)malloc(sizeof(int)*maxnode);
  int newroadnum;
  while(count != roadnum)
  {  int countx = count + 1;

	  delete_node(adjMatrix,maxnode,adjNumber,road,roadnum,count,countx,adjMatrix_del,adjNumber_del);
	  //dijkstra(road[count], road[countx], adjMatrix_del, adjNumber_del, &wMatrix[0][0],newroadnum,newroad);
	  int k = dijkstra(road[count],road[countx],adjMatrix_del,adjNumber_del,wMatrix,newroadnum,newroad);
	  if( k == 1)
	  {//如果添加成功,则把newroad添加到road中
		  //保存road中count后面的节点
		  int * temp = new int [roadnum - count];
		  for(int j = 0; j < roadnum -count; j++)
		  {
			  temp[j] = road[count + 1 + j];//不保存road[count+1]
		  }
		  for(int j = 1; j <= count + newroadnum - 2; j++)
		  {
			road[count + j] = newroad[j];
		  }

		  for(int j = 0; j < roadnum -count; j++)
		  {
			road[count + newroadnum -2 + j + 1] = temp[j];
		  }
		  roadnum += newroadnum - 2;
		  *Gcost = Rtweight(road,wMatrix,&adjMatrix[0][0],roadnum);
		  delete []temp;
		  return 1;//返回1表示插入成功

	  }else{
		  count++;
	  }

  }
  return 0;//返回0表示插入不成功
}

void autofitV(int **generation, int *Gnodenum, int groupnum, int *v, int vk, double *b){
	for (int i = 0; i < groupnum; i++)
	{
		for (int j = 0; j < Gnodenum[i]; j++)
		{
			for (int k = 0; k < vk; k++)
			{
				if (generation[i][j] == v[k])
				{
					b[i] = 1;//要调整的因子
				}
			}
		}

	}

}

void Sort(double * a,int *d, int *b,int **c,int Count)  //a是适应度，c是路径，b是节点个数，d是
{
	double temp;
	int temp1,temp2[maxn],temp3;
	for (int i = 1; i<Count; i++)  //外层每循环一次，将最小的一个移动到最前面
		for (int j = Count - 1; j >= i; j--)
			if (a[j] < a[j - 1])
			{
				temp = a[j];
				a[j] = a[j - 1];
				a[j - 1] = temp;
				temp3 = d[j];
				d[j] = d[j - 1];
				d[j - 1] = temp3;
				for (int q = 0; q < b[j]; q++)
					temp2[q] = c[j][q];
				for (int q = 0; q < b[j-1]; q++)
					c[j][q] = c[j-1][q];
				for (int q = 0; q < b[j]; q++)
					c[j-1][q] = temp2[q];
				temp1 = b[j];
				b[j] = b[j - 1];
				b[j - 1] = temp1;
			}
}

#endif /* GENEOPERATOR_H_ */

