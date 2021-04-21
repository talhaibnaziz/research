#include <bits/stdc++.h>
using namespace std;

#define MAXVAL 9999999
#define debug cout<<"I AM HERE"<<endl;

int nodecnt, linkcnt, k;
double dc;
vector <pair<double, int> > graph[1000];
int density[1000];
double mindthdn[1000];
int nearesthdn[1000];
double dis[1000][1000];
vector <int> cluster_heads;
int clusters[1000], cluster_id[1000];
double avglat[1000], maxlat[1000], iclat[1000];
vector <pair<double, int> > controllers;
int parent[1000][1000];
double mlatency;
bool unweighted;
double sizes[1000];
double maxsize, minsize, avgsize;

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

void find_density()
{
    for(int i=1; i<=nodecnt; i++)
    {
        int cnt = 0;
        for(int j=1; j<=nodecnt; j++)
        {
            if(dis[i][j]<dc)    cnt++;
        }

        //subtracting the node itself from the count
        density[i] = cnt-1;
    }
}

void find_mindthdn()
{
    for(int i=1; i<=nodecnt; i++)
    {
        int maxnode, node;
        double maxdis=0, d=MAXVAL;
        for(int j=1; j<=nodecnt; j++)
        {
            if(dis[i][j]>maxdis)
            {
                maxdis = dis[i][j];
                maxnode = j;
            }
            if(density[j]>density[i] && dis[i][j]<d)
            {
                d = dis[i][j];
                node = j;
            }
            if(d!=MAXVAL && density[j]>density[node] && dis[i][j]==d)
            {
                d = dis[i][j];
                node = j;
            }
        }
        if(d == MAXVAL)
        {
            mindthdn[i]=maxdis;
            nearesthdn[i]=maxnode;
        }
        else
        {
            mindthdn[i]=d;
            nearesthdn[i]=node;
        }
    }
}

void find_k()
{
    k = 0;
    cluster_heads.clear();

    double avg_mindthdn = 0;
    for(int i=1; i<=nodecnt; i++)
        avg_mindthdn+=mindthdn[i];

    avg_mindthdn/=(double)nodecnt;

    for(int i=1; i<=nodecnt; i++)
    {
        if(mindthdn[i]>avg_mindthdn)
        {
            k++;
            cluster_heads.push_back(i);
            cluster_id[i]=k;
        }
    }
    cout<<"AVG DELTA: "<<avg_mindthdn<<endl;
}

int find_cluster(int node)
{
   if(clusters[node]==node) return node;
   return clusters[node] = find_cluster(clusters[node]);
}

void make_clusters()
{
    memset(clusters, -1, sizeof clusters);
    queue <int> q;
    for(int i=0; i<cluster_heads.size(); i++)
        clusters[cluster_heads[i]] = cluster_heads[i];
    for(int i=1; i<=nodecnt; i++)
        if(clusters[i]==-1) q.push(i);

    int testing[1000] = {0};
    while(!q.empty())
    {
        int node = q.front();
        q.pop();
        if(clusters[node]==-1 && clusters[nearesthdn[node]]!=-1)
            clusters[node] = clusters[nearesthdn[node]];
        else    q.push(node);
        testing[node]++;
    }

    for(int i=1; i<=nodecnt; i++)
    {
        clusters[i]=find_cluster(i);
        cluster_id[i] = cluster_id[clusters[i]];
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
        sizes[i] = cluster_size;
        if(cluster_size>1)  avglat[i]/=cluster_size-1;
        else    avglat[i]=0.0;
        if(cluster_size<nodecnt)    iclat[i]/=(double)nodecnt-cluster_size;
        else    iclat[i] = 0.0;
        int idx = cluster_id[i];
        if(controllers[idx].first>(avglat[i]+maxlat[i]+iclat[i]))
        {
            controllers[idx].first = avglat[i]+maxlat[i]+iclat[i];
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
            int vi = controllers[cluster_id[i]].second;
            int si = i;
            for(int ii=path.size()-1; ii>-1; ii--)
            {
                int sm = path[ii];
                int vm = controllers[cluster_id[sm]].second;
                pathcost = max(pathcost, dis[vi][vm]+dis[vm][sm]);
            }
            pathcost += dis[si][vi];
            path.clear();
            mlatency += pathcost;
        }
    }
    mlatency/=(double)((nodecnt)*(nodecnt-1)/2);
}

