#include <bits/stdc++.h>
using namespace std;

#define MAXVAL 9999999
#define eps 1e-10
#define debug cout<<"I AM HERE"<<endl;

int nodecnt, linkcnt, k;
vector <pair<double, int> > graph[1000];
double dis[1000][1000];
int parent[1000][1000];
vector <pair<double, int> > controllers;
double avglat[1000], iclat[1000];
vector <pair <double, int> > cluster_heads;
int clusters[1000];
int cluster_size[1000];
vector <pair<double, int> > nodeList;
double mlatency;
double avg_degree;
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

void bccp()
{
    double threshold = (double)nodecnt/(double)k;
//    cout<<"THRESHOLD: "<<threshold<<endl;
    int cnt = 0;
    int idx = 0;
    cluster_heads.clear();

    /*THRESHOLD RADIUS DETERMINATION*/
    double limit = 1+avg_degree;
    double boundary = avg_degree;
    double Td = 1.0;
    while(limit<threshold)
    {
        boundary*=(avg_degree-1);
        limit+=boundary;
        Td+=1;
    }
    Td-=1;

    /*Cluster head selection*/
    while(idx<nodecnt && cnt<k)
    {
        int node = nodeList[idx].second;
        double deg = nodeList[idx].first;
        if(idx==0)
        {
            cluster_heads.push_back(make_pair(deg, node));
            cnt++; idx++;
            clusters[node] = cnt;
            continue;
        }
        bool inside_cluster = 0;
        for(int i=0; i<cluster_heads.size(); i++)
        {
            int nex_node = cluster_heads[i].second;
            int nex_deg = cluster_heads[i].first;
            if(dis[nex_node][node]<Td)
            {
//                cout<<"NODE: "<<node<<"  HEAD: "<<nex_node<<"  DIS: "<<dis[nex_node][node]<<"  LIMIT: "<<threshold/nex_deg<<endl;
                inside_cluster = 1;
            }
        }
        if(inside_cluster)
        {
            idx++;
            continue;
        }
        cnt++; idx++;
        clusters[node] = cnt;
        cluster_heads.push_back(make_pair(deg, node));
    }
    if(cnt<k)   cout<<"KI HOCHCHE"<<endl;

    /*CLUSTERING THE REMAINING SWITCHES*/
    memset(cluster_size, 0, sizeof cluster_size);
    for(int i=1; i<=nodecnt; i++)
    {
        int node;
        double d = MAXVAL;
        for(int j=0; j<cluster_heads.size(); j++)
        {
            int head = cluster_heads[j].second;
            if(dis[head][i]<d)
            {
                d = dis[head][i];
                node = head;
            }
        }
        clusters[i] = clusters[node];
        cluster_size[clusters[i]]++;
    }

    /*Handling border nodes*/
    /*for(int i=1; i<=nodecnt; i++)
    {
        int clash = 0;
        double d = MAXVAL;
        for(int j=0; j<cluster_heads.size(); j++)
        {
            int head = cluster_heads[j].second;
            if(dis[head][i]<d)
            {
                d = dis[head][i];
                clash = 0;
            }
            else if(abs(dis[head][i]-d)<eps)    clash++;
        }
        if(clash)
        {
            for(int j=0; j<cluster_heads.size(); j++)
            {
                int head = cluster_heads[j].second;
                if(abs(dis[head][i]-d)<eps && cluster_size[clusters[head]]<cluster_size[clusters[i]])
                {
                    cluster_size[clusters[i]]--;
                    cluster_size[clusters[head]]++;
                    clusters[i] = clusters[head];
                }
            }
        }
    }*/
}

void controller_selection()
{
    controllers.clear();
    for(int i=0; i<=k; i++)  controllers.push_back(make_pair(MAXVAL,0));
    for(int i=1; i<=nodecnt; i++)
    {
        avglat[i] = 0;
        iclat[i] = 0;
        double cs = cluster_size[clusters[i]];
        for(int j=1; j<=nodecnt; j++)
        {
            if(clusters[i]==clusters[j])    avglat[i]+=dis[i][j];
            else    iclat[i]+=dis[i][j];
        }

        if(cs>1)  avglat[i]/=cs-1;
        else    avglat[i] = 0.0;
        if(cs<nodecnt)    iclat[i]/=(double)nodecnt-cs;
        else    iclat[i] = 0.0;
        int idx = clusters[i];
        if(controllers[idx].first>(avglat[i]+iclat[i]))
        {
            controllers[idx].first = avglat[i]+iclat[i];
            controllers[idx].second = i;
        }
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
            for(int ii=path.size()-1; ii>-1; ii--)
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

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie();
    cout.tie();

    unweighted = 1;
    ofstream fout;
    double total_lat = 0.0;

//    if(unweighted) fout.open("Output/avg_unweighted_bccp.txt");
//    else    fout.open("Output/avg_weighted_bccp.txt");

    int wks[8] = {0,8,7,10,45,41,36,53};
    int uks[8] = {0,3,4,3,4,4,4,4};

    for(int testcase=1; testcase<=1; testcase++)
    {
//        string file_name="Input/";
//        string file_name="Input/test_1.";
//        file_name.push_back(testcase+48);
//        file_name.push_back('.');
//        file_name.push_back('t');
//        file_name.push_back('x');
//        file_name.push_back('t');
//        ifstream fin(file_name);

        double avg_k=0.0, avg_mlatency=0.0;

        for(int tc=0; tc<1; tc++)
        {
            for(int i=1; i<=nodecnt; i++)   graph[i].clear();

            cin>>nodecnt>>linkcnt;
            int u, v; double w;
            for(int i=0; i<linkcnt; i++)
            {
                cin>>u>>v>>w;
                if(unweighted)  w = 1;
                graph[u].push_back(make_pair(w,v));
                graph[v].push_back(make_pair(w,u));
            }

//            cout<<"TESTCASE: "<<testcase<<endl;

            for(int i=1; i<=nodecnt; i++)
                for(int j=1; j<=nodecnt; j++)
                    dis[i][j]=-1;

            nodeList.clear();
            for(int i=1; i<=nodecnt; i++)
            {
                dijkstra(i);
                nodeList.push_back(make_pair(graph[i].size(), i));
                avg_degree += graph[i].size();
            }
            avg_degree/=(double)nodecnt;
            sort(nodeList.begin(), nodeList.end());
            reverse(nodeList.begin(), nodeList.end());

//            k = uks[testcase];
            k = 2;
            bccp();

            controller_selection();

            calculate_mlatency();

            cout<<mlatency<<endl;

            cout<<"CLUSTER HEADS:";
            for(int i=0; i<cluster_heads.size(); i++)   cout<<' '<<cluster_heads[i].second;
            cout<<endl;
            for(int i=1; i<=nodecnt; i++)   cout<<"NODE: "<<i<<"   CLUSTERS: "<<clusters[i]<<endl;
            cout<<"CONTROLLERS:";
            for(int i=1; i<controllers.size(); i++) cout<<' '<<controllers[i].second;
            cout<<endl;
//            cout<<"MLATENCY: "<<mlatency<<endl;
//            avg_mlatency += mlatency;
//            avg_k += k;
        }
//        fout<<nodecnt<<' '<<avg_k/10.0<<' '<<avg_mlatency/10.0<<endl;
//        total_lat+=avg_mlatency/10.0;
//        fin.close();
    }
//    fout<<total_lat/7.0<<endl;
//    fout.close();

    return 0;
}

/*
5 6
1 2 1
1 3 1
1 4 1
2 5 1
3 5 1
4 5 1
*/
