#include <bits/stdc++.h>
using namespace std;

#define MAXVAL 9999999
#define debug cout<<"I AM HERE"<<endl;

int nodecnt, linkcnt, k;
double dk;
vector <pair<double, int> > graph[1000];
double dis[1000][1000];
double cost[1000][1000];
int parent[1000][1000];
int clusters[1000];
double mlatency;
double avglat[1000], maxlat[1000], iclat[1000];
vector <pair<double, int> > controllers;
vector <pair<int, int> > degreeList;
vector <double> cluster_sizes;

void take_input(int testcase)
{
//    string file_name="Input/test_1.";
    string file_name="Input/1";
    file_name.push_back(testcase+48);
    file_name.push_back('.');
    file_name.push_back('t');
    file_name.push_back('x');
    file_name.push_back('t');
    ifstream fin(file_name);

    for(int i=1; i<=nodecnt; i++)   graph[i].clear();

    fin>>nodecnt>>linkcnt;

    for(int i=1; i<=nodecnt; i++)
        for(int j=1; j<=nodecnt; j++)
            cost[i][j]=-1;

    int u, v; double w;
    dk = 0.0;
    for(int i=0; i<linkcnt; i++)
    {
        fin>>u>>v>>w;
//        w = 1;
        graph[u].push_back(make_pair(w,v));
        graph[v].push_back(make_pair(w,u));
        cost[u][v] = w;
        cost[v][u] = w;
        dk += w;
    }
    dk /= (double)linkcnt;
    fin.close();
}

void dijkstra(int node)
{
    dis[node][node] = 0.0;
    parent[node][node] = node;
    priority_queue <pair<double, int> > pq;
    pq.push(make_pair(0.0, node));
    while(!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        for(int i=0; i<graph[u].size(); i++)
        {
            int v = graph[u][i].second;
            double w = graph[u][i].first;
            if(dis[node][v]==-1 || (dis[node][u]+w<dis[node][v]))
            {
                dis[node][v]=dis[node][u]+w;
                pq.push(make_pair(-dis[node][v], v));
                parent[node][v]=u;
            }
        }
    }
}

int find_maxDegreeNode()
{
    int node = -1, maxDeg = -1;
    for(int i=1; i<=nodecnt; i++)
    {
        if(clusters[i]==-1)
        {
            int degree = 0;
            for(int j=0; j<graph[i].size(); j++)
            {
                int v = graph[i][j].second;
                if(clusters[v]==-1) degree++;
            }
            if(degree>maxDeg)
            {
                maxDeg = degree;
                node = i;
            }
        }
    }
//    cout<<"Max Degree: "<<maxDeg<<"  node: "<<node<<endl;
    return node;
}

double support(int node, vector <int> SetOfNodes)
{
    double sup = 0.0;
    for(int i=0; i<SetOfNodes.size(); i++)
    {
        int v = SetOfNodes[i];
        //cout<<v<<' '<<cost[node][v]<<endl;
        if(cost[node][v]!=-1)
        {
            sup+=cost[node][v];
        }
    }
    //cout<<"node: "<<node<<"  support: "<<sup<<endl;
    return sup;
}

double density(vector <int> SetOfNodes)
{
    double den = 0.0;
    for(int i=0; i<SetOfNodes.size(); i++)
    {
        int u = SetOfNodes[i];
        for(int j=i+1; j<SetOfNodes.size(); j++)
        {
            int v = SetOfNodes[j];
            if(cost[u][v]!=-1)  den+=cost[u][v];
        }
    }
//    int s = SetOfNodes.size();
//    if(s>1) den /= (double)(s*(s-1)/2);
    return den;
}

