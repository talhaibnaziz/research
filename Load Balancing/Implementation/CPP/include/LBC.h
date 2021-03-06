/*
Latency Based Solution version 1.0
Author: Talha Ibn Aziz
DO NOT USE THIS IF YOU ARE WORKING
WITH A LATER VERSION OF LBC
*/

#ifndef LBC_H
#define LBC_H

#define DEBUG cout<<"This line is for debugging"<<endl;

#include "graph.h"

class LBC
{
    public:
        /*
        Arguments: sorted list of nodes according to average case latency, list
        of edges, number of nodes in the network and number of controllers
        This method saves the graphs taken as input and assigns necessary values
        */
        LBC(vector<pair<double, int> > avg_list, vector<edge> edges, int n, int k);

        //cluster the network using bfs and sorted average case latency
        vector <vector<int> > cluster_with_bfs();

        //cluster the network using Dijkstra and sorted average case latency
        vector <vector<int> > cluster_with_dijkstra();

        virtual ~LBC();

    protected:

    private:
        //Adjacency List of the network
        vector <int> adjList[MAX_NODE_COUNT];

        //Weighted adjacency List of the network
        vector <pair<double, int> > wadjList[MAX_NODE_COUNT];

        //sorted list of nodes according to average case latency
        vector<pair<double, int> > avg_list;

        //node count of the network
        int node_count;

        //number of clusters to be formed
        int cluster_count;

        /*node marking - -1 for not yet clustered,
        1 for cluster number 1, 2 for cluster no 2,
        and so on until k for cluster number k*/
        int node_mark[MAX_NODE_COUNT];

        /*The ultimate result of this algorithm
        A list of node list indicating each of
        the clusters and their switches*/
        vector <vector<int> > clusteredNetwork;

        //a local copy of the clustered networks
        vector<int> LocalNetwork[MAX_NODE_COUNT];

        //Node location names of Aarnet
        char nodenames[100][100] = {"Sydney1", "Brisbane2", "Canberra1", "Sydney2", "Townsville", "Cairns", "Brisbane1", "Rockhampton", "Armidale", "Hobart", "Canberra2", "Perth1", "Perth2", "Adelaide1", "Adelaide2", "Melbourne1", "Melbourne2", "Alice Springs", "Darwin"};
};

#endif // LBC_H
