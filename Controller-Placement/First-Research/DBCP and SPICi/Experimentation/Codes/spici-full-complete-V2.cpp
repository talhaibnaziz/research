//Taking the node with less number of edge as the first first seed. Everything else is like Spici full complete.
#include<bits/stdc++.h>
using namespace std;

#define FastRead        ios_base::sync_with_stdio(0);cin.tie(0);
#define fread           freopen("input.txt","r",stdin);
#define fwrite          freopen("output.txt","w",stdout);
#define nl              "\n"
#define ll              long long
#define ull             unsigned long long
#define mk              make_pair
#define pb              push_back
#define sz(a)           (int)a.size()
#define pii             pair<int,int>
#define pLL             pair<ll,ll>
#define mem(a)          memset(a,-1,sizeof a)
#define clr(a)          memset(a,0,sizeof a)
#define all(a)          a.begin(),a.end()
#define UNIQUE(a)       sort(all(a)),a.erase(unique(all(a)),a.end())
#define SORT(v)         sort(v.begin(),v.end())
#define REV(v)          reverse(v.begin(),v.end())
#define ff              first
#define ss              second
#define sq(a)           ((a)*(a))
#define For(i,a,b)      for(int i=a;i<=b;i++)
#define Rof(i,a,b)      for(int i=a;i>=b;i--)
#define Rep(i,b)        for(int i=0;i<b;i++)
#define MOD             1000000007
#define PI              acos(-1.0)
#define eps             1e-9
#define Linf            2e18
#define inf             1<<30
#define MX4             10004
#define MX5             100005
#define MX6             1000006

int dx[] = {-1,+0,+1,+0,+1,+1,-1,-1,+0};
int dy[] = {+0,-1,+0,+1,+1,-1,+1,-1,+0};
//..........................................GEO................................
#define isRight(r,a,b)  ((b.y-r.y)*(a.x-r.x)<(b.x-r.x)*(a.y-r.y))
#define dot(r,a,b)      ((a.x-r.x)*(b.x-r.x)+(a.y-r.y)*(b.y-r.y))
#define ABSDIS(a,r)     (sqrt(sq(a.x-r.x)+sq(a.y-r.y)))
#define angle(r,a,b)    ((180.0*(acos(dot(r,a,b)/(ABSDIS(a,r)*ABSDIS(b,r)))))/PI)
template <typename T> inline T SqrDis(T x1, T y1, T x2, T y2){return sq(x1 - x2) + sq(y1 - y2);}
template <typename T> inline T Area2(T Ax, T Ay, T Bx, T By, T Cx, T Cy){T ret = Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By);
    if(ret < 0) return ret = -ret;return ret;
}
//struct coord{double x,y;bool operator==(coord a){return (x==a.x && y==a.y);}};
//..........................................NT.................................
template <typename T> inline T bigmod(T A,T B,T M = MOD){T ret = 1;while(B){if(B & 1) ret = (ret * A) % M;A = (A * A) % M;B = B >> 1;}return ret;}
template <typename T> inline T invmod (T A,T M = MOD){return BigMod(A,M-2,M);}

//.........................................END.................................

int n, m;
vector <pair<int,double> > graph[1000];
int clusters[1000];
double cost[1000][1000];
int cluster_cnt;
double Ts=0.5, Td=0.0;
int cluster_head[1001];
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
    }

}