int expand(int node)
{
    vector <int> S;
    S.push_back(node);

    //Collecting the Candidate nodes
    set <int> CandidateList;
    for(int i=0; i<graph[node].size(); i++)
    {
        int v = graph[node][i].second;
        if(clusters[v]==-1) CandidateList.insert(v);
    }
    set <int>::iterator idx;

    //Selecting cluster members iteratively
    double maxsup; int candidate;
    while(!CandidateList.empty())
    {
        //Find candidate with maximum support
        maxsup = 0.0;
        for(idx=CandidateList.begin(); idx!=CandidateList.end(); idx++)
        {
            int v = *idx;
            double sup = support(v,S);
            if(sup>maxsup)
            {
                maxsup = sup;
                candidate = v;
            }
        }
        //Check condition to include into cluster
//        cout<<"For candidate "<<candidate<<"   Support: "<<support(candidate,S)<<"  Density: "<<density(S)<<endl;
        if(support(candidate,S)>=density(S))
        {
            S.push_back(candidate);
            clusters[candidate] = k;
            CandidateList.erase(candidate);
            for(int i=0; i<graph[candidate].size(); i++)
            {
                int v = graph[candidate][i].second;
                if(clusters[v]==-1) CandidateList.insert(v);
            }
//            cout<<"Candidate Success: "<<candidate<<endl;
        }
        else    break;
    }

    //Probabilistic step
//    cout<<"INSIDE SET NOW"<<endl;
    if(!CandidateList.empty())
    {
//        cout<<"Size: "<<CandidateList.size()<<endl;
        for(idx=CandidateList.begin(); idx!=CandidateList.end(); idx++)
        {
            int u = *idx;
//            cout<<"Candidate: "<<u<<endl;
            int degree = 0;
            for(int i=0; i<graph[u].size(); i++)
            {
                int v = graph[u][i].second;
                if(clusters[v]==-1) degree++;
            }
            if(degree*nodecnt<2.0*linkcnt*1.2)
            {
                S.push_back(u);
                clusters[u] = k;
//                cout<<"Prob Success: "<<u<<endl;
            }
        }
    }

    return S.size()-1;
}

void pgca()
{
    k = 0;

    for(int i=1; i<=nodecnt; i++)
        for(int j=1; j<=nodecnt; j++)
            cost[i][j] = 1.0/cost[i][j];

    int stopcnt = 0;
    while(stopcnt<nodecnt)
    {
        stopcnt++; k++;
        int u = find_maxDegreeNode();
        clusters[u] = k;
        stopcnt += expand(u);
//        cout<<"Nodes Clustered: "<<stopcnt<<"   K: "<<k<<endl;
    }
}

void controller_selection()
{
    controllers.clear();
    for(int i=0; i<=k; i++)  controllers.push_back(make_pair(MAXVAL,-1));
    for(int i=1; i<=nodecnt; i++)
    {
        avglat[i] = 0;
        maxlat[i] = 0;
        iclat[i] = 0;
        double cluster_size = 0;
        for(int j=1; j<=nodecnt; j++)
        {
            if(clusters[i]==clusters[j])
            {
                avglat[i]+=dis[i][j];
                maxlat[i] = max(maxlat[i], dis[i][j]);
                cluster_size = cluster_size + 1;
            }
            else    iclat[i]+=dis[i][j];
        }
        avglat[i]/=cluster_size-1;
        iclat[i]/=(double)nodecnt-cluster_size;
        int idx = clusters[i];
        if(controllers[idx].first>(avglat[i]+maxlat[i]+iclat[i]))
        {
            controllers[idx].first = avglat[i]+maxlat[i]+iclat[i];
            controllers[idx].second = i;
        }
    }
}

