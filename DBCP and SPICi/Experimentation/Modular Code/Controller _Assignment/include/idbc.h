#ifndef IDBC_H
#define IDBC_H
#include "input.h"
#include <queue>
#include <string.h>
#define MX 110
#define infinity 999999


class idbc
{
    public:
        idbc();
        virtual ~idbc();
        void set_input(input Input);
        void dijkstra(int src);
        void cluster(int k);
        void output();

    protected:

    private:
        vector <vector <pair<double, int> > > G;
        int k, n, m;
        double dis[MX][MX], combdis[MX];
        int clusters[MX], cluster_size[MX];
        vector <int> cluster_heads;
};

#endif // IDBC_H
