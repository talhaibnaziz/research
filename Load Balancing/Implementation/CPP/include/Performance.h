#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "graph.h"
#include "Load_Balance.h"
/**
Unit of Latency (time required to send a
standard data packet of 1500 bytes) in milliseconds
which MUST be multiplied with the distance to
convert the distance into milliseconds
*/
#define LATENCY_UNIT 0.0111758708953857421875

struct path_node
{
    int destination;
    double des_dis;
    //the lesser the des_dis the lesser the structure
    bool operator<(const path_node& rhs) const
    {
        return des_dis < rhs.des_dis;
    }
};

class Performance
{
    public:

        /**
        Store number of nodes in the Constructor
        */
        Performance(int n, int k);

        /**Evaluate performance of any algorithm
        in terms of flow-setup latency*/
        double calculate_omega(int parent[MAX_NODE_COUNT][MAX_NODE_COUNT], double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], std::vector <int> controllers, int cluster[MAX_NODE_COUNT]);

        /**
        Evaluate performance of load balancing
        algorithms in terms of average response time
        */
        double calculate_delta(int clusters[MAX_NODE_COUNT], double s_loads[MAX_NODE_COUNT]);

        /**
        Calculate the flow-setup latency of the
        network including controller responses
        */
        double calculate_net_omega(int parent[MAX_NODE_COUNT][MAX_NODE_COUNT], double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], std::vector <int> controllers, int cluster[MAX_NODE_COUNT], double loads[MAX_NODE_COUNT]);

        virtual ~Performance();

    protected:

    private:
        ///number of nodes in the given network
        int node_count;

        ///number of clusters/controllers
        int c_count;

        ///overall latency of the network (omega)
        double omega;

        ///average flow-setup latency of a switch
        double delta[MAX_NODE_COUNT];

        ///total flow-setup latency of network
        double net_omega;

        ///average controller response time (ms)
        double tau[MAX_NODE_COUNT];
};

#endif // PERFORMANCE_H
