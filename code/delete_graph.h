/*
* generate.h
*
*  Created on: 2016年4月6日
*      Author: FLYAWAY
*/

#ifndef GENERATE_H_
#define GENERATE_H_

void delete_branch_in(int adjMatrix[][8], int maxnode, int *adjNumber, int *d, int &demandnumber, int *v)
{
	int vk = 0;
	//保存那些只有一个出度的必经节点
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
			//把那些只有一个出度的必经节点的上一个节点标记为必经节点，进行下一次减枝
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
{//scr 代表road中选中road中下标为scr的节点当做源节点，删入度不删出度，target代表road中选中road中下标为scr的节点当做目标节点，删出度不删入度

	//保存原始邻接矩阵,删掉那些除road[target]之外入度为路径中已经经过的点

	for (int j = 0; j <= maxnode; j++)
	{
		int cnt = 0;

		for (int k = 0; k < adjNumber[j]; k++)
		{
			int flag = 0;
			//这个for循环判断一行中节点是不是包含在road中,如果该点是road[target]则不删，否则一旦找到就删
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
			//如果该节点不在road中则天骄到adjMatrix中
			if (flag == 0)
			{
				adjMatrix_del[j][cnt] = adjMatrix[j][k];
				cnt++;
			}

		}
		adjNumber_del[j] = cnt;
	}
	//删除除road[src]的所有在road中的节点的出度
	for (int i = 0; i < roadnum; i++)
	{
		if (road[i] != road[scr])
		{
			for (int j = 0; j < adjNumber_del[road[i]]; j++)
			{
				adjMatrix_del[road[i]][j] = -1;
			}
			adjNumber_del[road[i]] = 0;//把长度负成-1表示这个点没有出去的路
		}
	}

}
#endif /* GENERATE_H_ */
