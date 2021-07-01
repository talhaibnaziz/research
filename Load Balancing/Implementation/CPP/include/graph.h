/**
Network version 1.0
Author: Talha Ibn Aziz
*/

#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODE_COUNT 1000
#define DEBUG cout<<"This line is for debugging"<<endl;

#include <bits/stdc++.h>

using namespace std;

struct node
{
    ///contains the node id
    int id;

    ///name of the node
    string node_name;

    /**contains list of connected nodes where the first value
    of pair is always 10.0/BANDWIDTH or LATENCY and the
    second value is node id*/
    vector <pair<double, int> > adjNodes;

    ///contains id of controller
    int cid;

    ///contains average case latency or distance from all other nodes
    double avg_dis;

    ///contains worst case latency or distance from all other nodes
    double max_dis;
};

struct edge
{
    ///source and destination node ids
    int src, des;
    ///link BANDWIDTH in Gbps
    double weight;
};

class graph
{
    public:
        ///Creates graph/network from passed text file location
        graph(char *file);

        virtual ~graph();

        ///Calculates all node to node distances using Dijkstra
        void calculate_distances(double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], int parent[MAX_NODE_COUNT][MAX_NODE_COUNT]);

        ///Typical get functions for various required private members
        std::vector<pair<double, int> > get_avg_list();
        node *get_vertices();
        std::vector<edge> get_edges();
        int get_node_count();
        std::vector<double> get_avg_distances();
        double get_min_bandwidth();
        double get_max_bandwidth();
        string get_node_name(int id);

        ///sorted list of nodes according to average case latency
        std::vector<pair<double, int> > avg_list;
        std::vector<double> avg_distances;

    protected:

    private:

        ///number of nodes in network
        int node_count;

        ///number of links in network
        int edge_count;

        ///maximum and minimum bandwidth of the network
        double max_bandwidth = INT_MIN;
        double min_bandwidth = INT_MAX;

        ///node ID of network center
        int centerID;

        ///Information of all vertex
        node vertices[MAX_NODE_COUNT];

        ///Information of all links/edges
        vector<edge> edges;
};

#endif // GRAPH_H