pair <int, double> find_max_wdeg()
{
    int node=-1; double max_wdeg = 9999999;
    for(int i=1; i<=n; i++)
    {
        if(clusters[i]==-1)
        {
            double wdeg = 0;
            for(int j=0; j<graph[i].size(); j++)
            {
                int nxtnode = graph[i][j].first;
                if(clusters[nxtnode]!=-1)    continue;
                wdeg ++;
            }
            if(wdeg<max_wdeg)
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
        //cout<<"max weighted unclustered node: ";
        cout<<u<<'('<<p.second<<')'<<endl;

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
                //cout<<"Candidates are: "<<node<<"=>"<<sup<<endl;
                if(sup>max_sup)
                {
                    max_sup = sup;
                    t = node;
                }
            }
            //insert into Set of Candidates or stop expanding cluster
           // cout<<"Working with Candidate: ";
           // cout<<t<<'('<<max_sup<<')'<<endl;
            //very inefficient but to debug
            //cout<<"condition values: c_size=>"<<cluster_size<<" density=>"<<density(S,-1)<<' '<<density(S,t)<<endl;
            if(max_sup>=Ts*cluster_size*density(S,-1) && density(S,t)>Td)
            {
                S.push_back(t);
                cluster_size++; cnt++;
                clusters[t] = cluster_cnt;
                Candidates.erase(t);
                for(int j=0; j<graph[t].size(); j++)
                {
                    int nxtnode = graph[t][j].first;
                    if(clusters[nxtnode]!=-1)   continue;
                    Candidates.insert(nxtnode);
                }
                //cout<<"inserted into cluster: "<<t<<endl;
            }
            else    break;
        }
    }
}
int cluster_minus;
void print()
{
    cout<<"Cluster Count: "<<cluster_cnt-cluster_minus<<endl;
    cout<<"Nodes and their Clusters:"<<endl;
    map<int,int> mp;
    map<int,int> :: iterator it;
    for(int i=1; i<=n; i++){
        cout<<i<<'('<<clusters[i]<<')'<<endl;
        mp[clusters[i]]++;
    }



    cout<<"Number of nodes in a cluster: "<<endl;
    for(it=mp.begin();it!=mp.end();it++){
        cout<< it->first <<" : "<< it->second <<nl;
    }
}
set<int> bad;
int dijakstra(int node){
    priority_queue< pair<double,int> > pq;
    pq.push(mk(0,node));
    double vis[MX4];
    Rep(i,MX4) vis[i]=9876543422.0;
    vis[node]=0;
    while(!pq.empty()){
        pair<double,int> temp=pq.top();
        pq.pop();
        double weight=temp.ff;
        int u= temp.ss;
        Rep(i,sz(graph[u])){
            int v=graph[u][i].ff;
            int cost=weight+graph[u][i].ss;
            if(cost>vis[v]) continue;
            if(bad.find(clusters[v]) ==bad.end() ){
                return clusters[v];
            }
            vis[v]=cost;
            pq.push(mk(cost,v));
        }
    }

}

