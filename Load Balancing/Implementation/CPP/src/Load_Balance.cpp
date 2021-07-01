#include "Load_Balance.h"

Load_Balance::Load_Balance(int *clusters, std::vector <int> controllers, double dis[MAX_NODE_COUNT][MAX_NODE_COUNT], int n)
{
    //ctor
    node_count = n;
    c_count = controllers.size();

    ///Replicate the clustered network
    for(int i=0; i<node_count; i++)
        this->cluster[i] = clusters[i];

    ///store list of controllers
    for(int i=0; i<c_count; i++)
    {
        this->controllers.push_back(controllers[i]);
        c_loads[i] = 0.0;
//        cout<<controllers[i]<<endl;
    }
}

void Load_Balance::randomize_load(double loads[MAX_NODE_COUNT])
{
//    srand(time(false));
    total_load = 0.0;
    //7.8 Millions mismatches can happen on average per switch
    for(int i=0; i<node_count; i++)
    {
        ///the load ranges from 1000 to 5000 new flows per second
//        s_loads[i] = (MIN_LOAD + (double)rand()*(MAX_LOAD - MIN_LOAD) / (double)RAND_MAX)/1000.0;

//        ///Loads created for testing
        s_loads[i] = (rand() % (MAX_LOAD - MIN_LOAD)) + (MIN_LOAD + 1);
        loads[i] = s_loads[i];

        int cluster_id = cluster[i];
        c_loads[cluster_id] += s_loads[i];
        total_load += s_loads[i];
//        cout<<"Load of "<<i<<" ("<<cluster[i]<<"):  "<<s_loads[i]<<endl;
    }
    ///To check how close the randomized values are to 8 Million
    cout<<"Average Load: "<< total_load / (double)node_count <<endl;
    cout<<setprecision(20)<<"Target Load: "<<total_load / (double)c_count <<endl;
}

int* Load_Balance::BestFS(node vertices[MAX_NODE_COUNT])
{
    cout<<"BLB Executing..."<<endl;

    double average_load = total_load / (double)c_count;
    double error = 0.0;
    for(int i=0; i<c_count; i++)
    {
        if(c_loads[i] > average_load)
            error += (c_loads[i] - average_load)*(c_loads[i] - average_load);
//        cout<<"Load of "<<controllers[i]<<" (C):  "<<c_loads[i]<<endl;
    }

    ///Calculate possible state changes
    int adj_node_count, u, v;
    int current_cluster, next_cluster;
    double cc_new_load, nc_new_load;
    double new_error;
    double best_error;
    int best_node, best_cluster;

    int iteration = 0;
    while(iteration++ < 100)
    {
        ///Find the best state change for each iteration
        best_error = error;
//        cout<<"CURRENT ERROR: "<<error<<endl;
        ///Printing loads of controllers before an iteration
//        for(int i=0; i<c_count; i++)
//            cout<<"Controller Load "<<i<<":  "<<c_loads[i]<<endl;
//        cout<<endl;

        for(int i=0; i<node_count; i++)
        {
            ///Find load of the controller of this node
            u = vertices[i].id;
            current_cluster = cluster[u];
            if(c_loads [current_cluster] < average_load)  continue;

            ///If current controller is overloaded
            adj_node_count = vertices[i].adjNodes.size();
            for(int j =0; j<adj_node_count; j++)
            {
                v = vertices[i].adjNodes[j].second;
                next_cluster = cluster[v];
                if(current_cluster != next_cluster)
                {
                    ///calculate cost of state change
                    new_error = error;

                    ///subtracting previous load differences from estimated error
                    new_error -= (c_loads[current_cluster] - average_load) * (c_loads[current_cluster] - average_load);
                    if(c_loads[next_cluster] > average_load)
                        new_error -= (c_loads[next_cluster] - average_load) * (c_loads[next_cluster] - average_load);

                    ///adding new load changes to estimated error
                    cc_new_load = c_loads[current_cluster] - s_loads[u];
                    nc_new_load = c_loads[next_cluster] + s_loads[u];
                    if(cc_new_load > average_load)
                        new_error += (cc_new_load - average_load) * (cc_new_load - average_load);
                    if(nc_new_load > average_load)
                        new_error += (nc_new_load - average_load) * (nc_new_load - average_load);

//                    cout<<"NODE: "<<u<<"\t New Cluster: "<<next_cluster<<"\t Error: "<<new_error<<endl;
                    if(new_error < best_error)
                    {
                        best_cluster = next_cluster;
                        best_node = u;
                        best_error = new_error;
//                        cout<<"Selected node: "<<best_node<<"  Selected Cluster: "<<best_cluster<<endl;
                    }
                }
            } ///End of checking adjacencies of a node
        } ///End of checking each node

        /**changing state to best option or terminating when
        there is not better state compared to current one*/
        if(best_error >= error) break;
        error = best_error;
        c_loads[cluster[best_node]] -= s_loads[best_node];
        c_loads[best_cluster] += s_loads[best_node];
        cluster[best_node] = best_cluster;

    } //End of iteration
    double check = 0.0, minmax_error = INT_MIN;
    for(int i=0; i<c_count; i++)
    {
//        cout<<c_loads[i]<<endl;
        check += c_loads[i];
    }
    cout<<check<<endl;
    for(int i=0; i<c_count; i++)
        minmax_error = max(minmax_error, c_loads[i]-check/(double)c_count);
    cout<<"Error: "<<minmax_error<<endl;
//    for(int i=0; i<node_count; i++)
//        cout<<"Node: "<<i<<"\tLoad: "<<s_loads[i]<<"\tCluster: "<<cluster[i]<<endl;
    return this->cluster;
}

Load_Balance::~Load_Balance()
{
    //dtor
}
