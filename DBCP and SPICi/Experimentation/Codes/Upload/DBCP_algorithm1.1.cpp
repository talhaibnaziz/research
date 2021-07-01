#include <bits/stdc++.h>
using namespace std;

vector <int> G[1000];
int dc = 2;
int n, m;
int dis[1000][1000];
int density[1000];
int mindistohi[1000];
int nearesthi[1000];
double avg_mindistohi = 0;
int k = 0;

int finddensity(int node)
{
    int cnt = 0;
    for(int i=1; i<=n; i++)
        if(dis[node][i]<dc) cnt++;
    return cnt;
}

int findmindistohi(int node)
{
    int d = 999999;
    for(int i=1; i<=n; i++)
    {
        if(density[node]<density[i])
        {
            if(d>dis[node][i])
            {
                d = dis[node][i];
                nearesthi[node] = i;
            }
        }
    }
    if(d == 999999)
    {
        d = 0;
        for(int i=1; i<=n; i++)
        {
            if(d<dis[node][i])
            {
                d = dis[node][i];
                nearesthi[node] = i;
            }
        }
    }
    return d;
}

void bfs(int src)
{
    queue <int> q;
    int u, v;
    q.push(src); dis[src][src] = 0;
    while(!q.empty())
    {
        u = q.front();
        q.pop();
        for(int i=0; i<G[u].size(); i++)
        {
            v = G[u][i];
            if(dis[src][v]==-1)
            {
                dis[src][v] = dis[src][u] + 1;
                q.push(v);
            }
        }
    }
}

void algorithm1_findk()
{
    for(int i=1; i<=n; i++) density[i] = finddensity(i);
    for(int i=1; i<=n; i++)
    {
        mindistohi[i] = findmindistohi(i);
        avg_mindistohi += mindistohi[i];
    }
    avg_mindistohi /= (double)n;

    for(int i=1; i<=n; i++) if((double)mindistohi[i]>avg_mindistohi)    k++;

    for(int i=1; i<=n; i++)
    {
        cout<<"NODE: "<<i<<"  DENSITY: "<<density[i]<<"  min dis to higher density: "<<mindistohi[i]<<'('<<nearesthi[i]<<')'<<endl;
        for(int j=1; j<=n; j++)
            cout<<"distance to "<<j<<": "<<dis[i][j]<<endl;
    }
    cout<<"average: "<<avg_mindistohi<<"  no of controllers: "<<k<<endl;
}

int clusters[1000];

void algorithm2_clustering()
{
    int cluster_no = 0;
    memset(clusters, -1, sizeof clusters);
    queue <int> q;
    for(int i=1; i<=n; i++)
    {
        if((double)mindistohi[i]>avg_mindistohi)    clusters[i] = ++cluster_no;
        else    q.push(i);
    }
    while(!q.empty())
    {
        int unassigned = q.front(); q.pop();
        if(clusters[nearesthi[unassigned]] == -1)   q.push(unassigned);
        else    clusters[unassigned] = clusters[nearesthi[unassigned]];
    }
    for(int i=1; i<=n; i++)
        cout<<"node: "<<i<<"  cluster: "<<clusters[i]<<endl;
}

int main()
{
    cout<<"Take input: "<<endl;
    cin>>n>>m;
    int u, v;
    for(int i=0; i<m; i++)
    {
        cin>>u>>v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    memset(dis, -1, sizeof dis);

    //G is the graph of the network
    for(int i=1; i<=n; i++) bfs(i);

    algorithm1_findk();
    cout<<"\n\n";
    algorithm2_clustering();

    return 0;
}
/*
graph input:
8 11
1 2
1 5
1 6
6 5
6 2
2 7
2 4
4 8
3 8
2 3
5 3
*/