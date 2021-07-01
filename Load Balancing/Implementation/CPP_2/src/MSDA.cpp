#include "MSDA.h"

MSDA::MSDA(int n, vector <edge> e, vector <int> controllers)
{
    //ctor

    this->nodecnt = n;
    int edge_cnt = e.size();
    this->c_cnt = controllers.size();

    for(int i=0; i<edge_cnt; i++)
    {
        int u = e[i].src;
        int v = e[i].des;
        ugraph[u].push_back(v);
        ugraph[v].push_back(u);
    }
    /**
    Calculate all possible pair shortest
    path distances in terms of hop-count
    */
    for(int i=0; i<nodecnt; i++)
        for(int j=0; j<nodecnt; j++)
            dis[i][j] = -1;

    for(int srcNode=0; srcNode<nodecnt; srcNode++)
    {
        queue <int> q;
        q.push(srcNode);
        dis[srcNode][srcNode] = 0.0;
        while(!q.empty())
        {
            int u = q.front();
            q.pop();
            for(int i=0; i<ugraph[u].size(); i++)
            {
                int v = ugraph[u][i];
                if(dis[srcNode][v] == -1)
                {
                    dis[srcNode][v] = dis[srcNode][u] + 1.0;
                    q.push(v);
                }
            }
        }
    }

    ///Store controller IDs and load
    for(int i=0; i<c_cnt; i++)
    {
        this->controllers.push_back(controllers[i]);
        c_loads[i] = 0.0;
    }
}

void MSDA::matching(double s_loads[MAX_NODE_COUNT], int clusters[MAX_NODE_COUNT])
{
    cout<<"MSDA Algorithm Executing..."<<endl;

    int cluster_id;
    /**Replicate previous matching and
    create the global precedence list*/
    for(int i=0; i<nodecnt; i++)
    {
        cluster_id = clusters[i];
        c_loads[cluster_id] += s_loads[i];
        LL.push_back(make_pair(s_loads[i], i));
    }

    sort(LL.begin(), LL.end());
    int remaining = nodecnt;
    int start_idx = nodecnt-1;
    int end_idx;
    int pref_idx[MAX_NODE_COUNT];
    for(int i=0; i<nodecnt; i++)    pref_idx[i] = 0;
    priority_queue < pair<double, int> > assignments[MAX_NODE_COUNT];

    ///Create preferred controller list
    for(int i=0; i<nodecnt; i++)
    {
        for(int j=0; j<c_cnt; j++)
        {
            int c_id = controllers[j];
            double response_time = 1.0/(PP - c_loads[j]);
            double preference = response_time * dis[c_id][i];
            whichController[i].push_back(make_pair(preference, j));
        }
        sort(whichController[i].begin(), whichController[i].end());
    }

    /**Minimum Quota selection and Load calculation:
    Just testing a minimum quota of one switch, the more
    this is increased the more difficult the matching*/
    for(int i=0; i<c_cnt; i++)
    {
        maxq[i] = MAX_PROCESSING_POWER;
        minq[i] = 1;
    }

    ///Standard Deferred Acceptance (DA) Algorithm
    while(remaining > 0)
    {
//        cout<<"Remaining Nodes: "<<remaining<<endl;
        int minqsum = 0;
        bool new_switch = true;
        int s_id;
        for(int i=0; i<c_cnt; i++)
            minqsum += minq[i];
        end_idx = minqsum-1;

        /**When only switches to fill minimum quota,
        is left, work with R_k (minimum quota switches)*/
        if(start_idx == end_idx)    end_idx = -1;
        ///Otherwise work with R_(k-1), previous min quota

        ///Standard Deferred Acceptance
        while(start_idx > end_idx)
        {
            ///Take switch from Load List(global preference)
            if(new_switch)
            {
                s_id = LL[start_idx].second;
                remaining--;
            }
//            cout<<"Current Switch: "<<s_id<<endl;

            /**Take preferred controller from switch's
            preference list (global precedence)*/
            int idx = pref_idx[s_id];
            cluster_id = whichController[s_id][idx].second;
            int c_id = controllers[cluster_id];
//            cout<<"Preferred Controller: "<<c_id<<endl;

            ///Assigns the switch to the controller
            double preference = c_loads[cluster_id]*dis[c_id][s_id];
            assignments[cluster_id].push(make_pair(preference, s_id));
            c_loads[cluster_id] += s_loads[s_id];
            minq[cluster_id] = max(0.0, minq[cluster_id]-1);

            ///If max quota already fulfilled
            if(c_loads[cluster_id] > maxq[cluster_id])
            {
                /**The popped switch may be the newly assigned
                switch or another low precedence switch*/
                s_id = assignments[cluster_id].top().second;
                new_switch = false;
                assignments[cluster_id].pop();
                c_loads[cluster_id] -= s_loads[s_id];
                ///Process next controller in preference
                pref_idx[s_id]++;
//                cout<<"QUOTA FILLED!"<<endl;
                if(pref_idx[s_id] >= c_cnt)   cout<<"WARNING: NO PREFERENCES LEFT"<<endl;
            }
            else
            {
//                cout<<"SUCCESSFULLY ASSIGNED!"<<endl;
                start_idx--;
                new_switch = true;
            }
        }
    }
    ///Cluster the network according to the assignment
    double check = 0.0, minmax_error = INT_MIN;
    for(int i=0; i<c_cnt; i++)
    {
        c_loads[i] = 0.0;
        while(!assignments[i].empty())
        {
            int s_id = assignments[i].top().second;
            assignments[i].pop();
            clusters[s_id] = i;
            c_loads[i] += s_loads[s_id];
//            cout<<s_id<<' ';
        }
//        cout<<endl;
//        cout<<c_loads[i]<<endl;
        check += c_loads[i];
    }
//    cout<<check<<endl;
//    for(int i=0; i<c_cnt; i++)
//        minmax_error = max(minmax_error, c_loads[i]-check/(double)c_cnt);
//    cout<<"Error: "<<minmax_error<<endl;
//    for(int i=0; i<nodecnt; i++)
//        cout<<"Node: "<<i<<"\tLoad: "<<s_loads[i]<<"\tCluster: "<<clusters[i]<<endl;
}

MSDA::~MSDA()
{
    //dtor
}
