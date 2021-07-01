#ifndef LBC_3_H
#define LBC_3_H

#include "graph.h"


class LBC_3
{
    public:
        /**
        Arguments: sorted list of nodes according to average case latency, list
        of edges, number of nodes in the network and number of controllers
        This method saves the graphs taken as input and assigns necessary values
        */
        LBC_3(std::vector<double> a, std::vector<edge> e, int n, double min_bandwidth, int k);

        /**
        Arguments: All pair switch distances of the network
        Return: Position of 'k' controllers.
        */
        std::vector<int> cluster(double dis[MAX_NODE_COUNT][MAX_NODE_COUNT]);

        /**Removes the node from the calculation by subtracting its distance from
        the average distances of other nodes.*/
        void remove_node(int idx, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT]);

        virtual ~LBC_3();

    protected:

    private:
        ///Adjacency List of the network
        vector <int> adjList[MAX_NODE_COUNT];

        ///Weighted adjacency List of the network
        vector <pair<double, int> > wadjList[MAX_NODE_COUNT];

        ///average case latencies in order of node ID
        vector<double> avg_distances;

        ///node count of the network
        int node_count;

        ///number of clusters to be formed
        int cluster_count;

        /**node marking - -1 for not yet clustered,
        1 for cluster number 1, 2 for cluster no 2,
        and so on until k for cluster number k*/
        int node_mark[MAX_NODE_COUNT];

        ///List of ids of cluster-centers
        vector <int> cluster_centers;

        /**The ultimate result of this algorithm
        A list of node list indicating each of
        the clusters and their switches*/
        vector <vector<int> > clusteredNetwork;

        ///a local copy of the clustered networks
        vector<int> LocalNetwork[MAX_NODE_COUNT];

};

#endif // LBC_3_H