void controller_head_set(){
    map<int,int> mp;
    map<int,int> :: iterator it;
    For(i,1,n) mp[clusters[i]]++;
    cout<<"Cluster Heads: "<<endl;
    for(it=mp.begin();it!=mp.end();it++){
        int no=it->first;
        int a= it->second;
        vector<int> cluster_set;
        For(i,1,n){
            if(clusters[i]==no) cluster_set.pb(i);

        }
        double maxv=-12132.0;
        int head_node;
        For(i,1,n){
            if(clusters[i]==no){
                double value=support(i,cluster_set);
                if(value>maxv){
                    maxv=value;
                    head_node=i;

                }
            }

        }
        cluster_head[no]=head_node;
        cout<<no <<" = " <<head_node<<endl;


    }


}
void post_process(){
    int minimum_cluster_threshold=1;
    map<int,int> mp;
    map<int,int> :: iterator it;
    cluster_minus=0;
    For(i,1,n) mp[clusters[i]]++;
    for(it=mp.begin();it!=mp.end();it++){
        int no=it->first;
        int a= it->second;
        if(a<=minimum_cluster_threshold){
            bad.insert(no);
        }

    }
    for(it=mp.begin();it!=mp.end();it++){
        int no=it->first;
        int a= it->second;
        if(a<=minimum_cluster_threshold){
            For(j,1,n){
                if(clusters[j]==no){
                    int v=dijakstra(j);
                    clusters[j]=v;
                    mp[v]++;
                }
            }
            cluster_minus++;
        }

    }

}
void clusters_head_distance(){

    set<int> st;
    set<int> :: iterator it,it1;

    for(int i=1;i<=n;i++){
        if(cluster_head[i]>0)
        {
            cout<<"CLUSTER HEADS:  "<<cluster_head[i]<<endl;
            st.insert(cluster_head[i]);
        }
    }
    cout<<"Cluster head to head distance: "<<endl;
    double love=-9999999, love_avg=0;
    for(it=st.begin();it!=st.end();it++){
        int pr=*it;
        priority_queue< pair<double,int> > Q;
        double vis[MX4];
        Rep(i,MX4) vis[i]=99999999.0;
        vis[pr]=0;
        Q.push(make_pair(0,pr));
        while(!Q.empty()){
            int u=Q.top().ss;
            double w=Q.top().ff;
            Q.pop();
            Rep(i,sz(graph[u])){
                int v=graph[u][i].ff;
                double cost=graph[u][i].ss+w;
                if(vis[v]<cost) continue;
                vis[v]=cost;
                Q.push(mk(cost,v));
            }

        }
        for(int i=1;i<=n;i++){
            if(cluster_head[i]>0) {
                cout<<vis[ cluster_head[i]]<<" ";
                love=max(love, vis[ cluster_head[i]]);
                love_avg += vis[ cluster_head[i]];
            }

        }
        cout<<endl;
    }
    love_avg /= 2.0;
    double k = cluster_cnt - cluster_minus;
    love_avg /= (double)(k*(k-1))/2.0;
    cout<<"maximum head to head distance is : "<<love<<endl;
    cout<<"average head to head distance is : "<<love_avg<<" "<<k<<endl;
}
void cluster_diameter(){
    set< pair<int, int> > st;
    set< pair<int, int> > :: iterator it,it1;
    double avg_diameter = 0;

    for(int i=1;i<=n;i++){
        if(cluster_head[i]>0) st.insert( mk(cluster_head[i],i));
    }
    cout<<"Cluster diameter: "<<endl;
    for(it=st.begin();it!=st.end();it++){
        pair<int,int> temp=*it;
        int pr=temp.ff;
        priority_queue< pair<double,int> > Q;
        double vis[MX4];
        Rep(i,MX4) vis[i]=99999999.0;
        vis[pr]=0;
        Q.push(make_pair(0,pr));
        while(!Q.empty()){
            int u=Q.top().ss;
            double w=Q.top().ff;
            //cout<<u <<" ----"<<  w<<endl;
            Q.pop();
            Rep(i,sz(graph[u])){
                int v=graph[u][i].ff;
                if(clusters[v]!=clusters[pr]) continue;
                double cost=graph[u][i].ss+w;
                if(vis[v]<cost) continue;
                vis[v]=cost;
                Q.push(mk(cost,v));
            }

        }
        double maxv=-98812321.2;
        int nn;
        for(int i=1;i<=n;i++){
            if(vis[i]>maxv && vis[i]<100000){
                maxv=vis[i];
                nn=i;
            }
            if(vis[i]<100000)   avg_diameter += vis[i];
        }
        //cout<<"nn:   "<<nn<<"  "<<maxv<<endl;
        /*
        Rep(i,MX4) vis[i]=99999999.0;
        vis[nn]=0;
        Q.push(make_pair(0,nn));
        while(!Q.empty()){
            int u=Q.top().ss;
            double w=Q.top().ff;
            Q.pop();
            Rep(i,sz(graph[u])){
                int v=graph[u][i].ff;
                if(clusters[v]!=clusters[pr]) continue;
                double cost=graph[u][i].ss+w;
                if(vis[v]<cost) continue;
                vis[v]=cost;
                Q.push(mk(cost,v));
            }

        }

        maxv=-98812321.2;
        for(int i=1;i<=n;i++){
            if(vis[i]>maxv && vis[i]<100000){
                maxv=vis[i];
                nn=i;
            }
        }
        */
        cout<< temp.ss <<" - > "<<maxv<<endl;
    }
    double k = cluster_cnt - cluster_minus;
    avg_diameter /= n-cluster_cnt;
    cout<<"Average diameter:  "<<avg_diameter<<" "<<k<<endl;
}
int main()
{
    cluster_cnt = 0;
    memset(clusters, -1, sizeof clusters);
    take_input();
    SPICi();
    //post_process();
    controller_head_set();
    clusters_head_distance();
    cluster_diameter();
    print();
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
