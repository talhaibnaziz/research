/**
Intelligent-SDN system version 2.0
@author: Talha Ibn Aziz

PROJECT BUILD DOCUMENTATIONS:
  1.Had to increase stack size to 256 MB
    using command "-Wl,--stack,268435456" in
    Project->Build options->Linker settings
    to avoid segmentation error due to
    large project memory allocation
  2.Had to increase stack size again to
    double, although might not have
    needed it
*/

#include <bits/stdc++.h>
#include <dirent.h>
using namespace std;

#include "graph.h"
#include "LBC.h"
#include "C_Placement.h"
#include "Load_Balance.h"
#include "Performance.h"

///Other algorithms for comparison
#include "DBC.h"
#include "DBCP.h"
#include "MSDA.h"

///Defined in a few other files too
#define DEBUG cout<<"This line is for debugging"<<endl;
#define MAX_NODE_COUNT 1000
#define TEST_CASE_COUNT 500
#define eps 10e-10
#define CUT_OFF 0.37

/**
number of controllers for each network according
to DBCP and collected by processing the result
*/
int All_k[TEST_CASE_COUNT] = {2, 6, 4, 2, 3, 1, 1, 2, 6, 1, 2, 3, 4, 6, 5, 7, 1, 8, 2, 2, 6, 1, 3, 1, 19, 19, 19, 19, 7, 7, 7, 7, 4, 2, 5, 2, 2, 2, 1, 2, 2, 1, 2, 8, 2, 11, 1, 3, 1, 1, 1, 1, 1, 1, 2, 4, 10, 1, 7, 1, 3, 1, 7, 5, 1, 3, 9, 2, 2, 3, 4, 2, 5, 5, 2, 2, 1, 6, 1, 12, 19, 19, 18, 20, 1, 18, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 2, 2, 4, 1, 3, 3, 1, 5, 8, 3, 4, 2, 2, 2, 6, 4, 2, 2, 7, 7, 3, 1, 6, 2, 6, 1, 1, 2, 7, 4, 3, 2, 4, 7, 7, 4, 5, 1, 10, 3, 2, 2, 1, 17, 2, 2, 3, 1, 7, 3, 3, 2, 17, 4, 8, 5, 1, 5, 2, 12, 4, 1, 4, 5, 12, 6, 3, 4, 15, 4, 1, 3, 2, 8, 7, 6, 3, 7, 1, 4, 1, 1, 1, 6, 6, 5, 3, 1, 3, 2, 1, 3, 3, 5, 8, 1, 1, 10, 7, 1, 6, 2, 4, 4, 9, 6, 4, 9, 2, 1, 3, 2, 1, 1, 1, 3, 1, 2, 4, 22, 27, 1, 4, 5, 8};

///all pair distances
double dis[MAX_NODE_COUNT][MAX_NODE_COUNT];

///All possible pair shortest path parents
int parent[MAX_NODE_COUNT][MAX_NODE_COUNT];

///list of ids of cluster centers
vector <int> cluster_centers;

///clustered network pointer, for other algorithms
int *clusters;

///Controller Data
int no_of_controllers = 0;
std::vector <int> controllers;

///Switch Loads
double s_loads[MAX_NODE_COUNT];

///The LBC and CSA algorithm function call
void lbc_csa();

///The BLB algorithm function call
void blb();

int main()
{
    /** This function will only call other functions and
    each function call will be explained and documented.
    However, this file will also contain other libraries
    and header files.*/
    ios_base::sync_with_stdio(false);

//    lbc_csa();

    blb();

    return 0;
}

