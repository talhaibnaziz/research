#ifndef DBC_H
#define DBC_H

#include "graph.h"

/**
Degree-based Balanced Clustering (DBC)
which is the implementation of our
previous controller placement algorithm

 1. Although DBC can also work with weighted
    networks, it gives better results with
    unweighted networks and our previous paper
    also shows unweighted results
*/

class DBC
{
    public:
        /**
        The constructor which creates the graph
        and initializes the required variables
        */
        DBC(int n, int k, std::vector<edge> e);

        /**
        The main clustering function which also
        returns the clustered network pointer
        */
        int* cluster();

        ///This function selects controllers and then returns the controllers
        std::vector <int> set_Controllers(double alpha, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT]);

        virtual ~DBC();

    protected:

    private:
        ///total number of nodes in the network
        int nodecnt;

        ///total number of links in the network
        int linkcnt;

        ///number of controllers
        int k;

        /**
        The average degree of the network
        calculated using graph theory
        */
        double avg_degree;

        ///adjacency list of the network
        vector <int> ugraph[MAX_NODE_COUNT];

        ///List of cluster head node IDs
        vector <pair <double, int> > cluster_heads;

        ///List of nodes sorted according to average degrees
        vector <pair <double, int> > nodeList;

        ///All possible pair shortest path hop-distances
        double dis[MAX_NODE_COUNT][MAX_NODE_COUNT];

        ///The cluster id of each node
        int clusters[MAX_NODE_COUNT];

        ///The number of nodes in each cluster
        int cluster_size[MAX_NODE_COUNT];

        /**
        The list of controllers and their
        controller selection criterion latencies
        */
        vector <int> controllers;
        vector <double> latencies;

        ///controller selection criterion breakdown
        double avglat[MAX_NODE_COUNT], iclat[MAX_NODE_COUNT];
};

#endif // DBC_H
