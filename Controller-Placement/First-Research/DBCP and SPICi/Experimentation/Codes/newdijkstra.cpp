#include <bits/stdc++.h>
using namespace std;

double dis[20][20];
vector<int> path[20][20];
vector<pair<double, int> > G[20];

void dijkstra(int src)
{
    priority_queue <pair<double, vector<int> > > q;
    pair <double, vector<int> > u;
    pair <double, int> v;
    u.first = 0.0; u.second.push_back(src);
    q.push(u); dis[src][src] = 0.0;
    path[src][src] = u.second;
    while(!q.empty())
    {
        u = q.top();
        q.pop();
        for(int i=0; i<G[u.second[u.second.size()-1]].size(); i++)
        {
            v = G[u.second[u.second.size()-1]][i];
            if(dis[src][v.second]==-1 || (dis[src][u.second[u.second.size()-1]]+v.first)<dis[src][v.second])
            {
                dis[src][v.second] = dis[src][u.second[u.second.size()-1]] + v.first;
                vector<int> newpath = u.second;
                newpath.push_back(v.second);
                path[src][v.second] = newpath;
                v.first = -1*dis[src][v.second];
                q.push(make_pair(v.first,newpath));
            }
        }
    }
}

int main()
{
    int n, m;
    cin>>n>>m;
    for(int i=0; i<m; i++)
    {
        int u, v;
        double w;
        cin>>u>>v>>w;
        pair<double, int> p;
        p.first = w;
        p.second = v;
        G[u].push_back(p);
        p.second = u;
        G[v].push_back(p);
    }
    for(int i=1; i<=n; i++) dis[1][i] = -1;
    dijkstra(1);

    for(int i=1; i<=n; i++)
    {
        cout<<"PATH to: "<<i<<endl;
        for(int j=0; j<path[1][i].size(); j++)   cout<<path[1][i][j]<<' ';
        cout<<endl;
    }

    return 0;
}
