#ifndef MSDA_H
#define MSDA_H

#include "graph.h"
#include "Load_Balance.h"

class MSDA
{
    public:
        /**Creates the unweighted graph, performs BFS to
        calculate pair to pair hop distances and stores
        the list of controllers in local variable*/
        MSDA(int n, vector <edge> e, std::vector <int> controllers);

        /**Performs the MSDA algorithm and returns the
        clustered network according to new assignment*/
        void matching(double s_loads[MAX_NODE_COUNT], int clusters[MAX_NODE_COUNT]);

        virtual ~MSDA();

    protected:

    private:
        ///Total number of switches and controllers in the network
        int nodecnt, c_cnt;

        ///adjacency list of the network
        vector <int> ugraph[MAX_NODE_COUNT];

        ///Node to node hop distances
        double dis[MAX_NODE_COUNT][MAX_NODE_COUNT];

        ///Loads of Controllers according to cluster ID
        double c_loads[MAX_NODE_COUNT];

        ///List of controllers
        vector <int> controllers;

        ///Switch's controller preference list
        vector < pair<double, int> > whichController[MAX_NODE_COUNT];

        /**Processing Power of controllers
        in number of flows per second*/
        double PP = MAX_PROCESSING_POWER;

        ///Load List of switches (global precedence)
        vector < pair<double, int> > LL;

        ///Maximum Quota in terms of flow queries
        double maxq[MAX_NODE_COUNT];

        ///Minimum Quota in terms of switch number
        double minq[MAX_NODE_COUNT];
};

#endif // MSDA_H
