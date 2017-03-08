/*
* generate.h
*
*  Created on: 2016��4��6��
*      Author: FLYAWAY
*/

#ifndef GENERATE_H_
#define GENERATE_H_

void delete_branch_in(int adjMatrix[][8], int maxnode, int *adjNumber, int *d, int &demandnumber, int *v)
{
	int vk = 0;
	//������Щֻ��һ�����ȵıؾ��ڵ�
	/*for(int i = 0; i < demandnumber; i++)
	{
	if(adjNumber[d[2 + i]] == 1)
	{
	v[vk] = d[2 + i];
	vk++;

	}
	}*/


	for (int i = 0; i < demandnumber; i++)
	{
		int cnt = 0;//
		int flag = -1;
		for (int j = 0; j <= maxnode; j++)
		{
			for (int k = 0; k < adjNumber[j]; k++)
			{
				if (adjMatrix[j][k] == d[2 + i])
				{
					flag = j;
					cnt++;
					break;
				}

			}
		}

		if (cnt == 1){
			v[vk] = d[2 + i];
			adjMatrix[flag][0] = v[vk];
			//����Щֻ��һ�����ȵıؾ��ڵ����һ���ڵ���Ϊ�ؾ��ڵ㣬������һ�μ�֦
			d[2 + demandnumber] = flag;
			demandnumber += 1;
			//for(int i = 1 ; i < adjNumber[flag]; i++)
			//{
			//adjMatrix[flag][i] = -1;
			//}
			adjNumber[flag] = 1;
			vk++;
		}
	}
}

void delete_branch_out(int adjMatrix[][8], int maxnode, int *adjNumber, int *d, int &demandnumber)
{
	int s = 0;
	for (int i = 0; i < demandnumber; i++)
	{
		int cnt = 0;
		int flag = -1;
		if (adjNumber[d[i + 2]] == 1)
			cnt = 1;
		if (cnt == 1)
		{
			int u = adjMatrix[d[i + 2]][0];
			for (int k = 0; k <= maxnode; k++)
			{
				for (int z = 0; z < adjNumber[k]; z++)
					if (adjMatrix[k][z] == u)
					{
					adjMatrix[k][z] = -1;
					}
				if (k == d[i + 2] - 1)
					k++;
			}
			d[demandnumber + 2] = u;
			demandnumber++;
		}
	}
}

void delete_node(int adjMatrix[][8], int maxnode, int *adjNumber, int *road, int roadnum, int scr, int target, int **adjMatrix_del, int *adjNumber_del)
{//scr ����road��ѡ��road���±�Ϊscr�Ľڵ㵱��Դ�ڵ㣬ɾ��Ȳ�ɾ���ȣ�target����road��ѡ��road���±�Ϊscr�Ľڵ㵱��Ŀ��ڵ㣬ɾ���Ȳ�ɾ���

	//����ԭʼ�ڽӾ���,ɾ����Щ��road[target]֮�����Ϊ·�����Ѿ������ĵ�

	for (int j = 0; j <= maxnode; j++)
	{
		int cnt = 0;

		for (int k = 0; k < adjNumber[j]; k++)
		{
			int flag = 0;
			//���forѭ���ж�һ���нڵ��ǲ��ǰ�����road��,����õ���road[target]��ɾ������һ���ҵ���ɾ
			for (int i = 0; i < roadnum; i++)
			{
				if (adjMatrix[j][k] == road[target])
				{
					flag = 0;
					break;
				}

				if (adjMatrix[j][k] == road[i])
				{
					flag = 1;
					break;
				}
			}
			//����ýڵ㲻��road�����콾��adjMatrix��
			if (flag == 0)
			{
				adjMatrix_del[j][cnt] = adjMatrix[j][k];
				cnt++;
			}

		}
		adjNumber_del[j] = cnt;
	}
	//ɾ����road[src]��������road�еĽڵ�ĳ���
	for (int i = 0; i < roadnum; i++)
	{
		if (road[i] != road[scr])
		{
			for (int j = 0; j < adjNumber_del[road[i]]; j++)
			{
				adjMatrix_del[road[i]][j] = -1;
			}
			adjNumber_del[road[i]] = 0;//�ѳ��ȸ���-1��ʾ�����û�г�ȥ��·
		}
	}

}
#endif /* GENERATE_H_ */
