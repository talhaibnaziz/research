#ifndef SPICI_H
#define SPICI_H
#include "input.h"
#include <queue>
#include <string.h>
#include <math.h>
#include <set>
#include <iostream>
#include <algorithm>
#include <time.h>
#define MX 110
#define infinity 999999


class spici
{
    public:
        spici();
        virtual ~spici();
        void set_ts(double ts);
        void set_td(double td);
        void set_input(input Input, bool weighted);
        pair <int, double> find_max_wdeg();
        void dijkstra(int src);
        int second_seed(int u);
        double density(vector <int> S, int addnode);
        double support(int t, vector <int> S);
        void cluster();
        void find_degree();
        vector <double> output();
        void set_controllers();
        double newlatency();
        vector <double> myoutput();
        int cluster_cnt;
        void set_myc(int myc);
        double latency;
        double local_search(int limit, int iter);

    protected:

    private:
        double ts, td, tg;
        vector <vector <pair<double, int> > > graph;
        vector <vector <pair<double, int> > > G;
        int k, n, m;
        double dis[MX][MX], cost[MX][MX];
        int clusters[MX];
        double piavg[MX], pimax[MX], piinter[MX], pilatency[MX];
        vector <pair<double, int> > controllers;
        vector <double> pia, pim, pii;
        int cluster_head[MX];
        vector <pair<double, int> > degree;
        double deg[MX];
        vector <int> path[MX][MX];
        int myc;
        bool vis[MX][MX];
};

#endif // SPICI_H
