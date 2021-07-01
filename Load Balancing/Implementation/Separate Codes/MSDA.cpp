#include <bits/stdc++.h>
using namespace std;

#define MAXVAL 1e+7
#define debug cout<<"HERE: "<<++DEBUG<<endl;

int DEBUG = 0;

int switchcnt, controllercnt, linkcnt;

//LOAD LIST of switches (global precedence)
vector < pair<double, int> > LL;

//PROCESSING POWER (maximum quota)
double PP[1000];

//RESPONSE TIME * HOP DISTANCE (which controller?)
//RESPONSE TIME = 1/(PROCESSING POWER - CONTROLLER LOAD)
double loads[1000];
vector <int> controllers;
vector < pair<double, int> > whichController[1000];

//SWITCH LOAD * HOP DISTANCE (which switch?)

//CONTROLLER LOAD = SUM of SWITCH LOAD

//MINIMUM RESOURCE UTILIZATION (minimum quota)
//RESOURCE UTILIZATION = CONTROLLER LOAD / PROCESSING POWER

//NODE to NODE distance
double dis[1000][1000];
bool vis[1000][1000];

//Network graph
vector <int> graph[1000];

//Clustered network
vector <int> network[1000];

void take_input()
{
    cout<<"Enter number of controllers:"<<endl;
    cin>>controllercnt;
    cout<<"Enter Controllers with Processing power and response time as follows:"<<endl;
    cout<<"CONTROLLER_ID    PROCESSING_POWER"<<endl;
    for(int i=0; i<controllercnt; i++)
    {
        int c_id;
        double propower;
        cin>>c_id>>propower;
        PP[c_id] = propower;
        controllers.push_back(c_id);
        loads[c_id] = 0.0;
    }

    cout<<"Enter number of switches:"<<endl;
    cin>>switchcnt;
    cout<<"Enter Switches with load and cluster id as follows"<<endl;
    cout<<"SWITCH_ID    LOAD    CONTROLLER_ID"<<endl;
    for(int i=0; i<switchcnt; i++)
    {
        int s_id, c_id;
        double load;
        cin>>s_id>>load>>c_id;
        loads[s_id] = load;
        loads[c_id] += load;
        LL.push_back(make_pair(load, s_id));
    }

    cout<<"Enter Number of links"<<endl;
    cin>>linkcnt;
    cout<<"Enter links: NODE_1 NODE_2"<<endl;
    for(int i=0; i<linkcnt; i++)
    {
        int u, v;
        cin>>u>>v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
}

void bfs(int src)
{
    queue <int> q;
    q.push(src);
    dis[src][src] = 0;
    vis[src][src] = 1;
    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        for(int i=0; i<graph[u].size(); i++)
        {
            int v = graph[u][i];
            if(!vis[src][v])
            {
                vis[src][v] = 1;
                dis[src][v] = dis[src][u] + 1;
                q.push(v);
            }
        }
    }
}

void find_distances()
{
    memset(vis, 0, sizeof vis);
    int nodecnt = switchcnt + controllercnt;
    for(int i=1; i<=nodecnt; i++)   bfs(i);

//    for(int i=1; i<=nodecnt; i++)
//    {
//        for(int j=1; j<=nodecnt; j++)
//        {
//            cout<<i<<'\t'<<j<<"\tDistance: "<<dis[i][j]<<endl;
//        }
//    }
}

void start_algorithm()
{
    sort(LL.begin(), LL.end());
    int remaining = switchcnt;
    int start_idx = switchcnt-1;
    int end_idx, minq[1000];
    int pref_idx[1000];
    memset(pref_idx, 0, sizeof pref_idx);
    priority_queue < pair<double, int> > clusters[1000];

//    for(int i=0; i<LL.size(); i++)  cout<<"global list: "<<LL[i].second<<endl;

    //CREATE "which controller?" LIST
    for(int i=0; i<switchcnt; i++)
    {
        int s_id = LL[i].second;
        for(int j=0; j<controllercnt; j++)
        {
            int c_id = controllers[j];
            double response_time = 1/(PP[c_id]-loads[c_id]);
            double preference = response_time * dis[c_id][s_id];
            whichController[s_id].push_back(make_pair(preference, c_id));
        }
        sort(whichController[s_id].begin(), whichController[s_id].end());
    }


    //MINIMUM QUOTA SELECTION and LOAD CALCULATION
    for(int i=0; i<controllercnt; i++)
    {
        int c_id = controllers[i];
        loads[c_id] = 0.0;
        minq[c_id] = 1;
    }

    while(remaining>0)
    {
        int minqsum = 0;
        for(int i=0; i<controllercnt; i++)
        {
            int c_id = controllers[i];
            minqsum += minq[c_id];
        }
        end_idx = minqsum;

        bool new_switch = true;
        int s_id;

//        cout<<"Limits: "<<start_idx<<' '<<end_idx<<" switches remaining: "<<remaining<<endl;

        //When R_(k-1) - R_k = null work with R_k
        if(start_idx==end_idx)  end_idx = -1;
        //Otherwise work with R_(k-1)

        //Standard DA
        while(start_idx>end_idx)
        {
            //Take switch id from Load List global preference
            if(new_switch)
            {
                s_id = LL[start_idx].second;
                remaining--;
            }
//            cout<<"Switch_IDX: "<<start_idx<<"  SWITCH: "<<s_id<<endl;

            //Take preferred controller from "which controller?"
            int idx = pref_idx[s_id];
            int c_id = whichController[s_id][idx].second;
//            cout<<"PREFERRED INDEX: "<<idx<<endl;
//            cout<<"CONTROLLER: "<<c_id<<endl;

            double preference = loads[s_id]*dis[c_id][s_id];
            clusters[c_id].push(make_pair(preference, s_id));
            loads[c_id] += loads[s_id];
            minq[c_id]--;
            if(minq[c_id]<0)    minq[c_id] = 0;
            if(loads[c_id]>PP[c_id])
            {
                //WORK WITH POPPED SWITCH
                s_id = clusters[c_id].top().second;
                new_switch = false;
                pref_idx[s_id]++;

                clusters[c_id].pop();
                loads[c_id] -= loads[s_id];

//                cout<<"A switch is popped"<<endl;
            }
            else
            {
                start_idx--;
                new_switch = true;
            }
        }
    }

    for(int i=0; i<controllercnt; i++)
    {
        int c_id = controllers[i];
        while(!clusters[c_id].empty())
        {
            int s_id = clusters[c_id].top().second;
            clusters[c_id].pop();
            network[c_id].push_back(s_id);
        }
    }
}

void print_network()
{
    cout<<"\n\nResults: \n";
    for(int i=0; i<controllercnt; i++)
    {
        int c_id = controllers[i];
        cout<<"Controller ID: "<<c_id<<" ->";
        for(int j=0; j<network[c_id].size(); j++)
        {
            int s_id = network[c_id][j];
            cout<<" "<<s_id;
        }
        cout<<endl;
    }
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(false); cout.tie(false);
    freopen("Input/test_1.txt", "r", stdin);
    take_input();
    find_distances();
    start_algorithm();
    print_network();

    return 0;
}
