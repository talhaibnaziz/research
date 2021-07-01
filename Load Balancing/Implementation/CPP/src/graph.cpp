#include "graph.h"
#include <bits/stdc++.h>

using namespace std;

graph::graph(char *file)
{
    //ctor
    ifstream fin(file);
    int u, v;
    double w;
    struct edge e;
    node_count = 0;
    edge_count = 0;

    do{
        fin>>vertices[node_count].id;
        if(vertices[node_count].id == -1)
            break;
        getline(fin,vertices[node_count].node_name);
//        cout<<vertices[node_count].id<<' '<<vertices[node_count].node_name<<endl;
    }while(++node_count);

    while(fin>>u>>v>>w){
        e.src = u;
        e.des = v;
        e.weight = w;
        edges.push_back(e);
        max_bandwidth = max(max_bandwidth, w);
        min_bandwidth = min(min_bandwidth, w);
        edge_count++;
    }

    bool handled;
    for(int i=0; i<edge_count; i++){
        u = edges[i].src;
        v = edges[i].des;
        w = edges[i].weight;
        handled = false;
        /** Handle multiple edges from one source to
        destination (same source and destination) */
        for(int j=0; j<vertices[u].adjNodes.size(); j++)
        {
            /**
            If a similar edge already exists, decrease
            the latencies of two existing link accordingly
            */
            if(vertices[u].adjNodes[j].second == v)
            {
                double old_w = vertices[u].adjNodes[j].first;
                vertices[u].adjNodes[j].first = 1.0/(1.0/old_w + w);
                handled = true;
                break;
            }
        }
        for(int j=0; j<vertices[v].adjNodes.size(); j++)
        {
            /**
            If a similar edge already exists, decrease
            the latencies of two existing link accordingly
            */
            if(vertices[v].adjNodes[j].second == u)
            {
                double old_w = vertices[v].adjNodes[j].first;
                vertices[v].adjNodes[j].first = 1.0/(1.0/old_w + w);
                break;
            }
        }
        if(handled) continue;
        /**
        Dividing a normalized (1.0) packet with any bandwidth gives
        the time required to send the packet (transmission latency)
        */
        vertices[u].adjNodes.push_back(make_pair(1.0/w, v));
        vertices[v].adjNodes.push_back(make_pair(1.0/w, u));
    }
    fin.close();
}

void graph::calculate_distances(double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], int parent[MAX_NODE_COUNT][MAX_NODE_COUNT])
{
    /**
    Find all pair shortest path distances by using
    Dijkstra algorithm for each pair distance calculation
    and represent the distance as negative bandwidth to
    prioritize more bandwidth or less latency
    */
    for(int i=0; i<node_count; i++)
        for(int j=0; j<node_count; j++)
            dis[i][j] = -1;

    for(int srcNode=0; srcNode<node_count; srcNode++)
    {
        /**
        This priority queue will keep the pair
        with highest priority, meaning the highest
        value of first variable, on its top
        */
        priority_queue <pair<double, int> > pq;
        pq.push(make_pair(0.0, srcNode));
        dis[srcNode][srcNode] = 0.0;
        parent[srcNode][srcNode] = srcNode;
        while(!pq.empty())
        {
            pair <double, int> pp = pq.top();
            pq.pop();
            int s = vertices[pp.second].adjNodes.size();
            int des1Node = pp.second;
            double des1Dis = pp.first;

            for(int i=0; i<s; i++)
            {
                int des2Node = vertices[des1Node].adjNodes[i].second;
                double adjEdge = vertices[des1Node].adjNodes[i].first;
                if(dis[srcNode][des2Node]==-1 || dis[srcNode][des2Node] > dis[srcNode][des1Node] + adjEdge)
                {
                    dis[srcNode][des2Node] = dis[srcNode][des1Node] + adjEdge;
                    /**
                    As the priority queue keeps the highest distance on top
                    and we need the lowest distance on top, we push the
                    negative of the appropriate distance into the queue
                    */
                    pq.push(make_pair(-dis[srcNode][des2Node], des2Node));
                    parent[srcNode][des2Node] = des1Node;
                }
            }
        }
    }

    ///Find average case latency and worst case latency for all nodes
    double least_avg_dis = (double)INT_MAX;
    for(int i=0; i<node_count; i++)
    {
        vertices[i].avg_dis = 0.0;
        vertices[i].max_dis = -1;
        for(int j=0; j<node_count; j++)
        {
//            cout<<"DIS "<<vertices[i].node_name<<" to "<<vertices[j].node_name<<" = "<<dis[i][j]<<endl;
            vertices[i].avg_dis += dis[i][j];
            vertices[i].max_dis = max(dis[i][j], vertices[i].max_dis);
        }
        vertices[i].avg_dis /= (double)node_count;

        ///make separate lists of nodes and average case latencies
        avg_list.push_back(make_pair(vertices[i].avg_dis, i));
        avg_distances.push_back(vertices[i].avg_dis);
//        cout<<vertices[i].node_name<<":  "<<vertices[i].avg_dis<<endl;
    }
}

std::vector<pair<double, int> > graph::get_avg_list()
{
    /*Sort network nodes according to ascending average
    case latencies and determine network center*/
    sort(avg_list.begin(), avg_list.end());
    centerID = avg_list[0].second;

    return avg_list;
}

node *graph::get_vertices()
{
    return vertices;
}

std::vector <edge> graph::get_edges()
{
    return edges;
};

int graph::get_node_count()
{
    return node_count;
}

std::vector <double> graph::get_avg_distances()
{
    return avg_distances;
}

double graph::get_min_bandwidth()
{
    return min_bandwidth;
}

double graph::get_max_bandwidth()
{
    return max_bandwidth;
}

string graph::get_node_name(int id)
{
    return vertices[id].node_name;
}

graph::~graph()
{
    //dtor
}
