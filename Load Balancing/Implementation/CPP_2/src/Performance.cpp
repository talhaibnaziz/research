#include "Performance.h"

Performance::Performance(int n, int k)
{
    node_count = n;
    c_count = k;
    for(int i=0; i<c_count; i++)    tau[i] = 0.0;
}

double Performance::calculate_omega(int parent[MAX_NODE_COUNT][MAX_NODE_COUNT], double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], std::vector <int> controllers, int cluster[MAX_NODE_COUNT])
{
    /**find all possible pair latencies for SDN new flow query
    packet path and calculate overall average latency (omega)*/
    omega = 0.0;
    for(int i=0; i<node_count; i++)
    {
        for(int j=0; j<node_count; j++)
        {
            if(i==j)    continue;
            int src = i, des = j, c_des, c_src = controllers[ cluster[src] ];
            double latency = dis[src][c_src];
            while(des != src)
            {
                c_des = controllers[ cluster[des] ];
                latency = max(latency, dis[c_src][c_des] + dis[c_des][des]);
                des = parent[src][des];
            }
            ///For initial query message
            latency += dis[src][ controllers[ cluster[src] ] ];
            omega += latency;
//            cout<<"source: "<<src<<"\tdestination: "<<des<<"\tOmega"<<endl;
        }
    }
    omega /= (double)(node_count*(node_count-1));

    return omega;
}

double Performance::calculate_delta(int clusters[MAX_NODE_COUNT], double s_loads[MAX_NODE_COUNT])
{
//    delta = 0.0;
//    double PP = MAX_PROCESSING_POWER;
//    double c_loads[MAX_NODE_COUNT]={};
//    for(int i=0; i<node_count; i++)
//    {
//        int cluster_id = clusters[i];
//        c_loads[cluster_id] += s_loads[i];
////        cout<<i<<' '<<cluster_id<<endl;
//    }
//    for(int i=0; i<c_count; i++)
//    {
//        double sojourn = 1/(PP-c_loads[i]);
//        cout<<"CONTROLLER: "<<i<<"  Power: "<<PP<<"  Load: "<<c_loads[i]<<endl;
//        delta += sojourn;
//    }
//    delta /= (double)c_count;
//    return delta;
}

double Performance::calculate_net_omega(int parent[MAX_NODE_COUNT][MAX_NODE_COUNT], double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], std::vector <int> controllers, int cluster[MAX_NODE_COUNT], double loads[MAX_NODE_COUNT])
{
    net_omega = INT_MIN;
    ///Calculate average response times of controllers
    for(int i=0; i<node_count; i++)
    {
        int cluster_id = cluster[i];
        tau[cluster_id] += loads[i];
    }
    for(int i=0; i<c_count; i++)
    {
        ///Response time in seconds
        tau[i] = 1.0/(MAX_PROCESSING_POWER - tau[i]);
        ///converting to milliseconds
        tau[i] *= 1000.0;
//        cout<<"AVG RT of "<<i<<" (ms): "<<tau[i]<<endl;
    }

    ///Calculate average flow-setup latency of a switch
    for(int i=0; i<node_count; i++)
    {
        delta[i] = 0.0;
        for(int j=0; j<node_count; j++)
        {
            if(i==j)    continue;
            int src = i, des = j, c_des, c_src = controllers[ cluster[src] ];
            double latency = dis[src][c_src];
            while(des != src)
            {
                c_des = controllers[ cluster[des] ];
                latency = max(latency, dis[c_src][c_des] + tau[ cluster[des] ] + dis[c_des][des]);
                des = parent[src][des];
            }
            ///For initial query message
            latency += dis[src][c_src] + tau[ cluster[src] ];
            delta[i] += latency;
        }
        delta[i] /= (double)(node_count-1);
        ///converting to milliseconds
        delta[i] *= LATENCY_UNIT;
    }
    /**Calculate net omega or overall flow-setup
    latency of the entire network */
    for(int i=0; i<node_count; i++)
    {
        int cluster_id = cluster[i];
        net_omega = max(net_omega, delta[i]*loads[i]);
    }
    return net_omega;
}

Performance::~Performance()
{
    //dtor
}
