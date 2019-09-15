#ifndef INPUT_H
#define INPUT_H
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#define MX 110
using namespace std;


class input
{
    public:
        input();
        virtual ~input();
        void take_input(string str);
        int get_nodescnt();
        int get_edgecnt();
        vector<vector<pair<double,int> > > get_graph();
        void make_unweighted();
        double find_diameter();
        void dijkstra(int src);
        void clear_all();

    protected:

    private:
        int nodes, edges;
        double dis[MX][MX];
        vector <vector <pair<double, int> > > graph;
        vector <pair<double, int> > G[1000];
};

#endif // INPUT_H
