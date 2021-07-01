#ifndef EVALUATION_H
#define EVALUATION_H

#define DEBUG cout<<"This line is for debugging"<<endl;
#define MAX_NODE_COUNT 500

#include "graph.h"

class Evaluation
{
    public:
        /*
        Arguments: List of cluster center IDs, List of controller IDs, All pair distances
        Cluster the network and assign switches to selected controllers for evaluation
        */
        Evaluation(std::vector <int> cluster_centers, std::vector <int> controllers, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], int n);

        //Returns the calculated overall latency "Omega"
        double get_omega();

        virtual ~Evaluation();

    protected:

    private:
        //cluster ID of each switch of the network
        int cluster[MAX_NODE_COUNT];

        //number of switches in the network
        int node_count;

        //number of controllers
        int c_count;

        //result of this class, overall latency
        double omega;
};

#endif // EVALUATION_H
