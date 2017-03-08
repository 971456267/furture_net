#include "Dijkstra.h"
#include "delete_graph.h"
#include "route.h"
#include "lib_record.h"
#include "transfer_format.h"
#include "initpath.h"
#include "geneoperator.h"
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <string.h>
#include <iostream>
#include <cstdio>

using namespace std;
#define demand_number  52
int groupnum;
int v[maxm], nextedge[maxm], w[maxm];
int first[maxn], e;
int len, flag = 0, demandnumber;
int adjMatrix[maxn][8];
int adjNumber[maxn];
int *selectedgene;
int *Gnodenum;
int *Gcost;//种群个体路径花费
int wMatrix[maxn][8];//权重矩阵
int **generation;//种群
int *d;
int iteration ;			//迭代次数
int COnum;//交叉次数
int MUnum;//变异次数
int randroadnum;
int repeatnum = 0;
int famount;
int *nodenum;
int **road;
int *off1, *off2;//子代
double *a;//必经点权重
double *b;//保存那些经过特殊节点的的种群
int *vax;//保存哪些只有一个出度或者只有一个入度的的必经节点
int vk;//保存哪些只有一个出度或者只有一个入度的的必经节点的个数
int * recordmin;//下标代表的是经过的必经点的个数，内容代表经过下标个必经点中所以有子代中费用的最小值
int *Necnum;//代表每个子代中包括的必经点的个数
int *finalfitness,countnum=0;
int maxnode;
int Repeat;

