#include "idbc.h"

idbc::idbc()
{
    //ctor
}

void idbc::set_input(input Input)
{
    G = Input.get_graph();
    n = Input.get_nodescnt();
    m = Input.get_edgecnt();
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;
    for(int i=1; i<=n; i++) dijkstra(i);
}

void idbc::dijkstra(int src)
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

void idbc::cluster(int k)
{
    memset(clusters,-1,sizeof clusters);
    memset(cluster_size,0,sizeof cluster_size);
    int start = 1; double maxdis = 0.0;
    int lft, rt, cntr;
    for(int i=2; i<=n; i++)
    {
        if(dis[start][i]>maxdis)
        {
            maxdis = dis[start][i];
            lft = i;
        }
    }
    maxdis = 0;
    for(int i=1; i<=n; i++)
    {
        if(dis[lft][i]>maxdis)
        {
            maxdis = dis[lft][i];
            rt = i;
        }
    }
    cluster_heads.push_back(lft);
    cluster_heads.push_back(rt);
    clusters[lft]=0;
    clusters[rt]=1;
    if(k==2)    return;
    for(int i=1; i<=n; i++)
            combdis[i] = dis[i][lft];
    for(int i=3; i<=k; i++)
    {
        int nxthead;
        double maxdissum = 0;
        double mindis = infinity;
        for(int j=1; j<=n; j++)
        {
            combdis[j] += dis[j][cluster_heads[cluster_heads.size()-1]];
            cout<<j<<"  "<<combdis[j]<<' '<<maxdissum<<endl;
            if(combdis[j]>maxdissum)
            {
                maxdissum = combdis[j];
                nxthead = j; mindis = infinity;
                cout<<"next:  "<<j<<' '<<combdis[j]<<endl;
                for(int h=0; h<i-1; h++)
                {
                    int head = cluster_heads[h];
                    mindis = min(mindis, dis[head][j]);
                }
            }
            else if(combdis[j]==maxdissum)
            {
                double mdis = infinity;
                for(int h=0; h<i-1; h++)
                {
                    int head = cluster_heads[h];
                    mdis = min(mdis, dis[head][j]);
                }
                //cout<<"MINDIS:  "<<mindis<<"  "<<mdis<<endl;
                if(mdis>mindis)
                {
                    mindis = mdis;
                    nxthead = j;
                    cout<<"next:  "<<j<<' '<<combdis[j]<<endl;
                }
            }
        }
        cout<<"HEAD:  "<<nxthead<<endl;
        cluster_heads.push_back(nxthead);
        clusters[nxthead]=i-1;
    }
    for(int i=1; i<=n; i++)
    {
        double distohead = infinity;
        int clus, cs=0;
        if(clusters[i]==-1)
        {
            for(int j=0; j<k; j++)
            {
                int head = cluster_heads[j];
                if(dis[i][head]<distohead)
                {
                    distohead = dis[i][head];
                    clus = j; cs = cluster_size[j];
                }
                else if(dis[i][head]==distohead && cluster_size[head]<cs)
                {
                    clus = j;
                    cs = cluster_size[j];
                }
            }
            clusters[i] = clus;
            cluster_size[clus]++;
        }
    }
}

void idbc::output()
{
    for(int i=0; i<cluster_heads.size(); i++)
    {
        cout<<cluster_heads[i]<<": ";
        for(int j=1; j<=n; j++)
        {
            if(j==cluster_heads[i]) continue;
            if(clusters[j]==i)    cout<<' '<<j;
        }
        cout<<endl;
    }
}

idbc::~idbc()
{
    //dtor
}
