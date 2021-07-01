#ifndef DBCP_H
#define DBCP_H

#include "graph.h"

class DBCP
{
    public:
        /**
        This function initializes the network
        from given edges and finds all possible
        pair hop-distances using BFS
        */
        DBCP(int n, vector<edge> e);

        ///Return the list of clustered nodes
        int* cluster();

        ///Calculates the diameter of the network
        double find_diameter();

        /**
        Calculate the nearest higher density node
        and the distance to the nearest higher
        density node
        */
        void find_mindthdn();

        void find_k();

        void make_clusters();

        int find_cluster(int n);

        std::vector<int> controller_selection();

        virtual ~DBCP();

    protected:

    private:
        /**
        Network details - total number of nodes,
        total number of edges, number of controllers
        */
        int nodecnt, linkcnt, k;

        ///adjacency list of the network
        vector <int> ugraph[MAX_NODE_COUNT];

        ///All possible pair shortest path hop-distances
        double dis[MAX_NODE_COUNT][MAX_NODE_COUNT];

        /**
        Clustered list of network nodes, representing
        cluster head and cluster id of each node
        */
        int clusters[MAX_NODE_COUNT];
        int cluster_id[MAX_NODE_COUNT];

        ///Threshold of distance
        double dc;

        /**
        Density of the network switches, distance
        to nearest higher density node and the
        nearest higher density node
        */
        double density[MAX_NODE_COUNT];
        double mindthdn[MAX_NODE_COUNT];
        int nearesthdn[MAX_NODE_COUNT];

        vector <int> cluster_heads;

        vector <int> controllers;
        vector <double> latencies;

        double avglat[MAX_NODE_COUNT];
        double maxlat[MAX_NODE_COUNT];
        double iclat[MAX_NODE_COUNT];
};

#endif // DBCP_H
