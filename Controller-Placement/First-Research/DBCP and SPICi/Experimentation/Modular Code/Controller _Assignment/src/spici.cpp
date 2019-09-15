#include "spici.h"

spici::spici()
{
    //ctor
}

void spici::set_ts(double ts)
{
    this->ts = ts;
}

void spici::set_td(double td)
{
    this->td = td;
}

void spici::set_myc(int myc)
{
    this->myc = myc;
}

void spici::set_input(input Input, bool weighted)
{
    G = Input.get_graph();
    graph = Input.get_graph();
    n = Input.get_nodescnt();
    m = Input.get_edgecnt();
    memset(cost, 0, sizeof cost);
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;
    //cout<<"GRAPH:   "<<n<<' '<<m<<endl;
    for(int i=1; i<=n; i++)
    {
        //cout<<"SIZE:  "<<i<<' '<<graph[i].size()<<endl;
        for(int j=0; j<graph[i].size(); j++)
        {
            if(weighted)    graph[i][j].first = 1.0/graph[i][j].first;
            int v = graph[i][j].second;
            double w = graph[i][j].first;
            cost[i][v] = w;
            //cout<<i<<' '<<v<<' '<<w<<endl;
        }
    }
    for(int i=1; i<=n; i++) dijkstra(i);
}

void spici::dijkstra(int src)
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


