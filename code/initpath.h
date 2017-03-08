#include<stdlib.h>
#include<time.h>
#include<string.h>
const int maxn = 600;
const int maxm = 4800;

int initpath(int src, int target, int *adjnum, int *linkm,int *route)
{
	int acmatrix[maxn][8];
	int  randnum, passed[maxn];
	int adj, nextadj,cnt=0, passednum[maxn];
	memset(passed, 0, sizeof(passed));
	memset(passednum, 0, sizeof(passednum));
	for (int i = 0; i < maxn; i++)
		for (int j = 0; j < adjnum[i]; j++)
			acmatrix[i][j] = 0;
	adj = src;
	route[0] = src;
	passed[src] = 1;
	while (adj != target)
	{
		if (passednum[adj] < *(adjnum + adj))
		{
			randnum = rand() % (*(adjnum + adj));
			nextadj = *(linkm + adj * 8 + randnum);
			if (nextadj == target)
			{
				cnt++;
				route[cnt] = nextadj;
				break;
			}
			else
			{
				if (passed[nextadj] != 1)
				{
					if (adjnum[nextadj] != 0)
					{
						passednum[adj]++;
						acmatrix[adj][randnum] = 1;
						adj = nextadj;
						cnt++;
						route[cnt] = adj;
						passed[nextadj] = 1;
						
					}
					else
					{
						acmatrix[adj][randnum] = 1;
						passed[nextadj] = 1;
						passednum[adj]++;
					}
				}
				else
				{
					if (acmatrix[adj][randnum]==0)
					{
						passednum[adj]++;
						acmatrix[adj][randnum] = 1;
					}
					else
						continue;
				}
			}
		}
		else
		{
			route[cnt] = -1;
			cnt--;
			adj = route[cnt];
		}
	}
	return cnt+1;
}

int routecmp(int *a,int *b,int c)
{
	int flag=0;
	for (int i=1;i<c;i++){    //因为源节点不需要进行比较
		if(*(a+i)!=*(b+i))
		{
			flag=1;
			break;

		}
	}
	return flag;
}
