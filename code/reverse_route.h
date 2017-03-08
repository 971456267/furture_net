void reverseroute( int *route,int routenum)
{
        int t = 0;
		for (int i = 0; i < routenum/2; i++)
		{
			t = *(route + i);
			*(route + i) = *(route + routenum - 1 - i);
			*(route + routenum - 1 - i) = t;
		}
}
