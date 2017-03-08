#include<cstdlib>
#include<functional>
#include <cstring>
#include <queue>
#include <algorithm>
#include"reverse_route.h"
using namespace std;
typedef pair<int, int> pii;
int dijkstra(int src, int target ,int **adjMatrix, int *adjNumber, int *wMatrix, int & roadnum,int *road)
{
	int spend[600], prevnode[3000],flag=1;
	priority_queue <pii, vector<pii>, greater<pii> > q;
	memset(spend, -1, sizeof(spend));
	memset(prevnode, -1, sizeof(prevnode));
	spend[src] = 0;
	q.push(make_pair(0, src));
	while (!q.empty())
	{
		while (!q.empty() && q.top().first > spend[q.top().second])    q.pop();
		if (q.empty())   break;
		int u = q.top().second;
		q.pop();
		for (int i = 0; i <adjNumber[u]; i++)
		{
			//if (spend[*(adjMatrix + u * 8 + i)] == -1 || spend[*(adjMatrix + u * 8 + i)] > spend[u] + *(wMatrix + u * 8 + i))
			if (spend[adjMatrix[u][i]] == -1 || spend[adjMatrix[u][i]] > spend[u] + *(wMatrix + u * 8 + i))
			{
				//spend[*(adjMatrix + u * 8 + i)] = spend[u] + *(wMatrix + u * 8 + i);
				spend[adjMatrix[u][i]] = spend[u] + *(wMatrix + u * 8 + i);
				//prevnode[*(adjMatrix + u * 8 + i)] = u;
				prevnode[adjMatrix[u][i]] = u;
				//q.push(make_pair(spend[*(adjMatrix + u * 8 + i)], *(adjMatrix + u * 8 + i)));
				q.push(make_pair(spend[adjMatrix[u][i]], adjMatrix[u][i]));
			}
		}
	}
	int k = 0;
	for (int j = target; j != src; j = prevnode[j])
	{
		if(j<-1)
		{
			flag=0;
			break;
		}
		else
		{
		road[k] = j;
		k++;
		}
	}
	road[k] = src;
	roadnum = k + 1;
	if(flag==1)
	  reverseroute(road,roadnum);
	return flag;
}