void groupiteration(int src, int target);
void initroute(int src, int target, int start, int roadnum);
int Crossover();
void init();
void add_edge(int a, int b, int c);
int allsearch(int src, int target, int *d);

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
	srand(time(NULL));
	printf("go to search_route:\n");
	init(); //初始化first为-1//添加topo文件里所有的边
	int *topo_int = (int*)malloc(sizeof(int)*edge_num * 3);
	//int maxnode;
	maxnode = transfer_format_topo(topo, edge_num, topo_int) + 1;
	//famount = maxnode * 100;
	//groupnum = maxnode * 10;

	if( 0<= maxnode  && maxnode <=80){
		famount = 50000;
		groupnum = maxnode * 100;
		Repeat = maxnode * 100;

	}

	if( 80< maxnode && maxnode <250){
		famount = 0;
		groupnum = maxnode * 6;
	    iteration =10;			//迭代次数
		//COnum = (int)groupnum*0.5;//交叉次数
	    COnum = 0;
	    MUnum = (int)groupnum*0.1;//变异次数
	    randroadnum = (int)groupnum*0.4;
		Repeat=maxnode*5;

	}
     if(maxnode >= 250){
    	 famount = 50000;
    	 groupnum = 5000;
	 Repeat = maxnode * 3;
     }

	for (int i = 0; i < edge_num; i++)
	{
		add_edge(*(topo_int + i * 3 + 0), *(topo_int + i * 3 + 1), *(topo_int + i * 3 + 2));
	}

	selectedgene = (int *)malloc(sizeof(int)*groupnum);

	generation = (int**)malloc(sizeof(int*)*groupnum * 2);
	for (int i = 0; i < groupnum * 2; i++)
	{
		generation[i] = (int *)malloc(sizeof(int)*maxn);
	}
	Gnodenum = (int*)malloc(sizeof(int)*(groupnum * 2));
	Gcost = (int*)malloc(sizeof(int)*(groupnum * 2));
	
	nodenum = (int*)malloc(sizeof(int)*famount);
	road = (int**)malloc(sizeof(int*)*famount);
	for (int i = 0; i < famount; i++)
	{
		road[i] = (int *)malloc(sizeof(int)*maxn);
	}

	off1 = (int *)malloc(sizeof(int)*maxn);
	off2 = (int *)malloc(sizeof(int)*maxn);
	
	vax = (int *)malloc(sizeof(int)*50);
	a = (double*)malloc(sizeof(double) * 51);
	b = (double*)malloc(sizeof(double) * 2 * groupnum);
	recordmin= (int *)malloc(sizeof(int)*51);
	finalfitness = (int *)malloc(sizeof(int)*groupnum);

	d = (int*)malloc(sizeof(int)*demand_number); //new(int);//前两个点分别是源节点和终点，后面的全是子集V，也就是必须要经过的点
	len = transfer_format_demand(demand, d);
	demandnumber = len - 2;
	int src = d[0];
	int target = d[1];
	int index;

	//保存点的可以去的点
	memset(adjMatrix, -1, sizeof(adjMatrix));
	memset(wMatrix, 0, sizeof(wMatrix));
	memset(adjNumber, -1, sizeof(adjNumber));
	//cout<<"打印原始邻接矩阵"<<endl;
	for (int j = 0; j <= maxnode; j++)
	{
		int k = 0;
		for (int i = first[j]; i != -1; i = nextedge[i])
		{
			adjMatrix[j][k] = v[i];
			//cout<<adjMatrix[j][k] <<" ";
			wMatrix[j][k] = w[i];
			k++;
		}
		//cout<<endl;
		adjNumber[j] = k;
	}
   //保存一个demand 和demandnumber的副本
	int * d_copy = (int*)malloc(sizeof(int)*80);
	for(int i = 0; i < len; i++)
	{
		d_copy[i] = d[i];
	}
	int demandnumber_copy = demandnumber;
	delete_branch_in(adjMatrix,maxnode ,adjNumber,d_copy,demandnumber_copy, vax);
	delete_branch_out(adjMatrix, maxnode, adjNumber, d_copy, demandnumber_copy);
	/*cout<<"打印特殊节点的个数："<<vk<<endl;
	for(int i = 0; i < vk ;i++)
		cout<<vax[i]<<" ";
	cout<<endl;
    cout<<"输出减枝完毕的邻接矩阵"<<endl;
	for(int i = 0; i < maxnode; i++)
	{
		for(int j = 0; j < adjNumber[i]; j++)
		{
			cout<<adjMatrix[i][j]<<" ";
		}
		cout<<endl;
	}*/
	//这一段代码是删掉只有一个入度的必经节点的上一个节点的的其他出度，并且把这个节点标记为必经节点，若该节点也只有一个入度，则把其上一个节点的其他出度也删掉，直到没有这样的点为止




	if (maxnode > 80 && maxnode < 250)
	{
		//cout<<" enter GA"<<endl;
		initroute(src, target, 0, groupnum);
		for (int i = 0; i < iteration; i++)
		{
			groupiteration(src, target);
			if (designedjudge(generation[0], d, Gnodenum[0], demandnumber) == 1)
			{
				break;
			}
			if (designedjudge(generation[0], d, Gnodenum[0], demandnumber) == 0 && i == iteration - 1)
				iteration = iteration + 2;
		}
		index = 0;
	}
	else
	{
		index = allsearch(src, target, d);
	}

	for (int i = 0; i < Gnodenum[index]; i++)
		cout << generation[index][i] << " ";
	cout << endl;
	//函数find_edge()将节点转换成边
	int * result = (int*)malloc(sizeof(int)*(Gnodenum[index] - 1));
	find_edge(generation[index], Gnodenum[index] - 1, first, nextedge, v, result);
	//写入到result.csv

	for (int i = 0; i < Gnodenum[index] - 1; i++)
	{
		cout << *(result + i) << " ";
		record_result((unsigned short)*(result + i));
	}
	cout << endl << Gcost[index] << endl;
	int j=designedjudge(generation[0], d, Gnodenum[0], demandnumber);
	cout<<j<<endl;
}