void blb()
{
    DIR *pDIR;
    struct dirent *entry;
    int test_case = 0;
    double alpha, omega;
//    ifstream fin("./required_parameters/LBC_parameters.txt");

    ///For all networks of Zoo Topology
    if( pDIR=opendir("./dataset") ){
        while(entry = readdir(pDIR)){
            ///For entire dataset experimentation
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
            {
                ///For sample network experimentation
                if(strcmp(entry->d_name, "Kdl.txt") == 0)
                {
                    char file_name[100];
                    strcpy(file_name, "./dataset/");
                    strcat(file_name, entry->d_name);
                    cout<<file_name<<endl;

                    ///Set required parameters
                    alpha = 0.5, no_of_controllers = 10;

                    ///Initialize graph from input file
                    graph g1(file_name);

                    ///Calculate all pair latencies and average case latencies
                    g1.calculate_distances(dis, parent);

                    ///Cluster the network
                    LBC lbc3(g1.get_avg_distances(), g1.get_edges(), g1.get_node_count(), g1.get_min_bandwidth(), no_of_controllers);
                    cluster_centers = lbc3.cluster(dis);

                    ///Place controllers
                    C_Placement cp(g1.get_node_count(), no_of_controllers, dis, cluster_centers);
                    controllers = cp.set_Controllers(alpha, dis);
                    clusters = cp.get_clusters();

                    ///Evaluate flow-setup performance
                    Performance performance(g1.get_node_count(), no_of_controllers);
                    cout<<performance.calculate_omega(parent, dis, controllers, clusters)<<endl;

                    ///Assign switches based on Load
                    Load_Balance lb(clusters, controllers, dis, g1.get_node_count());
                    lb.randomize_load(s_loads);
                    clusters = lb.BestFS(g1.get_vertices());

                    double test1 = performance.calculate_net_omega(parent, dis, controllers, clusters, s_loads), test2;
                    cout<<test1<<endl;

//                    MSDA msda(g1.get_node_count(), g1.get_edges(), controllers);
//                    msda.matching(s_loads, clusters);
//
//                    double test2 = performance.calculate_net_omega(parent, dis, controllers, clusters, s_loads);
//                    cout<<test2<<endl;
//                    cout<<"IMPROVED by: "<<(test2-test1)/test2*100.0<<"%"<<endl;
//                    for(int i=0; i<g1.get_node_count(); i++)
//                    {
//                        cout<<"Node: "<<i<<"  Cluster: "<<clusters[i]<<"  Load: "<<s_loads[i]<<endl;
//                    }
                }
//                if(++test_case>10)  break;
            }
        }
        closedir(pDIR);
    }

//    fin.close();
}