double find_diameter()
{
    int node1, node2;
    double dis1 = 0, dis2 = 0;
    for(int i=1; i<=nodecnt; i++)
    {
        if(dis1<dis[1][i])
        {
            dis1 = dis[1][i];
            node1 = i;
        }
    }
    for(int i=1; i<=nodecnt; i++)
    {
        if(dis2<dis[node1][i])
        {
            dis2 = dis[node1][i];
            node2 = i;
        }
    }
    return dis2;
}

double controllerSwitch()
{
    double result=0;
    for(int i=1; i<=k; i++)
    {
        int c = controllers[i].second;
        result += avglat[c];
    }
    return result/(double)k;
}

double interController()
{
    double result=0;
    for(int i=1; i<=k; i++)
    {
        int c1 = controllers[i].second;
        for(int j=1; j<=k; j++)
        {
            int c2 = controllers[j].second;
            if(c1==c2)  continue;
            result += dis[c1][c2];
        }
    }
    if(k>1) return result/(double)(k*(k-1));
    else    return 0.0;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie();
    cout.tie();

    ofstream fout, fout2, fout3;

    double total_lat=0.0;
    unweighted = 1;
    if(unweighted)  fout.open("Output/unweighted_dbcp.txt");
    else    fout.open("Output/weighted_dbcp.txt");

    if(unweighted)  fout2.open("Results/unweighted_dbcp_load.txt");
    else    fout2.open("Results/weighted_dbcp_load.txt");

    if(unweighted)  fout3.open("Results/unweighted_IntraInter.txt");
    else    fout3.open("Results/weighted_IntraInter.txt");

    for(int testcase=1; testcase<=7; testcase++)
    {
        //string file_name="Input/test_1.";
        string file_name="Input/";
        file_name.push_back(testcase+48);
        file_name.push_back('.');
        file_name.push_back('t');
        file_name.push_back('x');
        file_name.push_back('t');
        ifstream fin(file_name);

        double avg_k=0.0, avg_mlatency=0.0;
        double casecnt=10.0;
        maxsize = 0;
        minsize = 0;
        avgsize = 0;

        double ourIntra = 0;
        double ourInter = 0;

        for(int tc = 0; tc<10; tc++)
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

            for(int i=1; i<=nodecnt; i++)
                for(int j=1; j<=nodecnt; j++)
                    dis[i][j]=-1;

            for(int i=1; i<=nodecnt; i++) dijkstra(i);

            dc=0.3*find_diameter();
//            cout<<"diameter: "<<find_diameter()<<"   dc: "<<dc<<endl;

            find_density();

            find_mindthdn();
            cout<<"TESTCASE: "<<testcase<<endl;
//            if(testcase==3 && tc==8)  for(int i=1; i<=nodecnt; i++)   cout<<"NODE: "<<i<<"  Density: "<<density[i]<<"   Delta: "<<mindthdn[i]<<"  Nearest: "<<nearesthdn[i]<<endl;

            find_k();
            if(k>40)
            {
                cout<<"ERROR:  "<<endl;
                casecnt = casecnt-1;
                continue;
            }

            make_clusters();

            controller_selection();

            calculate_mlatency();
            avg_k += k;
            avg_mlatency += mlatency;

            double temporary1 = 0;
            double temporary2 = 0;
            double temporary3 = MAXVAL;

            for(int i=0; i<k; i++)
            {
                temporary1 += sizes[cluster_heads[i]];
                temporary2 = max(sizes[cluster_heads[i]], temporary2);
                temporary3 = min(sizes[cluster_heads[i]], temporary3);
            }
            temporary1/=(double)k;
            avgsize+=temporary1;
            maxsize+=temporary2;
            minsize+=temporary3;

            ourInter+=interController();
            ourIntra+=controllerSwitch();
        }
        avg_k/=casecnt;
        avg_mlatency/=casecnt;
        fin.close();
        fout<<nodecnt<<" "<<avg_k<<" "<<avg_mlatency<<endl;
        fout2<<minsize/10.0<<' '<<avgsize/10.0<<' '<<maxsize/10.0<<endl;
        fout3<<ourIntra/10.0<<' '<<ourInter/10.0<<endl;
        total_lat+=avg_mlatency;
        cout<<"edges: "<<linkcnt<<endl;
    }
//    fout<<total_lat/7.0<<endl;
    fout.close();

    return 0;
}
