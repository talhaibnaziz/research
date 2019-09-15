#include "input.h"

input::input()
{
    //ctor
    edges = 0;
    nodes = 0;
}

void input::take_input(string str)
{
    char chs[50];
    for(int i=0; i<str.size(); i++) chs[i]=str[i];
    chs[str.size()]='\0';
    ifstream fin(chs);
    vector <pair<double, int> > nodelist;
    pair<double, int> p;
    cout<<"Enter number of nodes and edges:"<<endl;
    fin>>nodes>>edges;
    cout<<"Enter edges (u v cost)"<<endl;
    for(int i=0; i<edges; i++)
    {
        int u, v;
        double w;
        fin>>u>>v>>w;
        p.first = w;
        p.second = v;
        G[u].push_back(p);
        p.second = u;
        G[v].push_back(p);
    }
    for(int i=0; i<=nodes; i++)
    {
        nodelist = G[i];
        graph.push_back(nodelist);
    }
    fin.close();
}
void input::clear_all()
{
    for(int i=0; i<=nodes; i++)
    {
        G[i].clear();
        graph[i].clear();
    }
}
int input::get_nodescnt()
{
    return nodes;
}
int input::get_edgecnt()
{
    return edges;
}
vector <vector <pair<double, int> > > input::get_graph()
{
    return graph;
}
void input::make_unweighted()
{
    for(int i=1; i<=nodes; i++)
    {
        for(int j=0; j<graph[i].size(); j++)
        {
            graph[i][j].first = 1.0;
            G[i][j].first = 1.0;
        }
    }
}

void input::dijkstra(int src)
{
    priority_queue <pair<double, int> > q;
    pair <double, int> u, v;
    u.first = 0.0; u.second = src;
    q.push(u); dis[src][src] = 0.0;
    while(!q.empty())
    {
        u = q.top();
        q.pop();
        for(int i=0; i<G[u.second].size(); i++)
        {
            v = G[u.second][i];
            if(dis[src][v.second]==-1 || (dis[src][u.second]+v.first)<dis[src][v.second])
            {
                dis[src][v.second] = dis[src][u.second] + v.first;
                v.first = -1*dis[src][v.second];
                q.push(v);
            }
        }
    }
}

double input::find_diameter()
{
    for(int i=1; i<=nodes; i++)
        for(int j=1; j<=nodes; j++)
            dis[i][j] = -1;
    for(int i=1; i<=nodes; i++) dijkstra(i);
    double maxdis = -1;
    int lft, rt;
    for(int i=1; i<=nodes; i++)
    {
        if(dis[1][i]>maxdis)
        {
            maxdis = dis[1][i];
            lft = i;
        }
    }
    maxdis = -1;
    for(int i=1; i<=nodes; i++)
    {
        if(dis[lft][i]>maxdis)
        {
            maxdis = dis[lft][i];
            rt = i;
        }
    }
    return dis[lft][rt];
}

input::~input()
{
    //dtor
}