void groupiteration(int src, int target)
{	
	double *fitness;
	int *tempGcost,*tempGnum;
	int **tempgen;
	int cnt;
	fitness = (double*)malloc(sizeof(double)*(groupnum * 2));
	cnt = Crossover();
	/*for (int k = 0; k<MUnum; k++)
	{
		cout<<"mutation"<<endl;
	   int flag;
		flag=mutation(generation[k],&Gcost[k],Gnodenum[k],d,demandnumber,adjMatrix,adjNumber,&wMatrix[0][0]);	//进行MUnum次变异
		flag= mutation(generation[k],maxnode,&Gcost[k],Gnodenum[k],d,demandnumber,adjMatrix,adjNumber,&wMatrix[0][0]);
		if(flag)
		{
			for(int i = 0; i < Gnodenum[k]; i++)
			{
				cout<<generation[k][i]<<" ";
			}
		}
	}*/
	initroute(src, target, cnt, randroadnum);//		从起点cnt开始添加randroadnum条随机路径
	cnt = cnt + randroadnum;
	Necnum = (int *)malloc(sizeof(int)*cnt);
	for (int i = 0; i < 51; i++)
	{
		a[i] = 1;
		recordmin[i] = MAX;
	}
	for (int i = 0; i < cnt; i++)
	{
		Necnum[i] = 0;
	}
    countNum(generation, cnt, Gcost, Gnodenum, d, demandnumber, Necnum, recordmin);
	autofit(recordmin, a);
	//autofitV(generation,Gnodenum,groupnum,vax,vk,b);
	for (int i = 0; i < cnt; i++)
	{
		fitness[i] = Gcost[i] * a[Necnum[i]];
	}
	
	//double min = fitness[0];
	//	for(int i = 1; i < cnt; i++)
	//{
		//min = min < fitness[i]?min:fitness[i];
	//}

	//for(int i = 0; i < cnt; i++){
	//	if(b[i] == 1){
			//fitness[i] = min *1.3  ;//fitness[i] *0.8;
		//}
	//}

	Sort(fitness,Gcost, Gnodenum, generation, cnt);//按适应度进行排序
	tempgen = (int**)malloc(sizeof(int*)*cnt); //开辟临时存储变量
	for (int i = 0; i < cnt; i++)
		tempgen[i] = (int *)malloc(sizeof(int)*maxn);
	tempGnum = (int*)malloc(sizeof(int)*cnt);
	tempGcost = (int*)malloc(sizeof(int)*cnt);
	int tempcount = 0, flag = 0;
	tempGnum[0] = Gnodenum[0];
	tempGcost[0] = Gcost[0];
	for (int q = 0; q<Gnodenum[0]; q++)
		tempgen[0][q] = generation[0][q];
	tempcount++;
	for (int i = 1; i<cnt; i++)					//剔除重复的路径
	{
		flag = 0;
		for (int j = 0; j<i; j++)
			if (Gcost[i]==Gcost[j])
				if (routecmp(generation[i], generation[j], Gnodenum[j]) == 0)
				{
					flag = 1;
					break;
				}
		if (flag == 1)
		{
			continue;
		}
		else
		{
			tempGnum[tempcount] = Gnodenum[i];
			tempGcost[tempcount] = Gcost[i];
			for (int q = 0; q<Gnodenum[i]; q++)
				tempgen[tempcount][q] = generation[i][q];
			tempcount++;
		}
	}
	for (int i = 0; i < cnt; i++)
		memset(generation[i], 0, maxn);
	for (int i = 0; i <groupnum; i++)		//每次取前groupnum个染色体
	{
		 Gnodenum[i] = tempGnum[i];
		 Gcost[i] = tempGcost[i];
		 for (int q = 0; q<tempGnum[i]; q++)
			generation[i][q] = tempgen[i][q];
	}
	countnum++;
	if (countnum == iteration)
	{
		for (int i = 0; i < groupnum; i++)
			finalfitness[i] = fitness[i];
	}
	free(tempGcost);			//释放临时变量
	tempGcost = NULL;
	free(tempGnum);
	tempGnum = NULL;
	for (int i = 0; i < cnt; i++)
	{
		free(tempgen[i]);
		tempgen[i] = NULL;
	}
	free(tempgen);
	tempgen = NULL;
	free(Necnum);
	Necnum = NULL;
	free(fitness);
	fitness = NULL;
}

