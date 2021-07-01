#include <bits/stdc++.h>
using namespace std;

int n, m;
vector <pair<int,double> > graph[1000];
vector <pair<double,int> > G[1000];
int clusters[1000];
double cost[1000][1000];
int cluster_cnt;
double Ts=0.5, Td=0.0;
int cluster_head[1000];
double dis[1000][1000];

void take_input()
{
    memset(cost, 0, sizeof cost);
    cout<<"Enter no of nodes and vertices:"<<endl;
    cin>>n>>m;
    cout<<"Enter connecting edges:"<<endl;
    double max_cost = 0;
    for(int i=0; i<m; i++)
    {
        int u, v;
        double w;
        cin>>u>>v>>w;
        pair <int, double> p;
        p.first = v; p.second = w;
        graph[u].push_back(p);
        p.first = u;
        graph[v].push_back(p);
        cost[u][v] = w; cost[v][u] = w;
        pair <double, int> q;
        q.first = w;
        q.second = v;
        G[u].push_back(q);
        q.second = u;
        G[v].push_back(q);
        max_cost = max(max_cost, w);
    }
    if(max_cost>1)
    {
        for(int i=1; i<=n; i++)
        {
            for(int j=0; j<graph[i].size(); j++)
            {
                graph[i][j].second/=max_cost;
                cost[i][graph[i][j].first]/=max_cost;
            }
        }
    }
}

pair <int, double> find_max_wdeg()
{
    int node=-1; double max_wdeg = -1;
    for(int i=1; i<=n; i++)
    {
        if(clusters[i]==-1)
        {
            double wdeg = 0;
            for(int j=0; j<graph[i].size(); j++)
            {
                int nxtnode = graph[i][j].first;
                if(clusters[nxtnode]!=-1)    continue;
                wdeg += graph[i][j].second;
            }
            if(wdeg>max_wdeg)
            {
                max_wdeg = wdeg;
                node = i;
            }
        }
    }
    pair <int, double> ret(node,max_wdeg);
    return ret;
}

double density(vector <int> S, int addnode)
{
    if(addnode!=-1) S.push_back(addnode);
    double ret = 0;
    double s = S.size();
    for(int i=0; i<S.size()-1; i++)
        for(int j=i+1; j<S.size(); j++)
            ret += cost[S[i]][S[j]];
    if(s>1) ret = ret / (s*(s-1)/2.0);
    return ret;
}

double support(int t, vector <int> S)
{
    double ret = 0;
    for(int i=0; i<S.size(); i++)
        ret += cost[S[i]][t];
    return ret;
}

void SPICi()
{
    int cnt = 0;
    //while all nodes not clustered
    while(cnt<n)
    {
        //find max weighted degree of unclustered nodes
        pair <int,double> p = find_max_wdeg();
        int u = p.first;
        cout<<"max weighted unclustered node: ";
        cout<<u<<'('<<p.second<<')'<<endl;
        cluster_head[u] = u;

        cluster_cnt++; double cluster_size = 1.0;
        clusters[u] = cluster_cnt; cnt++;
        vector <int> S; S.push_back(u);
        set <int> Candidates;
        set <int> :: iterator itr;
        //insert adjacent unclustered nodes into set Candidates
        for(int j=0; j<graph[u].size(); j++)
        {
            int nxtnode = graph[u][j].first;
            if(clusters[nxtnode]!=-1)   continue;
            Candidates.insert(nxtnode);
        }
        while(!Candidates.empty())
        {
            //Find Candidate with maximum support value
            int t;
            double max_sup = -1;
            for(itr=Candidates.begin(); itr!=Candidates.end(); ++itr)
            {
                int node = *itr;
                double sup = support(node, S);
                cout<<"Candidates are: "<<node<<"=>"<<sup<<endl;
                if(sup>max_sup)
                {
                    max_sup = sup;
                    t = node;
                }
            }
            //insert into Set of Candidates or stop expanding cluster
            cout<<"Working with Candidate: ";
            cout<<t<<'('<<max_sup<<')'<<endl;
            //very inefficient but to debug
            cout<<"condition values: c_size=>"<<cluster_size<<" density=>"<<density(S,-1)<<' '<<density(S,t)<<endl;
            if(max_sup>=Ts*cluster_size*density(S,-1) && density(S,t)>Td)
            {
                S.push_back(t);
                cluster_size++; cnt++;
                clusters[t] = cluster_cnt;
                cluster_head[t] = u;
                Candidates.erase(t);
                for(int j=0; j<graph[t].size(); j++)
                {
                    int nxtnode = graph[t][j].first;
                    if(clusters[nxtnode]!=-1)   continue;
                    Candidates.insert(nxtnode);
                }
                cout<<"inserted into cluster: "<<t<<endl;
            }
            else    break;
        }
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
                v.first = -1*dis[src][v.second];
                q.push(v);
            }
        }
    }
}

void print_data()
{
    int k = cluster_cnt;
    cout<<"clusters: "<<k<<endl;
    double maxdis = -1, ccdis = 0, avg = 0;
    int pressure = 0;
    vector <int> heads;
    for(int i=1; i<=n; i++)
        if(cluster_head[i]==i)  heads.push_back(i);
    for(int i=0; i<heads.size(); i++)
    {
        int cluster_size = 1;
        for(int j=1; j<=n; j++)
        {
            if(j==heads[i]) continue;
            if(cluster_head[j]==heads[i])
            {
                maxdis = max(maxdis, dis[heads[i]][j]);
                cluster_size++; avg += dis[heads[i]][j];
            }
        }
        pressure = max(pressure, cluster_size);
    }
    avg /= (double)(n-k);
    for(int i=0; i<heads.size()-1; i++)
        for(int j=i+1; j<heads.size(); j++)
            if(i!=j)    ccdis = ccdis + dis[heads[i]][heads[j]];
    ccdis /= (double)(k*(k-1))/2.0;
    cout<<"max distance to node: "<<maxdis<<endl;
    cout<<"avg distance to node: "<<avg<<endl;
    cout<<"cluster to cluster distance: "<<ccdis<<endl;
    cout<<"Final metric: "<<maxdis+avg+ccdis<<endl;
    cout<<"max nodes in a cluster: "<<pressure<<endl;
}

int main()
{
    cluster_cnt = 0;
    memset(clusters, -1, sizeof clusters);
    memset(cluster_head, 0, sizeof cluster_head);
    memset(dis, 0, sizeof dis);
    take_input();
    SPICi();

    //For evaluation
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;
    for(int i=1; i<=n; i++) dijkstra(i);
    print_data();

    return 0;
}
