#include "dbcp.h"

dbcp::dbcp()
{
    //ctor
}


void dbcp::set_dc(double dc)
{
    this->dc = dc;
//    cout<<this->dc<<endl;
}

void dbcp::set_input(input Input, bool weighted)
{
    originalG = Input.get_graph();
    if(weighted)    Input.make_unweighted();
    unweightedG = Input.get_graph();
    G = Input.get_graph();
    n = Input.get_nodescnt();
    m = Input.get_edgecnt();
    avg_mindistohi = 0;
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;
    for(int i=1; i<=n; i++) dijkstra(i,1);
}

void dbcp:: refresh(bool weighted)
{
    if(weighted)    G = unweightedG;
    else    G = originalG;
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;
    for(int i=1; i<=n; i++) dijkstra(i,1);
}

int dbcp::finddensity(int node)
{
    int cnt = 0;
    for(int i=1; i<=n; i++)
        if(dis[node][i]!=-1 && dis[node][i]<dc)
            {cnt++; /*cout<<"DIS:  " <<node<<' '<<i<<' '<<dis[node][i]<<endl;*/}
    return cnt-1;
}

int dbcp::findmindistohi(int node)
{
    double d = 999999;
    for(int i=1; i<=n; i++)
    {
        if(density[node]<density[i])
        {
            if(d>dis[node][i])
            {
                d = dis[node][i];
                nearesthi[node] = i;
            }
            else if(d==dis[node][i] && density[i]>density[nearesthi[node]])
                nearesthi[node] = i;
        }
    }
    if(d == 999999)
    {
        d = 0;
        for(int i=1; i<=n; i++)
        {
            if(d<dis[node][i])
            {
                d = dis[node][i];
                nearesthi[node] = i;
            }
        }
    }
    return d;
}

void dbcp::dijkstra(int src, bool savepath)
{
    priority_queue <pair<double, pair<int, vector<int> > > > q;
    pair<double, pair<int, vector<int> > > u;
    pair <double, int> v;
    u.first = 0.0; u.second.second.push_back(src);
    u.second.first = src;
    q.push(u); dis[src][src] = 0.0;
    if(savepath)    path[src][src] = u.second.second;
    while(!q.empty())
    {
        u = q.top();
        q.pop();
        for(int i=0; i<G[u.second.first].size(); i++)
        {
            v = G[u.second.first][i];
            if(dis[src][v.second]==-1 || (dis[src][u.second.first]+v.first)<dis[src][v.second])
            {
                dis[src][v.second] = dis[src][u.second.first] + v.first;
                vector<int> newpath = u.second.second;
                newpath.push_back(v.second);
                if(savepath)    path[src][v.second] = newpath;
                v.first = -1*dis[src][v.second];
                q.push(make_pair(v.first,make_pair(v.second,newpath)));
            }
        }
    }
}

int dbcp::find_k()
{
    k = 0;
    for(int i=1; i<=n; i++) density[i] = finddensity(i);
    for(int i=1; i<=n; i++)
    {
        mindistohi[i] = findmindistohi(i);
        avg_mindistohi += mindistohi[i];
//        cout<<"MINDIS to HI: "<<i<<' '<<mindistohi[i]<<"  DENSITY: "<<density[i]<<endl;
    }
    avg_mindistohi /= (double)n;
//    cout<<"AVG MINDIS to HI: "<<avg_mindistohi<<endl;
    for(int i=1; i<=n; i++)
        if(mindistohi[i]>avg_mindistohi)    k++;
    return k;
}

bool dbcp::cluster(int k)
{
//    cout<<"IN CLUSTERING"<<endl;
    cluster_heads.clear();
    memset(clusters, -1, sizeof clusters);
    priority_queue <pair<double, int> > q;
    int cnt = 0;
    for(int i=1; i<=n; i++)
    {
        if(mindistohi[i]>avg_mindistohi)
        {
            clusters[i] = cnt++;
            this->cluster_heads.push_back(i);
        }
        else
        {
            pair <double, int> p;
            p.first = density[i];
            p.second = i;
            q.push(p);
        }
    }
    while(cnt<k)
    {
        int head = q.top().second; q.pop();
        clusters[head] = cnt++;
        this->cluster_heads.push_back(head);
    }
    int pleasestop = 0;
    while(!q.empty())
    {
        if(pleasestop>200)  return 0;
        pleasestop++;
        pair <double, int> p = q.top(); q.pop();
        int unassigned = p.second;
        if(clusters[nearesthi[unassigned]] == -1)   q.push(p);
        else    clusters[unassigned] = clusters[nearesthi[unassigned]];
//        cout<<clusters[unassigned]<<' '<<clusters[nearesthi[unassigned]]<<endl;
    }
//    cout<<"OUT OF CLUSTERING"<<endl;
    return 1;
}

