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

int second_seed(int u)
{
    vector <int> bin[5];
    for(int i=0; i<graph[u].size(); i++)
    {
        int nxtnode = graph[u][i].first;
        if(clusters[nxtnode]!=-1)   continue;
        double cost = graph[u][i].second;
        int idx = ceil((cost*5.0)-1.0);
        cout<<"bin creation: "<<cost<<"->"<<idx<<endl;
        bin[idx].push_back(nxtnode);
    }

    int s_seed; double max_wdeg = -1;
    for(int i=4; i>-1; i--)
    {
        for(int j=0; j<bin[i].size(); j++)
        {
            int node = bin[i][j];
            double wdeg = 0;
            for(int k = 0; k<graph[node].size(); k++)
            {
                int nxtnode = graph[node][k].first;
                if(clusters[nxtnode]!=-1)   continue;
                wdeg += graph[node][k].second;
            }
            if(wdeg>max_wdeg)
            {
                max_wdeg = wdeg;
                s_seed = node;
            }
        }
        if(max_wdeg>-1) return s_seed;
    }
    return -1;
}

double density(vector <int> S, int addnode)
{
    if(addnode!=-1) S.push_back(addnode);
    double ret = 0;
    double s = S.size();
    for(int i=0; i<S.size()-1; i++)
        for(int j=i+1; j<S.size(); j++)
            ret += cost[S[i]][S[j]];
    ret = ret / (s*(s-1)/2.0);
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
        int v = second_seed(u);
        cluster_head[u] = u;

        //no second seed
        if(v==-1)
        {
            cluster_cnt++;
            clusters[u] = cluster_cnt;
            cnt++;
            cout<<"NO SECOND SEED"<<endl;
        }
        //Expand cluster
        else
        {
            cout<<"Second Seed: "<<v<<endl;
            cluster_cnt++; double cluster_size = 2.0;
            clusters[u] = cluster_cnt;
            clusters[v] = cluster_cnt;
            cluster_head[v] = u;
            cnt += 2;
            vector <int> S; S.push_back(u); S.push_back(v);
            set <int> Candidates;
            set <int> :: iterator itr;
            //insert adjacent unclustered nodes into set Candidates
            for(int i=0; i<2; i++)
            {
                int node = S[i];
                for(int j=0; j<graph[node].size(); j++)
                {
                    int nxtnode = graph[node][j].first;
                    if(clusters[nxtnode]!=-1)   continue;
                    Candidates.insert(nxtnode);
                }
            }
            bool stop_clustering = 0;
            while(!Candidates.empty())
            {
                if(stop_clustering) break;
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
                if(max_sup>=Ts*cluster_size*density(S,-1) && density(S,t)>Td)
                {
                    S.push_back(t);
                    cluster_size++; cnt++;
                    clusters[t] = cluster_cnt;
                    cluster_head[t] = u;
                    for(int j=0; j<graph[t].size(); j++)
                    {
                        int nxtnode = graph[t][j].first;
                        if(clusters[nxtnode]!=-1)   continue;
                        Candidates.insert(nxtnode);
                    }
                }
                else    stop_clustering = 1;
                break;
            }
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


/*
graph input
10 11
1 2 1
1 3 1
1 9 1
1 10 0.4
2 4 1
2 3 1
1 6 0.6
6 5 0.5
6 7 1
6 8 1
7 8 0.4
*/
