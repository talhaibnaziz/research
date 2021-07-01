#include "LBC.h"
#include <bits/stdc++.h>

using namespace std;

LBC::LBC(vector<pair<double, int> > avg_list, vector<edge> edges, int n, int k)
{
    //ctor
    for(int i=0; i<edges.size(); i++)
    {
        int u = edges[i].src;
        int v = edges[i].des;
        int w = edges[i].weight;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        wadjList[u].push_back(make_pair(MAX_BANDWIDTH/w, v));
        wadjList[v].push_back(make_pair(MAX_BANDWIDTH/w, u));
    }
    node_count = n;
    cluster_count = k;
    for(int i=0; i<avg_list.size(); i++)
        this->avg_list.push_back(avg_list[i]);
    memset(node_mark, -1, sizeof(int)*n);
}

vector <vector<int> > LBC::cluster_with_bfs()
{
    int k = cluster_count;
    int n = node_count;
    int node_idx = 0;
    while(k!=1)
    {
        //select a new cluster center according to average case latency
        while(node_idx<n && node_mark[avg_list[node_idx].second]!=-1) node_idx++;
        int clusterCenter = avg_list[node_idx].second;
        node_mark[clusterCenter] = k;
        int cluster_size = 1;

        /*Apply BFS on current Cluster center and mark
        each node as it is visited. When number of visited
        nodes is equal to the size of a balanced cluster,
        stop the BFS to select a new center*/
        queue <int> q;
        q.push(clusterCenter);
        cout<<"CLUSTER CENTER: "<<clusterCenter<<endl;
        while(cluster_size<n/k && !q.empty())
        {
            int u = q.front();
            q.pop();
            int s = adjList[u].size();
            for(int i=0; i<s; i++)
            {
                int v = adjList[u][i];
                if(node_mark[v]==-1)
                {
                    node_mark[v] = k;
                    cluster_size++;
                    q.push(v);
                    if(cluster_size==n/k)   break;
                }
            }
        }
        n-=cluster_size;
        k--;
    }

    /*create clusters from marked node list and
    return the created sub-networks*/
    for(int i=0; i<node_count; i++)
    {
        if(node_mark[i]==-1)
            LocalNetwork[1].push_back(i);
        else    LocalNetwork[node_mark[i]].push_back(i);
    }
    for(int i=0; i<cluster_count; i++)
    {
        clusteredNetwork.push_back(LocalNetwork[i+1]);
        for(int j=0; j<LocalNetwork[i+1].size(); j++)
            cout<<LocalNetwork[i+1][j]<<' ';
        cout<<endl;
    }
    return clusteredNetwork;
}

vector <vector<int> > LBC::cluster_with_dijkstra()
{
    int k = cluster_count;
    int n = node_count;
    int node_idx = 0;
    while(k!=1)
    {
        //select a new cluster center according to average case latency
        while(node_idx<n && node_mark[avg_list[node_idx].second]!=-1) node_idx++;
        int clusterCenter = avg_list[node_idx].second;
        node_mark[clusterCenter] = k;
        int cluster_size = 0;

        /*Apply Dijkstra on current Cluster center and mark
        each node as it is visited. When number of visited
        nodes is equal to the size of a balanced cluster,
        stop the Dijkstra to select a new center*/
        priority_queue <pair<double, int> > pq;
        pq.push(make_pair(0.0, clusterCenter));
        while(cluster_size<n/k && !pq.empty())
        {
            int u = pq.top().second;
            double dsofar = -pq.top().first;
            pq.pop();

            /*mark the node that is extracted from the
            priority queue as a visited node*/
            node_mark[u] = k;
            cluster_size++;
            if(cluster_size==n/k)   break;

            cout<<"MARKED NODE: "<<nodenames[u]<<endl;

            int s = wadjList[u].size();
            for(int i=0; i<s; i++)
            {
                int v = wadjList[u][i].second;
                double w = wadjList[u][i].first;
                if(node_mark[v]==-1)
                    pq.push(make_pair(-(dsofar + w),v));
            }
        }
        n-=cluster_size;
        k--;
    }

    /*create clusters from marked node list and
    return the created sub-networks*/
    for(int i=0; i<node_count; i++)
    {
        if(node_mark[i]==-1)
            LocalNetwork[1].push_back(i);
        else    LocalNetwork[node_mark[i]].push_back(i);
    }
    for(int i=0; i<cluster_count; i++)
    {
        clusteredNetwork.push_back(LocalNetwork[i+1]);
        for(int j=0; j<LocalNetwork[i+1].size(); j++)
            cout<<nodenames[LocalNetwork[i+1][j]]<<' ';
        cout<<endl;
    }
    return clusteredNetwork;
}

LBC::~LBC()
{
    //dtor
}