int Crossover()
{
	int cnt = groupnum;
	for (int i = 0; i < COnum; i++)
	{
		int success = 0, p1, p2, s11, s12, s21, s22, legal_1, legal_2;
		while (success != 1)
		{
			p1 = 0, p2 = 0;
			int c = (int)groupnum * 0.35;
			while (p1 == p2)//随机选择两个不同父代进行交叉；
			{
				p1 = rand() % c;
				p2 = rand() % c;
			}
			success = crossover(generation[p1], generation[p2], Gnodenum[p1], Gnodenum[p2], off1, off2);
		}
		s11 = routecmp(off1, generation[p1], Gnodenum[p2]);
		s12 = routecmp(off1, generation[p2], Gnodenum[p2]);
		s21 = routecmp(off2, generation[p1], Gnodenum[p1]);
		s22 = routecmp(off2, generation[p2], Gnodenum[p1]);
		legal_1 = roadjudge(off1, Gnodenum[p2], &adjMatrix[0][0], adjNumber);
		legal_2 = roadjudge(off2, Gnodenum[p1], &adjMatrix[0][0], adjNumber);
		if (s11 == 1 && s12 == 1 && legal_1 == 1)
		{
			for (int j = 0; j < Gnodenum[p2]; j++)
			{
				*(generation[cnt] + j) = *(off1 + j);
			}
			Gnodenum[cnt] = Gnodenum[p2];
			Gcost[cnt] = Rtweight(generation[cnt], &wMatrix[0][0], &adjMatrix[0][0], Gnodenum[cnt]);
			cnt++;
			memset(off1, 0, Gnodenum[p2]);
		}
		if (s21 == 1 && s22 == 1 && legal_2 == 1)
		{
			for (int j = 0; j < Gnodenum[p1]; j++)
			{
				*(generation[cnt] + j) = *(off2 + j);
			}
			Gnodenum[cnt] = Gnodenum[p1];
			Gcost[cnt] = Rtweight(generation[cnt], &wMatrix[0][0], &adjMatrix[0][0], Gnodenum[cnt]);
			cnt++;
			memset(off2, 0, Gnodenum[p1]);
		}
	}
	/*for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < Gnodenum[i]; j++)
			cout << generation[i][j] << " ";
		cout << Gcost[i] << endl;
	}*/
	return cnt;
}

void initroute(int src, int target, int start, int roadnum)
{//sttart = 0,roadnum = 1;
	int i;
	for (i = start; i<roadnum + start; i++)  //随机产生初始路径
	{
		flag = 0;
		Gnodenum[i] = initpath(src, target, &adjNumber[0], &adjMatrix[0][0], generation[i]);
		for (int k = 0; k<i; k++)
		{
			if (Gnodenum[i] == Gnodenum[k])
			{
				if (routecmp(generation[k], generation[i], Gnodenum[k]) == 0)
				{
					flag = 1;
					for (int s = 0; s<Gnodenum[i]; s++)
						generation[i][s] = 0;
					break;
				}
			}
		}
		if (flag == 1)
		{
			i--;
		}
		else
		{
			Gcost[i] = Rtweight(generation[i], &wMatrix[0][0], &adjMatrix[0][0], Gnodenum[i]);
			continue;
		}
	}
}

int allsearch(int src, int target, int *d)
{
	cout<<"enter voliet search"<<endl;
	int Roadnum = 0, s = 0;
	for (int i = 0; i<famount; i++)  //随机产生初始路径
	{
		flag = 0;
		nodenum[i] = initpath(src, target, &adjNumber[0], &adjMatrix[0][0], road[i]);
		for (int k = 0; k<i; k++)
		{
			if (nodenum[i] == nodenum[k])
			{
				if (routecmp(road[k], road[i], nodenum[k]) == 0)
				{
					flag = 1;
					for (int s = 0; s<nodenum[i]; s++)
						road[i][s] = 0;
					repeatnum++;
					break;
				}
			}
		}
		if (flag == 1)
		{
			i--;
		}
		else
		{
			repeatnum = 0;
			Roadnum++;
			continue;
		}
		if (repeatnum>Repeat)
			break;
	}

	for (int j = 0; j < Roadnum; j++)     //从初始生成的随机路径中选取出包含所有必经点的初始路径
	{
		if (designedjudge(road[j], d, nodenum[j], demandnumber) == 1)
		{
			generation[s] = road[j];
			Gnodenum[s] = nodenum[j];
			Gcost[s] = Rtweight(generation[s], &wMatrix[0][0], &adjMatrix[0][0], Gnodenum[s]);
			s++;
		}
	}
	int mincost = Gcost[0];  //保存最短路径的权重；
	int index = 0;			//保存最短路径的下标；
	for (int i = 1; i < s; i++){
		if (mincost >= Gcost[i])  {
			mincost = Gcost[i];
			index = i;
		}
	}
	return index;
}

void init()
{
	e = 0;
	memset(first, -1, sizeof(first));
}

void add_edge(int a, int b, int c)
{
	v[e] = b; nextedge[e] = first[a]; w[e] = c; first[a] = e++;
}