void dbcp::clustermod(int k)
{
    cluster_heads.clear(); int cnt = 0;
    memset(clusters, -1, sizeof clusters);
    priority_queue <pair<double, int> > q;
    for(int i=1; i<=n; i++)
    {
        if(mindistohi[i]>avg_mindistohi)
        {
            pair<double, int> p;
            p.first = mindistohi[i];
            p.second = i;
            q.push(p);
        }
    }

    while(k-- > 0)
    {
        int head = q.top().second; q.pop();
        clusters[head] = cnt++;
        this->cluster_heads.push_back(head);
    }
    if(cnt==1)
    {
        for(int i=1; i<=n; i++)
        {
            if(i==cluster_heads[0]) continue;
            clusters[i]=0;
        }
    }
    while(!q.empty())    q.pop();
    for(int i=1; i<=n; i++)
    {
        if(clusters[i]==-1)
        {
            pair <double, int> p;
            p.first = density[i];
            p.second = i;
            q.push(p);
        }
    }
    int stop = 0;
    while(!q.empty())
    {
        pair <double, int> p = q.top(); q.pop();
        int unassigned = p.second;
        if(clusters[nearesthi[unassigned]] == -1)
        {
            p.first = (double)(--stop);
            q.push(p);
        }
        else    clusters[unassigned] = clusters[nearesthi[unassigned]];
        //cout<<unassigned<<' '<<clusters[unassigned]<<' '<<nearesthi[unassigned]<<' '<<clusters[nearesthi[unassigned]]<<endl;
    }
}

void dbcp::set_controllers(int k)
{
    controllers.clear();
    pia.clear(); pim.clear(); pii.clear();
    pair <double, int> p;
    p.first = infinity; p.second = 0;
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
            else    {piinter[i] += dis[i][j]; /*if(k==1)  cout<<"inter:  "<<dis[i][j]<<endl;*/}
        }
        piavg[i] /= cluster_size;
//        if(k==1)    cout<<"inter:  "<<piinter[i]<<endl;
        if( fabs(n-cluster_size)>0.00000001 )piinter[i] /= (double)(n-cluster_size);


        pilatency[i] = piavg[i] + pimax[i] + piinter[i];
//        if(k==1)    cout<<"LATENCY:   "<<pilatency[i]<<"("<<piavg[i]<<' '<<pimax[i]<<' '<<piinter[i]<<")  "<<controllers[cluster_id].first<<endl;
        if(pilatency[i]<controllers[cluster_id].first)
        {
            controllers[cluster_id].first = pilatency[i];
            controllers[cluster_id].second = i;
            pia[cluster_id] = piavg[i];
            pim[cluster_id] = pimax[i];
            pii[cluster_id] = piinter[i];
        }
//        cout<<"Cluster Number "<< cluster_id <<" node : "<< i  <<" latency: "<< pilatency[i]<<endl;
//        cout<<pimax[i]<<' '<<piavg[i]<<' '<<piinter[i]<<endl;
    }
//    cout<<"Controllers: "<<endl;
//    for(int i=0; i<k; i++)  cout<<controllers[i].second<<"("<<controllers[i].first<<' '<<cluster_heads[i]<<")  ";
//    cout<<endl;
//    if(k==1)    cout<<"Cluster Head: "<<controllers[0].second<<endl;
}

void dbcp::calculate_dis()
{
    G = originalG;
    for(int i=1; i<=n; i++)
        for(int j=1; j<=n; j++)
            dis[i][j] = -1;
    for(int i=1; i<=n; i++) dijkstra(i,0);
}

vector <double> dbcp::output()
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
            else    {piinter[cntrl] += dis[cntrl][j];/*cout<<"CLUSTER SIZE: "<<cluster_size<<endl;*/}
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

vector <double> dbcp::deterministic_search(int k, int limit, int iter)
{
    if(iter>limit)  return current_output;
    cout<<"DETERMINISTIC SEARCH:  "<<k<<' '<<iter<<endl;
    for(int i=1; i<=n; i++)
    {
        for(int j=0; j<G[i].size(); j++)
        {
            int adjnode = G[i][j].second;
            if(clusters[i]!=clusters[adjnode])
            {
                int prevclus = clusters[i];
                clusters[i]=clusters[adjnode];
                set_controllers(k);
                vector <double> v = output();
                if(current_output[0]>v[0])
                {
                    current_output = v;
                    v = deterministic_search(k, limit, iter+1);
                }
                clusters[i] = prevclus;
            }
        }
    }
    return current_output;
}

vector <double> dbcp::local_search(int limit, int iter, bool our)
{
    if(iter>limit)  return current_output;
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
        vector <double> v;
        if(our) v = myoutput();
        else    v = newlatency();
        cout<<"CHANGES:  "<<rnode<<"->"<<rcluster<<endl;
        if(v[0]<current_output[0])
        {
            set_output(v);
            cout<<"BETTER RESULTS\n";
            local_search(limit, iter+1, our);
            break;
        }
        else    clusters[rnode] = prevclus;
    }
    return current_output;
}

vector <double> dbcp::myoutput()
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

vector <double> dbcp::newlatency()
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
            //if(k==1 && !once)    cout<<"NODE PAIRS: "<<i<<' '<<j<<' '<<" LATENCY: "<<dis[vi][i] + maxlatency<<endl;
        }
    }
    latency /= (double)((n-1)*n);
//    if(k==1)   cout<<"LATENCY:  "<<latency<<endl;
    vector <double> ret;
    ret.push_back(latency);
    return ret;
}

void dbcp::set_output(vector <double> output)
{
    this->current_output = output;
}

dbcp::~dbcp()
{
    //dtor
}
