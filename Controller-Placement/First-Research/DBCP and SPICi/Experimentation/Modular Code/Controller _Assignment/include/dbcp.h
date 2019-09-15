#ifndef DBCP_H
#define DBCP_H
#include "input.h"
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MX 110
#define infinity 999999


class dbcp
{
    public:
        dbcp();
        virtual ~dbcp();
        void set_dc(double dc);
        void set_input(input Input, bool weighted);
        int finddensity(int node);
        int findmindistohi(int node);
        void dijkstra(int src, bool savepath);
        void calculate_dis();
        int find_k();
        bool cluster(int k);
        void clustermod(int k);
        void set_controllers(int k);
        vector <double> deterministic_search(int k, int limit, int iter);
        vector <double> local_search(int limit, int iter, bool our);
        vector<double> output();
        vector<double> myoutput();
        void set_output(vector <double> output);
        vector <double> newlatency();
        void refresh(bool weighted);

    protected:

    private:
        vector <vector <pair<double, int> > > G;
        vector <vector <pair<double, int> > > originalG;
        vector <vector <pair<double, int> > > unweightedG;
        int k, n, m;
        double dc;
        double dis[MX][MX];
        double density[MX];
        double mindistohi[MX];
        int nearesthi[MX];
        double avg_mindistohi;
        vector <int> cluster_heads;
        int clusters[MX];
        double piavg[MX], pimax[MX], piinter[MX], pilatency[MX];
        vector <pair<double, int> > controllers;
        vector <double> pia, pim, pii;
        vector <double> current_output;
        bool vis[MX][MX];
        vector <int> path[MX][MX];
        bool once=0;
};

#endif // DBCP_H
