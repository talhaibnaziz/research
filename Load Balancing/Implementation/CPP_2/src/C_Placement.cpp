#include "C_Placement.h"

C_Placement::C_Placement(int n, int k, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], std::vector <int> cluster_centers)
{
    //ctor
    c_count = k;
    node_count = n;
    int cc;
    double cc_to_node;
    bool eq = false;
    for(int i=0; i<c_count; i++)    this->cluster_size[i] = 0;
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
            if(dis[cc][i] == cc_to_node)
            {
                if(eq)
                {
                    cc_to_node = dis[cc][i];
                    this->cluster[i] = j;
                }
                eq = !eq;
            }
        }
        this->cluster_size[cluster[i]]++;
    }
}

std::vector <int> C_Placement::set_Controllers(double alpha, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT])
{
    ///Find intra_controller and inter_controller distances
    double avg_inter = 0.0, avg_intra = 0.0;
    for(int i=0; i<node_count; i++)
    {
        intra_controller_distance[i] = 0.0;
        inter_controller_distance[i] = 0.0;
        for(int j=0; j<node_count; j++)
        {
            if(cluster[i] == cluster[j])
                intra_controller_distance[i] += dis[i][j];
            else    inter_controller_distance[i] += dis[i][j];
        }
        if(c_count > 1) inter_controller_distance[i] /= (double)(node_count-cluster_size[cluster[i]]);
        intra_controller_distance[i] /= (double)(cluster_size[cluster[i]]);
        avg_inter += inter_controller_distance[i];
        avg_intra += intra_controller_distance[i];
    }
    avg_inter /= (double)node_count;
    avg_intra /= (double)node_count;
//    cout<<"Latencies: "<<avg_inter<<' '<<avg_intra<<endl;
    if(c_count==1)  avg_inter = 1.0;
    /**
    Assign controllers based on the value of alpha
    Extreme cases like on controller or all controllers
    are automatically handled
    */
    for(int i=0; i<c_count; i++)
    {
        long double total_latency = LONG_LONG_MAX;
        long double temp_latency;
        int selected_controller;
        for(int j=0; j<node_count; j++)
        {
            if(cluster[j] == i)
            {
                temp_latency = intra_controller_distance[j]*avg_inter*alpha;
                temp_latency += inter_controller_distance[j]*avg_intra*(1.0-alpha);
                if(temp_latency < total_latency)
                {
                    total_latency = temp_latency;
                    selected_controller = j;
                }
            }
        }
        controllers.push_back(selected_controller);
    }
    return controllers;
}

int* C_Placement::get_clusters()
{
    return cluster;
}

C_Placement::~C_Placement()
{
    //dtor
}
