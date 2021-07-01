#include "LBC_3.h"

LBC_3::LBC_3(std::vector<double> a, std::vector<edge> e, int n, double min_bandwidth, int k)
{
    //ctor

    ///Clearing and Initializing variables for algorithm execution
    node_count = n;
    cluster_count = k;

    for(int i=0; i<e.size(); i++)
    {
        int u = e[i].src;
        int v = e[i].des;
        double w = e[i].weight;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        wadjList[u].push_back(make_pair(w/min_bandwidth, v));
        wadjList[v].push_back(make_pair(w/min_bandwidth, u));
    }

    for(int i=0; i<node_count; i++)
    {
        avg_distances.push_back(a[i]*(double)node_count);
        node_mark[i] = -1;
    }
}

std::vector<int> LBC_3::cluster(double dis[MAX_NODE_COUNT][MAX_NODE_COUNT])
{
    int k = cluster_count;
    int n = node_count;
    int node_idx;
    double least_inv_priority;

    /**
    Handle the special case when the network needs
    to be divided into two equal parts
    */
//    if(k==2)
//    {
//        int node1, node2;
//        double min_avg1 = INT_MAX, min_avg2 = INT_MAX;
//        for(int i=0; i<node_count; i++)
//        {
//            if(avg_distances[i] < min_avg1)
//            {
//                min_avg2 = min_avg1;
//                node2 = node1;
//                min_avg1 = avg_distances[i];
//                node1 = i;
//            }
//            else if(avg_distances[i] < min_avg2)
//            {
//                min_avg2 = avg_distances[i];
//                node2 = i;
//            }
////            cout<<i<<' '<<avg_distances[i]<<endl;
//        }
//        cout<<min_avg1<<' '<<min_avg2<<endl;
//        cluster_centers.push_back(node1);
//        cluster_centers.push_back(node2);
//        return cluster_centers;
//    }
    while(k)
    {
        /**select a new cluster center according to average case latency
        and weighted degree. Find an equal weighted sum of latency and degree
        and choose the node with least sum as center*/
        least_inv_priority = INT_MAX;
        for(int i=0; i<node_count; i++)
        {
            if(node_mark[i]==-1)
            {
//                cout<<"CHANGE: "<<i<<"  avg: "<<avg_distances[i]<<endl;
                double inv_priority = avg_distances[i];
                if(inv_priority < least_inv_priority)
                {
                    least_inv_priority = inv_priority;
                    node_idx = i;
                }
            }
        }
        int clusterCenter = node_idx;

//        cout<<"CLUSTER CENTER: "<<clusterCenter<<" ("<<k<<")  AVG: "<<avg_distances[clusterCenter]<<endl;
        node_mark[clusterCenter] = k;
        remove_node(clusterCenter, dis);
        cluster_centers.push_back(clusterCenter);
        if(k==1)    break;
        int cluster_size = 1;

        /**Apply BFS on current Cluster center and mark
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
                    //node_mark[v] = k;
                    remove_node(v, dis);
                    cluster_size++;
                    q.push(v);
//                    cout<<"Marked: "<<nodenames[v]<<"  Current Size("<<n/k<<"): "<<cluster_size<<endl;
                    if(cluster_size==n/k)   break;
                }
            }
        }
        n-=cluster_size;
        k--;
    }

    return cluster_centers;
}

void LBC_3::remove_node(int idx, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT])
{
    for(int i=0; i<node_count; i++)
        avg_distances[i] -= dis[idx][i];
}

LBC_3::~LBC_3()
{
    //dtor
}