void lbc_csa()
{
    DIR *pDIR;
    struct dirent *entry;
    int test_case = 0;
    long double old_omega, new_omega;
    long double prev_ratio, new_ratio;
    double alpha, rat;
    ofstream fout("./Output/LBC.txt");
//    ofstream fout("./Output/DBC.txt");
//    ofstream fout("./Output/DBCP.txt");
//    ofstream fout("./Output/best_LBC.txt");
//    ofstream v_fout("./Output/LBC_parameters.txt");
    ///DO NOT UNCOMMENT ANY TWO CONDITIONS SIMULTANEOUSLY
    ///For all networks of Zoo Topology
    if( pDIR=opendir("./dataset") ){
    ///For a single network extensive experimentation
//    if( pDIR=opendir("./single_datasets/Abvt") ){
        while(entry = readdir(pDIR)){
            ///For entire dataset experimentation
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
            {
                ///For sample network experimentation
                no_of_controllers = All_k[test_case];
//                if(strcmp(entry->d_name, "Ai3.txt") == 0)
                {
                    char file_name[100];
                    strcpy(file_name, "./dataset/");
    //                strcpy(file_name, "./single_datasets/Abvt/");
                    strcat(file_name, entry->d_name);
                    fout<<file_name<<endl;

                    ///Initialize graph from input file
                    graph g1(file_name);
                    fout<<g1.get_node_count()<<' '<<g1.get_edges().size()<<endl;

                    ///Calculate all pair latencies and average case latencies
                    g1.calculate_distances(dis, parent);

                    /**
                    This part should only be uncommented when working
                    with LBC, as it fine-tunes the values of 'k' and
                    'alpha' through multiple iterations
                    */
                    prev_ratio = LONG_LONG_MAX;
                    for(alpha = 0.5; abs(alpha-0.6)>eps; alpha += 0.1)
                    {
//                        no_of_controllers = 1;
//                        do
//                        {
                            ///Cluster the network
                            LBC lbc3(g1.get_avg_distances(), g1.get_edges(), g1.get_node_count(), g1.get_min_bandwidth(), no_of_controllers);
                            cluster_centers = lbc3.cluster(dis);
                            ///Place controllers
                            C_Placement cp(g1.get_node_count(), no_of_controllers, dis, cluster_centers);
                            controllers = cp.set_Controllers(alpha, dis);
                            clusters = cp.get_clusters();
                            ///Evaluate flow-setup performance
                            Performance performance(g1.get_node_count(), no_of_controllers);
                            new_omega = performance.calculate_omega(parent, dis, controllers, clusters);
//                            if(no_of_controllers == 1)  old_omega = new_omega;
//                            cout<<new_omega<<endl;
                            /**
                            Terminate if latency significantly reduced compared
                            to network with only one controller and save the 'k'
                            and alpha value from the best latency result
                            */
                            ///If cut-off improvement_rate is reached
//                            new_ratio = old_omega/new_omega/(double)no_of_controllers;
//                            if((prev_ratio-new_ratio) < 0.1)   break;
//                            prev_ratio = new_ratio;


                            ///Prepare for next iteration
//                            no_of_controllers++;
    //                        break;
//                        }while(no_of_controllers <= 10);
                    }

//                    for(int i=0; i<g1.get_node_count(); i++)
//                        cout<<g1.get_node_name(i)<<"\tCluster: "<<clusters[i]<<endl;
//                    for(int i=0; i<controllers.size(); i++)
//                        cout<<"Controllers: "<<g1.get_node_name(controllers[i])<<endl;
                    fout<<no_of_controllers<<' '<<new_omega<<endl;
//                    rat += (double)g1.get_node_count()/(double)no_of_controllers;
    //                v_fout<<best_k<<' '<<best_alpha<<endl;

                    ///Create an algorithm class with a given graph/network class
    //                LBC lbc3(g1.get_avg_distances(), g1.get_edges(), g1.get_node_count(), g1.get_min_bandwidth(), no_of_controllers);
    //                DBC dbc(g1.get_node_count(), no_of_controllers, g1.get_edges());
//                    DBCP dbcp(g1.get_node_count(), g1.get_edges());

                    ///Cluster the network
    //                cluster_centers = lbc3.cluster(dis);
    //                clusters = dbc.cluster();
//                    clusters = dbcp.cluster();

                    ///Select controllers based on Latency using a placement class
    //                C_Placement cp(g1.get_node_count(), no_of_controllers, dis, cluster_centers);
    //                controllers = cp.set_Controllers(alpha, dis);
    //                clusters = cp.get_clusters();
    //                controllers = dbc.set_Controllers(0.5, dis);
//                    controllers = dbcp.controller_selection();


//                    for(int i=0; i<g1.get_node_count(); i++)
//                        cout<<g1.get_node_name(i)<<"\tCluster: "<<clusters[i]<<endl;
//
//                    for(int i=0; i<controllers.size(); i++)
//                        cout<<"Controllers: "<<g1.get_node_name(controllers[i])<<endl;

                    ///Calculate Overall Latency of the Latency Based Solution
//                    Performance performance(g1.get_node_count(), controllers.size());
//                    cout<<controllers.size()<<' '<<performance.calculate_omega(parent, dis, controllers, clusters)<<endl;

                    ///For Experimentation with number of controllers
//                    new_omega = performance.calculate_omega(parent, dis, controllers, clusters);
    //                new_omega = new_omega * LATENCY_UNIT;
    //                new_omega = new_omega * 2.0 * 10;
//                    cout<<controllers.size()<<' '<<new_omega<<endl;
    //                if(abs(prev_omega-new_omega) > eps) cout<<prev_omega - new_omega<<endl;
    //                else    cout<<0.0<<endl;
    //                prev_omega = new_omega;

                    fout<<endl;
    //                alpha += 0.1;
    //                if(test_case == 20)    break;
                }
                test_case++;
            }
        }
        closedir(pDIR);
    }
//    cout<<rat/238.0<<endl;
//    fout.close();
//    v_fout.close();
}

