#include "LBC_2.h"

LBC_2::LBC_2(vector<pair<double, int> > avg_list, vector<edge> edges, int n, int k)
{
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
//    cout<<"AVERAGE CASE LATENCY"<<endl;
    for(int i=0; i<avg_list.size(); i++)
    {
//        cout<<nodenames[avg_list[i].second]<<": \t"<<avg_list[i].first<<endl;
        this->avg_list.push_back(avg_list[i]);
    }
    memset(node_mark, -1, sizeof(int)*n);
}

vector <vector<int> > LBC_2::cluster(vector <vector <double> > dis)
{
    int k = cluster_count;
    int n = node_count;
    int node_idx = 0;
    while(k)
    {
        //select a new cluster center according to average case latency
        while(node_idx<node_count && node_mark[avg_list[node_idx].second]!=-1) node_idx++;
        int clusterCenter = avg_list[node_idx].second;

        cout<<"CLUSTER CENTER: "<<nodenames[clusterCenter]<<"  "<<node_idx<<endl;
        node_mark[clusterCenter] = k;
        cluster_centers.push_back(clusterCenter);
        if(k==1)    break;
        int cluster_size = 1;

        /*Apply BFS on current Cluster center and mark
        each node as it is visited. When number of visited
        nodes is equal to the size of a balanced cluster,
        stop the BFS to select a new center*/
        queue <int> q;
        q.push(clusterCenter);
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
                    cout<<"MARKED NODE: "<<nodenames[v]<<"  Current Size("<<n/k<<"): "<<cluster_size<<endl;
                    //Might be required for a mesh network
                    //if(cluster_size==n/k)   break;
                }
            }
        }
        n-=cluster_size;
        k--;
    }

    /*create clusters from selected cluster-centers
    and return the created sub-networks*/
    for(int i=0; i<node_count; i++)
    {
        int idx = -1;
        double d = INT_MAX;
//        cout<<"NODE: "<<i<<endl;
        for(int j=0; j<cluster_count; j++)
        {
            int cc = cluster_centers[j];
//            cout<<dis[i][cc]<<endl;
            if(dis[i][cc]<d)
            {
                d = dis[i][cc];
                idx = j;
            }
        }
        LocalNetwork[idx].push_back(i);
    }
    for(int i=0; i<cluster_count; i++)
    {
        clusteredNetwork.push_back(LocalNetwork[i]);
        for(int j=0; j<LocalNetwork[i].size(); j++)
            cout<<nodenames[LocalNetwork[i][j]]<<' ';
        cout<<endl;
    }
    return clusteredNetwork;
}

LBC_2::~LBC_2()
{
    //dtor
}