void post_processing()
{
    for(int i=0; i<=k; i++)
        cluster_sizes.push_back(0.0);

    for(int i=1; i<=nodecnt; i++)
        cluster_sizes[clusters[i]]++;

//    int tk = k;
//    for(int i=tk; i>0; i--)
//    {
//        if(cluster_sizes[i]<4)
//        {
//            int S = cluster_sizes[i];
//            cout<<i<<' '<<S<<endl;
//            while(S--)
//            {
//                int node, nearest;
//                double d=MAXVAL;
//                for(int j=1; j<=nodecnt; j++)
//                    if(clusters[j]==i)  node = j;
//                for(int j=1; j<=nodecnt; j++)
//                {
//                    if(clusters[j]=clusters[node] && dis[node][j]<d)
//                    {
//                        d = dis[node][j];
//                        nearest = j;
//                    }
//                }
//                clusters[node]=clusters[nearest];
//                cluster_sizes[clusters[nearest]]++;
//            }
//            k--;
//        }
//    }
}

void calculate_mlatency()
{
    mlatency = 0;
    vector <int> path;
    for(int i=1; i<=nodecnt; i++)
    {
        for(int j=i+1; j<=nodecnt; j++)
        {
            int node = j;
            while(node!=i)
            {
                path.push_back(node);
                node = parent[i][node];
            }
            path.push_back(node);
            double pathcost = 0;
            int vi = controllers[clusters[i]].second;
            int si = i;
            for(int ii=path.size()-2; ii>-1; ii--)
            {
                int sm = path[ii];
                int vm = controllers[clusters[sm]].second;
                pathcost = max(pathcost, dis[vi][vm]+dis[vm][sm]);
            }
            pathcost += dis[si][vi];
            path.clear();
            mlatency += pathcost;
        }
    }
    mlatency/=(double)((nodecnt)*(nodecnt-1)/2);
}

void local_search()
{
    double old_mlatency=mlatency;
    vector <int> S[1000];
    bool checked[1000][1000];
    memset(checked, 0, sizeof checked);

    for(int i=1; i<=nodecnt; i++)
        S[clusters[i]].push_back(i);

    int counter = 0;
    while(counter++<(nodecnt*(nodecnt-1)/2))
    {
        int cluster1 = rand()%k+1;
        int cluster2 = rand()%k+1;
        if(cluster1==cluster2)
        {
            counter--;
            continue;
        }
        int node1 = S[cluster1][rand()%S[cluster1].size()];
        int node2 = S[cluster2][rand()%S[cluster2].size()];
        if(checked[node1][node2])
        {
            counter--;
            continue;
        }
        clusters[node1] = cluster2;
        clusters[node2] = cluster1;
        controller_selection();
        calculate_mlatency();
        if(mlatency<old_mlatency)
        {
            counter=0;
            memset(checked, 0, sizeof checked);
            old_mlatency = mlatency;
//            cout<<"TRIAL no. "<<counter<<"  Change: "<<node1<<' '<<node2<<"  Old: "<<old_mlatency<<"  New: "<<mlatency<<endl;
        }
        else
        {
            counter++;
            clusters[node1] = cluster1;
            clusters[node2] = cluster2;
            mlatency = old_mlatency;
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie();
    cout.tie();

//    ofstream fout("Output/new_weighted_local_pgca.txt");
    ofstream fout("Output/new_weighted_pgca.txt");
//    ofstream fout("Output/unweighted_pgca.txt");
//    ofstream fout("Output/unweighted_local_pgca.txt");

    for(int testcase=1; testcase<8; testcase++)
    {
        take_input(testcase);
        for(int i=1; i<=nodecnt; i++)
            for(int j=1; j<=nodecnt; j++)
                dis[i][j]=-1;

        for(int i=1; i<=nodecnt; i++) dijkstra(i);

        memset(clusters, -1, sizeof clusters);
        pgca();

        post_processing();

        controller_selection();

        calculate_mlatency();

//        local_search();

        fout<<nodecnt<<" "<<k<<" "<<mlatency<<endl;
//        for(int i=1; i<=nodecnt; i++)   cout<<"Node: "<<i<<"  cluster: "<<clusters[i]<<endl;
//        for(int i=1; i<=k; i++) cout<<' '<<controllers[i].second;
        for(int i=1; i<=k; i++) cout<<' '<<cluster_sizes[i];
        cout<<endl;
    }

    return 0;
}
