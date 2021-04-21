//Iterative Search Based Clustering

#include <bits/stdc++.h>
using namespace std;

#define MAXVAL 9999999
#define debug cout<<"I AM HERE"<<endl;

int nodecnt, linkcnt, k;
vector <pair<double, int> > graph[1000];
double dis[1000][1000];
int parent[1000][1000];
vector <pair<double, int> > controllers;
double avglat[1000], maxlat[1000], iclat[1000];
vector <int> cluster_heads;
pair <int, int> degreeList[1000];
double mlatency;
int clusters[1000];
bool test = 0;
bool unweighted;

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

void controller_selection()
{
    controllers.clear();
    for(int i=0; i<=k; i++)  controllers.push_back(make_pair(MAXVAL,0));
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
        if(cluster_size>1)  avglat[i]/=cluster_size-1;
        else    avglat[i] = 0.0;
        if(cluster_size<nodecnt)    iclat[i]/=(double)nodecnt-cluster_size;
        else    iclat[i] = 0.0;
        int idx = clusters[i];
        if(controllers[idx].first>(avglat[i]+maxlat[i]+iclat[i]))
        {
            controllers[idx].first = avglat[i]+maxlat[i]+iclat[i];
            controllers[idx].second = i;
        }
        if(test && clusters[i]==2)  cout<<"selection: "<<i<<"  values: "<<avglat[i]<<' '<<maxlat[i]<<' '<<iclat[i]<<endl;
    }
}

void calculate_mlatency()
{
    mlatency = 0;
    vector <int> path;
    for(int i=1; i<nodecnt; i++)
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
            if(test && i==29 && j==34)  cout<<"node: "<<i<<"  cluster: "<<clusters[i]<<"  controller: "<<vi<<endl;
            if(test && i==29 && j==34)  cout<<"node: "<<j<<"  cluster: "<<clusters[j]<<endl;
            for(int ii=path.size()-1; ii>-1; ii--)
            {
                int sm = path[ii];
                int vm = controllers[clusters[sm]].second;
                pathcost = max(pathcost, dis[vi][vm]+dis[vm][sm]);
            }
            pathcost += dis[si][vi];
            path.clear();
            mlatency += pathcost;
//            if(test)    cout<<"PATHCOST from "<<i<<" to "<<j<<": "<<pathcost<<endl;
        }
    }
    mlatency/=(double)((nodecnt)*(nodecnt-1)/2);
}

void Calibrate()
{
    controller_selection();
    calculate_mlatency();
    double old_latency = mlatency;
    int stopcnt = 0;
    bool checked[1000][1000]={0};
    int cluster_sizes[1000]={0};
    for(int i=1; i<=nodecnt; i++)
        cluster_sizes[clusters[i]]++;
//    srand(time(nullptr));
    while(++stopcnt*2<nodecnt*k)
    {
        int node1 = rand()%nodecnt + 1;
        int cluster1 = rand()%k + 1;
        //cout<<node1<<' '<<cluster1<<endl;
        if(checked[node1][cluster1])
        {
            stopcnt--;
            continue;
        }
        checked[node1][cluster1] = 1;
        if(clusters[node1]==cluster1 || cluster_sizes[clusters[node1]]==1)
            continue;
        int prev_cluster = clusters[node1];
        clusters[node1] = cluster1;
        controller_selection();
        calculate_mlatency();
        if(mlatency<old_latency)
        {
            old_latency = mlatency;
            stopcnt = 0;
            memset(checked, 0, sizeof checked);
            cluster_sizes[cluster1]++;
            cluster_sizes[prev_cluster]--;
            continue;
        }
        clusters[node1] = prev_cluster;
    }
}

