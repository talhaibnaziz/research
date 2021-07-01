#include <bits/stdc++.h>
using namespace std;

vector <pair<double, int> > G[1000];
double dis[1000][1000];
int n, m; int density[1000];
double dc = 10.0; int k = 0;

void take_input()
{
    freopen("separated_input.txt", "r", stdin);
    cout<<"Take input: "<<endl;
    cin>>n>>m;
    int u, v; double w;
    for(int i=0; i<m; i++)
    {
        cin>>u>>v>>w;
        cout<<u<<' '<<v<<' '<<w<<endl;
        pair <double, int> p;
        p.first = w;
        p.second = v;
        G[u].push_back(p);
        p.second = u;
        G[v].push_back(p);
    }
}

void dijkstra(int src)
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
                v.first *= -1;
                q.push(v);
            }
        }
    }
}

void find_k()
{
    for(int i=1; i<=n; i++)
    {
        dijkstra(i);
        for(int j=1; j<=n; j++)
        {
            if(i==j)    continue;
            if(dis[i][j]<dc)    density[i]++;
        }
    }
}

int main()
{
    take_input();
    memset(density, 0, sizeof density);
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;

    return 0;
}