pair <int, double> spici::find_max_wdeg()
{
    int node=-1; double max_wdeg = -1;
    for(int i=1; i<=n; i++)
    {
        if(clusters[i]==-1)
        {
            double wdeg = 0;
            for(int j=0; j<graph[i].size(); j++)
            {
                int nxtnode = graph[i][j].second;
                if(clusters[nxtnode]!=-1)    continue;
                wdeg += graph[i][j].first;
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

int spici::second_seed(int u)
{
    vector <int> bin[5];
    for(int i=0; i<graph[u].size(); i++)
    {
        int nxtnode = graph[u][i].second;
        if(clusters[nxtnode]!=-1)   continue;
        double cost = graph[u][i].first;
        int idx = ceil((cost*5.0)-1.0);
        //cout<<"bin creation: "<<cost<<"->"<<idx<<endl;
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
                int nxtnode = graph[node][k].second;
                if(clusters[nxtnode]!=-1)   continue;
                wdeg += graph[node][k].first;
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

double spici::density(vector <int> S, int addnode)
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

double spici::support(int t, vector <int> S)
{
    double ret = 0;
    for(int i=0; i<S.size(); i++)
        ret += cost[S[i]][t];
    return ret;
}

void spici::cluster()
{
    int cnt = 0; cluster_cnt = 0;
    //while all nodes not clustered
    memset(clusters,-1,sizeof clusters);
    memset(cluster_head, 0, sizeof cluster_head);
    while(cnt<n)
    {
        //find max weighted degree of unclustered nodes
        pair <int,double> p = find_max_wdeg();
        int u = p.first;
//        cout<<"max weighted unclustered node: ";
        cout<<u<<'('<<p.second<<')'<<endl;
        if(p.second == 0)
        {
            int newhead; double distance = infinity;
            for(int i=0; i<graph[u].size(); i++)
            {
                int adj = graph[u][i].second;
                int head = cluster_head[adj];
                if(dis[u][head]<distance)
                {
                    distance = dis[u][head];
                    cluster_head[u]=head;
                    clusters[u]=clusters[head];
                }
            }
            cnt++;
            continue;
        }
        int v = second_seed(u);
        cluster_head[u] = u;

        //no second seed
        if(v==-1)
        {
            cluster_cnt++;
            clusters[u] = cluster_cnt-1;
            cnt++;
//            cout<<"NO SECOND SEED"<<endl;
        }
        //Expand cluster
        else
        {
//            cout<<"Second Seed: "<<v<<endl;
            cluster_cnt++; double cluster_size = 2.0;
            clusters[u] = cluster_cnt-1;
            clusters[v] = cluster_cnt-1;
            cluster_head[v] = u;
            cnt += 2;
            vector <int> S; S.push_back(u); S.push_back(v);
            set <int> Candidates;
            set <int> :: iterator itr, todel;
            //insert adjacent unclustered nodes into set Candidates
            for(int i=0; i<2; i++)
            {
                int node = S[i];
                for(int j=0; j<graph[node].size(); j++)
                {
                    int nxtnode = graph[node][j].second;
                    if(clusters[nxtnode]!=-1)   continue;
                    Candidates.insert(nxtnode);
                }
            }
            bool stop_clustering = 0;
            while(!Candidates.empty())
            {
                if(stop_clustering)
                {
                    for(itr=Candidates.begin(); itr!=Candidates.end(); ++itr)
                    {
                        int node = *itr;
                        //cout<<"CHECK to ADD:  "<<node<<' '<<deg[node]<<endl;
                        if(deg[node]<=tg)
                        {
                            clusters[node]=cluster_cnt-1;
                            cluster_head[node] = u;
                            cnt++;
                            //cout<<"ADD IT to OTHERS: "<<node<<' '<<deg[node]<<' '<<tg<<endl;
                        }
                    }
                    break;
                }
                //Find Candidate with maximum support value
                int t;
                double max_sup = -1;
                for(itr=Candidates.begin(); itr!=Candidates.end(); ++itr)
                {
                    int node = *itr;
                    double sup = support(node, S);
//                    cout<<"Candidates are: "<<node<<"=>"<<sup<<endl;
                    if(sup>max_sup)
                    {
                        max_sup = sup;
                        t = node;
                        todel = itr;
                    }
                }
                //insert into Set of Candidates or stop expanding cluster
//                cout<<"Working with Candidate: ";
                cout<<t<<'('<<max_sup<<')'<<endl;
                if(max_sup>=ts*cluster_size*density(S,-1) && density(S,t)>td)
                {
                    S.push_back(t);
                    cluster_size++; cnt++;
                    clusters[t] = cluster_cnt-1;
                    cluster_head[t] = u;
                    Candidates.erase(todel);
                    for(int j=0; j<graph[t].size(); j++)
                    {
                        int nxtnode = graph[t][j].second;
                        if(clusters[nxtnode]!=-1)   continue;
                        Candidates.insert(nxtnode);
                    }
                }
                else    stop_clustering = 1;
            }
        }
    }
}

void spici::find_degree()
{
    degree.clear();
    for(int i=1; i<=n; i++)
    {
        double d = (double)graph[i].size();
        pair <double, int> p;
        p.first = d;
        p.second = i;
        degree.push_back(p);
        deg[i] = d;
        cout<<i<<' '<<deg[i]<<endl;
    }
    cout<<n<<endl;
    sort(degree.begin(), degree.end());
    int idx = (myc*n)/5;
//    cout<<"Degrees limit:   "<<degree[idx].first<<endl;
    tg = degree[idx].first;
}

void spici::set_controllers()
{
    controllers.clear();
    pia.clear(); pim.clear(); pii.clear();
    pair <double, int> p;
    p.first = infinity; p.second = 0;
    k = cluster_cnt;
    for(int i=0; i<k; i++)
    {
        controllers.push_back(p);
        pia.push_back(infinity);
        pim.push_back(infinity);
        pii.push_back(infinity);
    }
    for(int i=1; i<=n; i++)
    {
        piavg[i]=0, pimax[i]=0, piinter[i]=0;
        double cluster_size = 0.0;
        int cluster_id = clusters[i];
        for(int j=1; j<=n; j++)
        {
            if(clusters[i]==clusters[j])
            {
                piavg[i] += dis[i][j];
                pimax[i] = max(pimax[i], dis[i][j]);
                cluster_size = cluster_size + 1.0;
            }
            else    piinter[i] += dis[i][j];
        }
        piavg[i] /= cluster_size;
        piinter[i] /= (double)(n-cluster_size);
        pilatency[i] = piavg[i] + pimax[i] + piinter[i];
        cout<<"CLUSTER ID: "<<cluster_id<<' '<<controllers[cluster_id].first<<endl;
        if(pilatency[i]<controllers[cluster_id].first)
        {
            controllers[cluster_id].first = pilatency[i];
            controllers[cluster_id].second = i;
            pia[cluster_id] = piavg[i];
            pim[cluster_id] = pimax[i];
            pii[cluster_id] = piinter[i];
        }
    }
    cout<<"Controllers: "<<endl;
    for(int i=0; i<k; i++)  cout<<controllers[i].second<<"("<<controllers[i].first<<")  ";
    cout<<endl;
}

vector <double> spici::output()
{
    for(int i=0; i<controllers.size(); i++)
    {
        int cntrl = controllers[i].second;
        piavg[cntrl]=0, pimax[cntrl]=0, piinter[cntrl]=0;
        double cluster_size = 0.0;
        int cluster_id = clusters[cntrl];
        for(int j=1; j<=n; j++)
        {
            if(cluster_id==clusters[j])
            {
                piavg[cntrl] += dis[cntrl][j];
                pimax[cntrl] = max(pimax[cntrl], dis[cntrl][j]);
                cluster_size = cluster_size + 1.0;
            }
            else    piinter[cntrl] += dis[cntrl][j];
        }
        piavg[cntrl] /= cluster_size;
        piinter[cntrl] /= (double)n;
        pilatency[cntrl] = piavg[cntrl] + pimax[cntrl] + piinter[cntrl];
    }

//    cout<<"HEADS:  ";
//    for(int i=0; i<cluster_heads.size(); i++)   cout<<cluster_heads[i]<<' ';
//    cout<<endl;
    double avgpi = 0, avgpia = 0, avgpim = 0, avgpii = 0;
    int k = controllers.size();
    for(int i=0; i<k; i++)
    {
        int head = controllers[i].second;
        avgpi += pilatency[head];
        avgpia += piavg[head];
        avgpim += pimax[head];
        avgpii += piinter[head];
//        avgpi += controllers[i].first;
//        avgpia += pia[i];
//        avgpim += pim[i];
//        avgpii += pii[i];
    }

//    avgpi /= (double)k;
//    avgpia /= (double)k;
//    avgpim /= (double)k;
//    avgpii /= (double)k;

    vector <double> ret;
    ret.push_back(avgpi);
    ret.push_back(avgpia);
    ret.push_back(avgpim);
    ret.push_back(avgpii);

    return ret;
}

vector <double> spici::myoutput()
{
    int k = controllers.size();
    double avgdissum=0, avgctocsum=0;
    for(int i=0; i<k; i++)
    {
        for(int j=1; j<=n; j++)
        {
            int head = controllers[i].second;
            if(clusters[head]==clusters[j]) avgdissum += dis[head][j];
        }
        for(int j=0; j<k; j++)
        {
            if(i==j)    continue;
            int a = controllers[i].second;
            int b = controllers[j].second;
            avgctocsum += dis[a][b];
        }
    }

    avgdissum /= (double)k;
    avgctocsum /= (double)(2*k);

    vector <double> ret;
    ret.push_back(avgdissum);
    ret.push_back(avgctocsum);

    return ret;
}


double spici::newlatency()
{
    double latency = 0;
    for(int i=1; i<n; i++)
    {
        int cluster_idi = clusters[i];
        int vi = controllers[cluster_idi].second;
        for(int j=i+1; j<=n; j++)
        {
            vector<int> pathij = path[i][j];
            double maxlatency = -1;
            for(int ii=1; ii<pathij.size(); ii++)
            {
                int sm = pathij[ii];
                int cluster_idii = clusters[sm];
                int vm = controllers[cluster_idii].second;
                maxlatency = max(maxlatency,dis[vi][vm] + dis[vm][sm]);
            }
            latency += dis[vi][i] + maxlatency;
        }
    }
    latency /= (double)((n-1)*n);
    return latency;
}

double spici::local_search(int limit, int iter)
{
    if(iter>limit)  return latency;
    srand(time(NULL));
    int cnt = 0; memset(vis, 0, sizeof vis);
    while(1)
    {
        int rcluster = rand();
        rcluster %= k;
        int rnode = rand();
        rnode = (rnode%n)+1;
        if(vis[rnode][rcluster])    continue;
        else    cnt++;
        if(cnt>=n*k) break;
        vis[rnode][rcluster] = 1;
        if(clusters[rnode]==rcluster)    continue;
        int prevclus = clusters[rnode];
        clusters[rnode] = rcluster;
        double v = newlatency();
//        cout<<"CHANGES:  "<<rnode<<"->"<<rcluster<<endl;
        if(v<latency)
        {
            latency = v;
//            cout<<"BETTER RESULTS\n";
            local_search(limit, iter+1);
            break;
        }
        else    clusters[rnode] = prevclus;
    }
    return latency;
}

spici::~spici()
{
    //dtor
}
