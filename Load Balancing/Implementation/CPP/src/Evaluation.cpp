#include "Evaluation.h"

Evaluation::Evaluation(std::vector <int> cluster_centers, std::vector <int> controllers, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], int n)
{
    //ctor
    node_count = n;
    c_count = controllers.size();

    //Assign switches based on latency
    int cc;
    double cc_to_node;
    for(int i=0; i<node_count; i++)
    {
        cc_to_node = INT_MAX;
        for(int j=0; j<c_count; j++)
        {
            cc = cluster_centers[j];
            if(dis[cc][i] < cc_to_node)
            {
                cc_to_node = dis[cc][i];
                this->cluster[i] = j;
            }
        }
    }
}

double Evaluation::get_omega()
{
    return omega;
}
Evaluation::~Evaluation()
{
    //dtor
}
