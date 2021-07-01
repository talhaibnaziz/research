#include "DBCP.h"

DBCP::DBCP(int n, std::vector<edge> e)
{
    //ctor
    /**
    Initialize the variables of the network
    and create the adjacency list
    */
    nodecnt = n;
    linkcnt = e.size();

    for(int i=0; i<linkcnt; i++)
    {
        ugraph[e[i].src].push_back(e[i].des);
        ugraph[e[i].des].push_back(e[i].src);
    }

    /**
    Calculate all possible pair shortest
    path distances in terms of hop-count
    */
    for(int i=0; i<nodecnt; i++)
        for(int j=0; j<nodecnt; j++)
            dis[i][j] = -1;

    for(int srcNode=0; srcNode<nodecnt; srcNode++)
    {
        queue <int> q;
        q.push(srcNode);
        dis[srcNode][srcNode] = 0.0;
        while(!q.empty())
        {
            int u = q.front();
            q.pop();
            for(int i=0; i<ugraph[u].size(); i++)
            {
                int v = ugraph[u][i];
                if(dis[srcNode][v] == -1)
                {
                    dis[srcNode][v] = dis[srcNode][u] + 1.0;
                    q.push(v);
                }
            }
        }
    }
//    for(int i=0; i<nodecnt; i++)
//        cout<<"DISTANCE from "<<i<<": "<<dis[0][i]<<endl;
}

int* DBCP::cluster()
{
    ///Calculate the threshold dc
    dc=0.3*find_diameter();
//    cout<<"diameter: "<<find_diameter()<<"   dc: "<<dc<<endl;

    ///Calculate the density of the nodes
    for(int i=0; i<nodecnt; i++)
    {
        int cnt = 0;
        for(int j=0; j<nodecnt; j++)
            if(dis[i][j] < dc)  cnt++;
        ///Subtracting the node itself
        density[i] = cnt - 1.0;
//        cout<<"DENSITY of "<<i<<": "<<density[i]<<endl;
    }

    find_mindthdn();

    find_k();

    make_clusters();

    return cluster_id;
}

double DBCP::find_diameter()
{
    int node1, node2;
    double dis1 = 0, dis2 = 0;
    for(int i=0; i<nodecnt; i++)
    {
        if(dis1 < dis[0][i])
        {
            dis1 = dis[0][i];
            node1 = i;
        }
    }
    for(int i=0; i<nodecnt; i++)
    {
        if(dis2 < dis[node1][i])
        {
            dis2 = dis[node1][i];
            node2 = i;
        }
    }
    return dis2;
}

void DBCP::find_mindthdn()
{
    for(int i=0; i<nodecnt; i++)
    {
        int maxnode, node;
        double maxdis = INT_MIN, d = INT_MAX;
        for(int j=0; j<nodecnt; j++)
        {
            if(dis[i][j] > maxdis)
            {
                maxdis = dis[i][j];
                maxnode = j;
            }
            if(density[j]>density[i] && dis[i][j]<d)
            {
                d = dis[i][j];
                node = j;
            }
            if(d != INT_MAX && density[j]>density[node] && dis[i][j]==d)
            {
                d = dis[i][j];
                node = j;
            }
        }
        if(d == INT_MAX)
        {
            mindthdn[i] = maxdis;
            nearesthdn[i] = maxnode;
        }
        else
        {
            mindthdn[i]=d;
            nearesthdn[i]=node;
        }
    }
}

void DBCP::find_k()
{
    k = 0;

    double avg_mindthdn = 0;
    for(int i=0; i<nodecnt; i++)
        avg_mindthdn += mindthdn[i];

    avg_mindthdn /= (double)nodecnt;

    for(int i=0; i<nodecnt; i++)
    {
        if(mindthdn[i]>avg_mindthdn)
        {
            cluster_heads.push_back(i);
            cluster_id[i]=k;
            k++;
        }
//        cout<<"MIN DIS TO HDN "<<i<<": "<<mindthdn[i]<<endl;
    }
//    cout<<"AVG DELTA: "<<avg_mindthdn<<endl;
}

int DBCP::find_cluster(int n)
{
   if(clusters[n]==n) return n;
   return clusters[n] = find_cluster(clusters[n]);
}

void DBCP::make_clusters()
{
    memset(clusters, -1, sizeof clusters);
    queue <int> q;
    for(int i=0; i<cluster_heads.size(); i++)
        clusters[cluster_heads[i]] = cluster_heads[i];
    for(int i=0; i<nodecnt; i++)
        if(clusters[i]==-1) q.push(i);

    while(!q.empty())
    {
        int node = q.front();
        q.pop();
        /**
        If nearest higher density node is already clustered then
        include the node in that cluster, else reinsert into queue
        */
        if(clusters[node]==-1 && clusters[nearesthdn[node]]!=-1)
            clusters[node] = clusters[nearesthdn[node]];
        else    q.push(node);
    }

    for(int i=0; i<nodecnt; i++)
    {
        clusters[i]=find_cluster(i);
        cluster_id[i] = cluster_id[clusters[i]];
    }
}

std::vector<int> DBCP::controller_selection()
{
    for(int i=0; i<k; i++)
    {
        controllers.push_back(0);
        latencies.push_back(INT_MAX);
    }
    for(int i=0; i<nodecnt; i++)
    {
        avglat[i] = 0.0;
        maxlat[i] = 0.0;
        iclat[i] = 0.0;
        double cluster_size = 0.0;
        for(int j=0; j<nodecnt; j++)
        {
            if(clusters[i]==clusters[j])
            {
                avglat[i] += dis[i][j];
                maxlat[i] = max(maxlat[i], dis[i][j]);
                cluster_size = cluster_size + 1.0;
            }
            else    iclat[i] += dis[i][j];
        }
        if(cluster_size > 1)  avglat[i] /= cluster_size-1;
        else    avglat[i] = 0.0;
        if(cluster_size < nodecnt)    iclat[i] /= (double)nodecnt-cluster_size;
        else    iclat[i] = 0.0;
        int idx = cluster_id[i];
        if(latencies[idx] > (avglat[i]+maxlat[i]+iclat[i]))
        {
            latencies[idx] = avglat[i]+maxlat[i]+iclat[i];
            controllers[idx] = i;
        }
    }

    return controllers;
}

DBCP::~DBCP()
{
    //dtor
}
