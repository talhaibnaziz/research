#include "DBC.h"

DBC::DBC(int n, int k, std::vector<edge> e)
{
    //ctor
    nodecnt = n;
    linkcnt = e.size();
    this->k = k;

    ///Create graph (network connections) for DBC
    for(int i=0; i<linkcnt; i++)
    {
        int u = e[i].src;
        int v = e[i].des;
        double w = e[i].weight;
        ugraph[u].push_back(v);
        ugraph[v].push_back(u);
    }

    for(int i=0; i<nodecnt; i++)
    {
        nodeList.push_back(make_pair(ugraph[i].size(), i));
        avg_degree += ugraph[i].size();
    }
    avg_degree /= (double)nodecnt;
    sort(nodeList.begin(), nodeList.end());
    reverse(nodeList.begin(), nodeList.end());
//    cout<<avg_degree<<endl;

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
}

int* DBC::cluster()
{
    double threshold = (double)nodecnt/(double)k;
//    cout<<"THRESHOLD: "<<threshold<<endl;
    int cnt = 0;
    int idx = 0;

    ///Threshold Radius (Td) Determination
    double limit = 1+avg_degree;
    double boundary = avg_degree;
    double Td = 1.0;
    while(limit < threshold)
    {
//        cout<<"BOUNDARY: "<<boundary<<"   LIMIT: "<<limit<<endl;
        boundary *= (avg_degree - 1);
        limit += boundary;
        Td += 1;
    }
    Td -= 1;
//    cout<<"Td: "<<Td<<endl;

    ///Cluster head selection
    double maxDeg = nodeList[idx].first;
    while(idx < nodecnt && cnt < k)
    {
        int new_node = nodeList[idx].second;
        double deg = nodeList[idx].first;
        if(idx == 0)
        {
            cluster_heads.push_back(make_pair(deg, new_node));
            clusters[new_node] = cnt;
            cnt++; idx++;
//            cout<<"CLUSTER HEAD:  "<<new_node<<endl;
            continue;
        }
        bool inside_cluster = false;
        for(int i=0; i<cluster_heads.size(); i++)
        {
            int nex_node = cluster_heads[i].second;
            double nex_deg = cluster_heads[i].first;
            if(dis[nex_node][new_node] < Td*maxDeg/nex_deg)
                inside_cluster = true;
        }
        if(inside_cluster)
        {
            idx++;
//            cout<<idx<<" INSIDE CLUSTER"<<endl;
            continue;
        }
//        cout<<"CLUSTER HEAD:  "<<new_node<<endl;
        clusters[new_node] = cnt;
        cnt++; idx++;
        cluster_heads.push_back(make_pair(deg, new_node));
    }

    ///CLUSTERING THE REMAINING SWITCHES
    memset(cluster_size, 0, sizeof cluster_size);
    for(int i=0; i<nodecnt; i++)
    {
        int assigned_head;
        double d = INT_MAX;
        for(int j=0; j<cluster_heads.size(); j++)
        {
            int head = cluster_heads[j].second;
            if(dis[head][i]<d)
            {
                d = dis[head][i];
                assigned_head = head;
            }
        }
//        cout<<"NODE: "<<i<<"   CONTROLLER: "<<assigned_head<<endl;
        clusters[i] = clusters[assigned_head];
        cluster_size[clusters[i]]++;
    }

    return clusters;
}


std::vector <int> DBC::set_Controllers(double alpha, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT])
{
    for(int i=0; i<k; i++)
    {
        controllers.push_back(0);
        latencies.push_back(INT_MAX);
    }
    for(int i=0; i<nodecnt; i++)
    {
        avglat[i] = 0;
        iclat[i] = 0;
        double cs = cluster_size[clusters[i]];
        for(int j=0; j<nodecnt; j++)
        {
            if(clusters[i] == clusters[j])    avglat[i] += dis[i][j];
            else    iclat[i] += dis[i][j];
        }

        ///Handling extreme cases
        if(cs > 1)  avglat[i] /= cs-1;
        else    avglat[i] = 0.0;
        if(cs < nodecnt)    iclat[i]/=(double)nodecnt-cs;
        else    iclat[i] = 0.0;
        int idx = clusters[i];
        if(latencies[idx] > ((1.0-alpha)*avglat[i]+alpha*iclat[i]))
        {
            latencies[idx] = (1.0-alpha)*avglat[i]+alpha*iclat[i];
            controllers[idx] = i;
        }
    }

    return controllers;
}

DBC::~DBC()
{
    //dtor
}
