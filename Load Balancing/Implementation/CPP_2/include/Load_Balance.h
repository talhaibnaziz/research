#ifndef LOAD_BALANCE_H
#define LOAD_BALANCE_H

///All these are in terms of flows per second
#define MAX_LOAD 5000
#define MIN_LOAD 4000
#define MAX_PROCESSING_POWER 1000000

#include "graph.h"

class Load_Balance
{
    public:
        /**
        Initialize the class by storing lists of controllers and cluster centers
        and assigning nodes to their appropriate clusters for load balancing
        */
        Load_Balance(int *clusters, std::vector <int> controllers, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], int n);

        ///assign random reasonable load to switches
        void randomize_load(double loads[MAX_NODE_COUNT]);

        ///balance loads using Best First Search
        int* BestFS(node vertices[MAX_NODE_COUNT]);

        /**balance loads considering both average
        controller response time and distance from
        controller to switch*/
        int* BestFS_dual(node vertices[MAX_NODE_COUNT]);

        ///Calculate the distance of the farthest node from a controller
        double find_max_dis(int cluster_id);

        virtual ~Load_Balance();

    protected:

    private:
        ///list of controllers
        vector <int> controllers;

        ///cluster ID of each switch of the network
        int cluster[MAX_NODE_COUNT];

        ///number of switches in the network
        int node_count;

        ///number of controllers
        int c_count;

        ///switch loads
        double s_loads[MAX_NODE_COUNT];

        ///controller loads (based on cluster id NOT controller id)
        double c_loads[MAX_NODE_COUNT];

        ///total load of the network
        double total_load;

        ///pair to pair distance of the nodes
        double dis[MAX_NODE_COUNT][MAX_NODE_COUNT];
};

#endif // LOAD_BALANCE_H
