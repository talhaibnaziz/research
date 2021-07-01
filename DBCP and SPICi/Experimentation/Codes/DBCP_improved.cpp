#include <bits/stdc++.h>
using namespace std;

vector <pair<double, int> > G[1000];
double dc = 3.0;
int n, m;
double dis[1000][1000];
double density[1000];
double mindistohi[1000];
int nearesthi[1000];
double avg_mindistohi = 0;
int cluster_head[1000];
int k = 0;

int finddensity(int node)
{
    int cnt = 0;
    for(int i=1; i<=n; i++)
        if(dis[node][i]<dc) cnt++;
    return cnt-1;
}

int findmindistohi(int node)
{
    double d = 999999;
    for(int i=1; i<=n; i++)
    {
        if(density[node]<density[i])
        {
            if(d>dis[node][i])
            {
                d = dis[node][i];
                nearesthi[node] = i;
            }
            else if(d==dis[node][i] && density[i]>density[nearesthi[node]])
                nearesthi[node] = i;
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

void dijkstra(int src)
{
    priority_queue <pair<double, int> > q;
    pair <double, int> u, v;
    u.first = 0.0; u.second = src;
    q.push(u); dis[src][src] = 0.0;
    while(!q.empty())
    {
        u = q.top();
        q.pop();
        for(int i=0; i<G[u.second].size(); i++)
        {
            v = G[u.second][i];
            if(dis[src][v.second]==-1 || (dis[src][u.second]+v.first)<dis[src][v.second])
            {
                dis[src][v.second] = dis[src][u.second] + v.first;
                v.first = -1*dis[src][v.second];
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

    for(int i=1; i<=n; i++) if(mindistohi[i]>avg_mindistohi)    k++;

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
        if(mindistohi[i]>avg_mindistohi)
        {
            clusters[i] = ++cluster_no;
            cluster_head[i] = i;
        }
        else    q.push(i);
    }
    while(!q.empty())
    {
        int unassigned = q.front(); q.pop();
        if(clusters[nearesthi[unassigned]] == -1)   q.push(unassigned);
        else
        {
            clusters[unassigned] = clusters[nearesthi[unassigned]];
            cluster_head[unassigned] = cluster_head[nearesthi[unassigned]];
        }
    }
    /*for(int i=1; i<=n; i++)
        cout<<"node: "<<i<<"  cluster: "<<clusters[i]<<"  head: "<<cluster_head[i]<<endl;*/
}

void print_data()
{
    cout<<"clusters: "<<k<<endl;
    double maxdis = -1, ccdis = 0, avg = 0;
    int pressure = 0;
    vector <int> heads;
    for(int i=1; i<=n; i++)
        if(cluster_head[i]==i)  heads.push_back(i);
    for(int i=0; i<heads.size(); i++)
    {
        int cluster_size = 1;
        for(int j=1; j<=n; j++)
        {
            if(j==heads[i]) continue;
            if(cluster_head[j]==heads[i])
            {
                maxdis = max(maxdis, dis[heads[i]][j]);
                cluster_size++; avg += dis[heads[i]][j];
            }
        }
        pressure = max(pressure, cluster_size);
    }
    avg /= (double)(n-k);
    for(int i=0; i<heads.size()-1; i++)
        for(int j=i+1; j<heads.size(); j++)
            if(i!=j)    ccdis = ccdis + dis[heads[i]][heads[j]];
    ccdis /= (double)(k*(k-1))/2.0;
    cout<<"max distance to node: "<<maxdis<<endl;
    cout<<"avg distance to node: "<<avg<<endl;
    cout<<"cluster to cluster distance: "<<ccdis<<endl;
    cout<<"Final metric: "<<maxdis+avg+ccdis<<endl;
    cout<<"max nodes in a cluster: "<<pressure<<endl;
}

int main()
{
    freopen("input_3.txt", "r", stdin);
    cout<<"Take input: "<<endl;
    cin>>n>>m;
    int u, v; double w;
    for(int i=0; i<m; i++)
    {
        cin>>u>>v>>w;
        cout<<u<<' '<<v<<' '<<w<<endl;
        pair <double, int> p;
        p.first = w;
        p.second = v;
        G[u].push_back(p);
        p.second = u;
        G[v].push_back(p);
    }
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;

    //G is the graph of the network
    for(int i=1; i<=n; i++) dijkstra(i);

    memset(cluster_head, 0, sizeof cluster_head);
    algorithm1_findk();
    cout<<"\n\n";
    algorithm2_clustering();

    print_data();

    return 0;
}
