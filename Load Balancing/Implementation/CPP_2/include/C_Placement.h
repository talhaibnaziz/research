#ifndef C_PLACEMENT_H
#define C_PLACEMENT_H

#include "graph.h"

class C_Placement
{
    public:
        /**
        initializes the placement class by creating clusters from cluster centers
        Arguments: no of nodes, no of controllers, all pair distances, list of cluster centers
        */
        C_Placement(int n, int k, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], std::vector <int> cluster_centers);


        /**
        places controllers in an already clustered network based on
        inter controller and controller to switch distances
        */
        std::vector <int> set_Controllers(double alpha, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT]);

        ///Returns the clustered network pointer
        int* get_clusters();

        virtual ~C_Placement();

    protected:

    private:
        ///total number of clusters/controllers
        int c_count;

        ///total number of nodes in the network
        int node_count;

        ///cluster number of each node in the network
        int cluster[MAX_NODE_COUNT];

        ///Controller to switch distances of each node as controller
        double intra_controller_distance[MAX_NODE_COUNT];

        /**Controller to Controller distance assuming the cluster itself as
        the controller when the controller is not yet known*/
        double inter_controller_distance[MAX_NODE_COUNT];

        ///list of controller ids
        vector <int> controllers;

        ///Size of each cluster
        int cluster_size[MAX_NODE_COUNT];
};

#endif // C_PLACEMENT_H
