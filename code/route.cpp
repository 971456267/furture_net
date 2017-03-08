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
int *Gcost;//��Ⱥ����·������
int wMatrix[maxn][8];//Ȩ�ؾ���
int **generation;//��Ⱥ
int *d;
int iteration ;			//��������
int COnum;//�������
int MUnum;//�������
int randroadnum;
int repeatnum = 0;
int famount;
int *nodenum;
int **road;
int *off1, *off2;//�Ӵ�
double *a;//�ؾ���Ȩ��
double *b;//������Щ��������ڵ�ĵ���Ⱥ
int *vax;//������Щֻ��һ�����Ȼ���ֻ��һ����ȵĵıؾ��ڵ�
int vk;//������Щֻ��һ�����Ȼ���ֻ��һ����ȵĵıؾ��ڵ�ĸ���
int * recordmin;//�±������Ǿ����ıؾ���ĸ��������ݴ������±���ؾ������������Ӵ��з��õ���Сֵ
int *Necnum;//����ÿ���Ӵ��а����ıؾ���ĸ���
int *finalfitness,countnum=0;
int maxnode;
int Repeat;

void groupiteration(int src, int target);
void initroute(int src, int target, int start, int roadnum);
int Crossover();
void init();
void add_edge(int a, int b, int c);
int allsearch(int src, int target, int *d);

//��Ҫ��ɵĹ��������
void search_route(char *topo[5000], int edge_num, char *demand)
{
	srand(time(NULL));
	printf("go to search_route:\n");
	init(); //��ʼ��firstΪ-1//���topo�ļ������еı�
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
	    iteration =10;			//��������
		//COnum = (int)groupnum*0.5;//�������
	    COnum = 0;
	    MUnum = (int)groupnum*0.1;//�������
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

	d = (int*)malloc(sizeof(int)*demand_number); //new(int);//ǰ������ֱ���Դ�ڵ���յ㣬�����ȫ���Ӽ�V��Ҳ���Ǳ���Ҫ�����ĵ�
	len = transfer_format_demand(demand, d);
	demandnumber = len - 2;
	int src = d[0];
	int target = d[1];
	int index;

	//�����Ŀ���ȥ�ĵ�
	memset(adjMatrix, -1, sizeof(adjMatrix));
	memset(wMatrix, 0, sizeof(wMatrix));
	memset(adjNumber, -1, sizeof(adjNumber));
	//cout<<"��ӡԭʼ�ڽӾ���"<<endl;
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
   //����һ��demand ��demandnumber�ĸ���
	int * d_copy = (int*)malloc(sizeof(int)*80);
	for(int i = 0; i < len; i++)
	{
		d_copy[i] = d[i];
	}
	int demandnumber_copy = demandnumber;
	delete_branch_in(adjMatrix,maxnode ,adjNumber,d_copy,demandnumber_copy, vax);
	delete_branch_out(adjMatrix, maxnode, adjNumber, d_copy, demandnumber_copy);
	/*cout<<"��ӡ����ڵ�ĸ�����"<<vk<<endl;
	for(int i = 0; i < vk ;i++)
		cout<<vax[i]<<" ";
	cout<<endl;
    cout<<"�����֦��ϵ��ڽӾ���"<<endl;
	for(int i = 0; i < maxnode; i++)
	{
		for(int j = 0; j < adjNumber[i]; j++)
		{
			cout<<adjMatrix[i][j]<<" ";
		}
		cout<<endl;
	}*/
	//��һ�δ�����ɾ��ֻ��һ����ȵıؾ��ڵ����һ���ڵ�ĵ��������ȣ����Ұ�����ڵ���Ϊ�ؾ��ڵ㣬���ýڵ�Ҳֻ��һ����ȣ��������һ���ڵ����������Ҳɾ����ֱ��û�������ĵ�Ϊֹ




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
	//����find_edge()���ڵ�ת���ɱ�
	int * result = (int*)malloc(sizeof(int)*(Gnodenum[index] - 1));
	find_edge(generation[index], Gnodenum[index] - 1, first, nextedge, v, result);
	//д�뵽result.csv

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
		flag=mutation(generation[k],&Gcost[k],Gnodenum[k],d,demandnumber,adjMatrix,adjNumber,&wMatrix[0][0]);	//����MUnum�α���
		flag= mutation(generation[k],maxnode,&Gcost[k],Gnodenum[k],d,demandnumber,adjMatrix,adjNumber,&wMatrix[0][0]);
		if(flag)
		{
			for(int i = 0; i < Gnodenum[k]; i++)
			{
				cout<<generation[k][i]<<" ";
			}
		}
	}*/
	initroute(src, target, cnt, randroadnum);//		�����cnt��ʼ���randroadnum�����·��
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

	Sort(fitness,Gcost, Gnodenum, generation, cnt);//����Ӧ�Ƚ�������
	tempgen = (int**)malloc(sizeof(int*)*cnt); //������ʱ�洢����
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
	for (int i = 1; i<cnt; i++)					//�޳��ظ���·��
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
	for (int i = 0; i <groupnum; i++)		//ÿ��ȡǰgroupnum��Ⱦɫ��
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
	free(tempGcost);			//�ͷ���ʱ����
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
			while (p1 == p2)//���ѡ��������ͬ�������н��棻
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
	for (i = start; i<roadnum + start; i++)  //���������ʼ·��
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
	for (int i = 0; i<famount; i++)  //���������ʼ·��
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

	for (int j = 0; j < Roadnum; j++)     //�ӳ�ʼ���ɵ����·����ѡȡ���������бؾ���ĳ�ʼ·��
	{
		if (designedjudge(road[j], d, nodenum[j], demandnumber) == 1)
		{
			generation[s] = road[j];
			Gnodenum[s] = nodenum[j];
			Gcost[s] = Rtweight(generation[s], &wMatrix[0][0], &adjMatrix[0][0], Gnodenum[s]);
			s++;
		}
	}
	int mincost = Gcost[0];  //�������·����Ȩ�أ�
	int index = 0;			//�������·�����±ꣻ
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
