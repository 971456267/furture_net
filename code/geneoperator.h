/*
 * operator.h
 *
 *  Created on: 2016��3��30��
 *      Author: shijia
 */

#ifndef GENEOPERATOR_H_
#define GENEOPERATOR_H_
#include<cstdlib>
#include "delete_graph.h"
#include "Dijkstra.h"

int  MAX = 100000;
//#include "initpath.h"
int Rtweight(int *a,int *b,int *d,int c) //·����Ȩ�����ɺ�����a��·����b�Ƿ��þ���d���ڽӾ���c��·���ϵĽڵ����
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

int designedjudge(int *route, int *d, int nodenum,int demandnum)    //�Ƚ�·�������Ƿ�������бؾ��㣬�Ƿ���1�����򷵻�0
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
	//cout << "����select";
	double*fit = (double *)malloc(sizeof(double)*crossnum);  //fit ��Ӧ��
	double sum = 0;
	double *ratio=(double*)malloc(sizeof(double)*crossnum);//ѡ�����
	double *accratio = (double*)malloc(sizeof(double)*crossnum); //���۸���
	memset(ratio, 0, crossnum);
	memset(accratio, 0, crossnum);


	//������Ӧ��
	//cout << "������Ӧ��";
	for (int i = 0; i < crossnum; i++){
		*(fit + i) = 1 / Weight[i];
		cout << *(fit + i) << " ";
		sum += *(fit + i);
	}
	//cout <<endl<<sum <<endl;

	//����ѡ�����
	cout << endl << "����ѡ�����";
	for (int i = 0; i <crossnum; i++){
		ratio[i] = (*(fit + i)) / sum;
		cout << ratio[i] << " ";
	}
	cout << endl;
	////�����ݶȸ���
	cout << "�����ݶȸ���";
	for (int i = 0; i < crossnum; i++){
		for (int j = 0; j <= i; j++){
			accratio[i] += ratio[j];
		}
		cout << accratio[i] << " ";
	}
	cout << endl << "���Ⱦɫ�������";
	//�������Ⱦɫ�������,��Ϊֻ����groupnum��������ֻ����groupnum������
	double *pick = (double*)malloc(sizeof(double)*groupnum);
	memset(pick, 0, groupnum);
	for (int i = 0; i < groupnum; i++)
	{
		pick[i] = (((rand()) % 10001) / (10000 + 0.000001));
		//�������������������ĸ���
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
	//cout << "Ӧѡ��ĸ����ı��";
	for (int i = 0; i < groupnum; i++){
		//cout << *(selectedgene + i) << " ";
	}
}

void find_edge(int *d, int z, int first_1[], int next_1[], int v_1[], int *result)  //���ݽڵ�Ѱ�ұ�
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

int crossover(int *parent1, int *parent2, int p1nodenum, int p2nodenum, int *offspring1, int *offspring2)   //parent1��2Ϊ����Ⱦɫ�壬p1nodenumΪ����1�Ľڵ������offspring1Ϊ���ɵ��Ӵ�1������0��ʾ����ʧ�ܣ���·������С��3������˴ν���ɹ�
{
   // if(p1nodenum == p2nodenum){
    //	if(!routecmpA(parent1,parent2,p1nodenum)) return 0;
    //}
	int crossnode, nodenum;
	if (p1nodenum > p2nodenum)
		nodenum = p2nodenum;
	else
		nodenum = p1nodenum;
	if (nodenum <= 3)        //�����һ��������·��С�ڵ���3���ڵ㣬�򷵻ؽ��治�ɹ�
		return 0;
	else
	{

		crossnode = rand() % (nodenum - 3) + 2 ;   //����������ڵ�
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

int roadjudge(int *road, int roadnum, int *linkmatrix,int *nodematrix) //roadΪ·����roadnumΪ·���ϵĽڵ������linkmatrixΪ�ڽӾ���nodematrixΪ�ڵ��������·���Ϸ����жϺ������Ϸ�����1�����Ϸ�����0
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


//����ÿ��·��
void countNum(int **generation,int cnt,int *Gcost,int *Gnodenum,int *d, int demandnum,int *count,int *recordmin)
{
	//count����ÿһ�������ıؾ��ڵ����
	//recordmin,�±����ؾ��ڵ��������ֵ�����������±���Ŀ��ȵıؾ��ڵ�ʱ���д���·����̵Ļ��ѣ���ʼ����Ϊ���
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

//���ݰ����ıؾ��ڵ�ĸ�������Ӧ�������ӣ�w1*a1(����n���ؾ��ڵ�����·��xan)<w2*a2(����n-1���ؾ��ڵ�����·��xan-1)
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
	int d1[50];// new int[demandnum];//���road�Ѿ����ıؾ���
	int d2[50];// = new int[demandnum];//���roadû�о����ıؾ���
	int cnt1 = 0;//�����ĸ���
	int cnt2 = 0;//δ�����ĸ���
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
  int count = 0;//��¼����˶��ٴα�
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
	  {//�����ӳɹ�,���newroad��ӵ�road��
		  //����road��count����Ľڵ�
		  int * temp = new int [roadnum - count];
		  for(int j = 0; j < roadnum -count; j++)
		  {
			  temp[j] = road[count + 1 + j];//������road[count+1]
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
		  return 1;//����1��ʾ����ɹ�

	  }else{
		  count++;
	  }

  }
  return 0;//����0��ʾ���벻�ɹ�
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
					b[i] = 1;//Ҫ����������
				}
			}
		}

	}

}

void Sort(double * a,int *d, int *b,int **c,int Count)  //a����Ӧ�ȣ�c��·����b�ǽڵ������d��
{
	double temp;
	int temp1,temp2[maxn],temp3;
	for (int i = 1; i<Count; i++)  //���ÿѭ��һ�Σ�����С��һ���ƶ�����ǰ��
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