void isbc()
{
    memset(clusters, -1, sizeof clusters);
    cluster_heads.clear();

    for(int i=1; i<=nodecnt; i++)
    {
        degreeList[i].first = graph[i].size();
        degreeList[i].second = i;
    }
    sort(degreeList+1, degreeList + nodecnt + 1);
    for(int i=1; i<=k; i++)
    {
        int node = degreeList[nodecnt-i+1].second;
        clusters[node]=i;
        cluster_heads.push_back(node);
    }
    for(int i=1; i<=nodecnt; i++)
    {
        if(clusters[i]==-1)
        {
            double mindis = MAXVAL;
            int node;
            for(int j=0; j<k; j++)
            {
                if(mindis>dis[i][cluster_heads[j]])
                {
                    mindis = dis[i][cluster_heads[j]];
                    node = cluster_heads[j];
                }
            }
            clusters[i] = clusters[node];
        }
    }
//    int cluster_sizes[1000]={0};
//    for(int i=1; i<=nodecnt; i++)   cout<<"Node: "<<i<<' '<<"  cluster: "<<clusters[i]<<endl;
//    for(int i=1; i<=nodecnt; i++)   cluster_sizes[clusters[i]]++;
//    for(int i=1; i<=k; i++) cout<<"CLUSTER SIZES: "<<cluster_sizes[i]<<endl;
    Calibrate();
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie();
    cout.tie();

    unweighted = 0;
    ofstream fout;

    if(unweighted) fout.open("Output/avg_unweighted_isbc.txt");
    else    fout.open("Output/avg_weighted_isbc.txt");

//    if(unweighted) fout.open("Output/forced_unweighted_isbc.txt");
//    else    fout.open("Output/forced_weighted_isbc.txt");

    int wks[8] = {0,8,7,10,45,41,36,53};
    int uks[8] = {0,3,4,3,4,4,4,4};

    for(int testcase=1; testcase<=7; testcase++)
    {
        string file_name="Input/";
        file_name.push_back(testcase+48);
        file_name.push_back('.');
        file_name.push_back('t');
        file_name.push_back('x');
        file_name.push_back('t');
        ifstream fin(file_name);

        double avg_k=0.0, avg_mlatency=0.0;

        for(int tc=0; tc<10; tc++)
        {
            for(int i=1; i<=nodecnt; i++)   graph[i].clear();

            fin>>nodecnt>>linkcnt;
            int u, v; double w;
            for(int i=0; i<linkcnt; i++)
            {
                fin>>u>>v>>w;
                if(unweighted)  w = 1;
                graph[u].push_back(make_pair(w,v));
                graph[v].push_back(make_pair(w,u));
            }

            cout<<"TESTCASE: "<<testcase<<endl;

            for(int i=1; i<=nodecnt; i++)
                for(int j=1; j<=nodecnt; j++)
                    dis[i][j]=-1;

            for(int i=1; i<=nodecnt; i++) dijkstra(i);
    //        for(int i=1; i<=nodecnt; i++)
    //            for(int j=1; j<=nodecnt; j++)
    //                cout<<"DIS from "<<i<<" to "<<j<<": "<<dis[i][j]<<endl;

            int limita = .05*(double)nodecnt;
            int limitb = .15*(double)nodecnt;
            int fixedlimit = .10*(double)nodecnt;
            if(limita==1)   limita++;
            cout<<limita<<' '<<limitb<<' '<<fixedlimit<<endl;
            int oldk;
            double oldmlatency;
            for(int i=limita; i<=limitb; i++)
            {
    //            k = ks[testcase];
//                k = wks[testcase];
                k = i;
                isbc();
                cout<<"RESULTS: "<<k<<' '<<mlatency<<endl;
                if(limita==i)
                {
                    oldk = k;
                    oldmlatency = mlatency;
                    continue;
                }
                double rat = (oldmlatency-mlatency)/oldmlatency;
                rat = rat/double(k-oldk);
                cout<<"RATIO: "<<rat<<endl;
                if(mlatency<oldmlatency && rat>0.1)
                {
                    oldmlatency = mlatency;
                    oldk = k;
                }
            }

    //        test = 1;
    //        controller_selection();

            avg_k += k;
            avg_mlatency += mlatency;
    //        cout<<"CONTROLLERS:";
    //        for(int i=1; i<=k; i++)   cout<<' '<<controllers[i].second<<" ("<<controllers[i].first<<") ";
    //        cout<<endl;
    //        for(int i=1; i<=nodecnt; i++)   cout<<"Node: "<<i<<' '<<"  cluster: "<<clusters[i]<<endl;
        }
        avg_k/=10.0;
        avg_mlatency/=10.0;
        fout<<nodecnt<<" "<<avg_k<<" "<<avg_mlatency<<endl;
        fin.close();
    }

    return 0;
}
