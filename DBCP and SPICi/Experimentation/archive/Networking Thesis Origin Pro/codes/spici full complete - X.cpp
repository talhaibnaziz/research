
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
double fcost[1000][1000];
double cluster_radius[1000];
int cluster_cnt;
double Ts=0.4, Td=.25;
int cluster_head[1001];
void take_input()
{
    memset(cost, 0, sizeof cost);
    for(int i=0;i<1000;i++) for(int j=0;j<1000;j++) {
        fcost[i][j]=double(inf);
        if(i==j) fcost[i][j]=0.0;
    }
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
        fcost[u][v] = w; fcost[v][u] = w;

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
    memset(clusters, -1, sizeof clusters);
    int cnt = 0;
    //while all nodes not clustered
    cluster_cnt=0;
    while(cnt<n)
    {
        //find max weighted degree of unclustered nodes
        pair <int,double> p = find_max_wdeg();
        int u = p.first;
        //cout<<"max weighted unclustered node: ";
        //cout<<u<<'('<<p.second<<')'<<endl;

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

void controller_head_set(){

    //cout<<"Cluster Heads: "<<endl;
    for(int ii=1;ii<=cluster_cnt;ii++){
        vector<int> cluster_set;
        For(i,1,n){
            if(clusters[i]==ii) cluster_set.pb(i);

        }
        double maxv=-12132.0;
        int head_node;
        for(int ii=0;ii<cluster_set.size();ii++){
            int i=cluster_set[ii];
            double value=support(i,cluster_set);
            if(value>maxv){
                maxv=value;
                head_node=i;

            }
        }
        cluster_head[ii]=head_node;
    }


}
void clusters_head_distance(){

    double love=-9999999;
    for(int i=1;i<=cluster_cnt;i++){
        for(int j=1;j<=cluster_cnt;j++){
            int ii= cluster_head[i];
            int jj=cluster_head[j];
            love=max(love, fcost[ii][jj]);
        }
    }
    //cout<<"maximum head to head distance is : "<<love<<endl;

}
void cluster_diameter(){

    double maxmaxv=-1;
    for(int i=1;i<=cluster_cnt;i++){
        int u=cluster_head[i];
        double maxv=-1232.0;
        for(int j=1;j<=n;j++){
            if(clusters[j]==i) maxv=max(maxv,fcost[u][j]);
        }
        cluster_radius[i]= maxv;
        maxmaxv=max(maxmaxv,maxv);

        //cout<<i<<" -- "<<maxv<<endl;
    }
    //cout<<"Highest Cluster radius: "<<maxmaxv<<endl;

}
void floyd_warshall(){
    for(int k=1;k<=n;k++){
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++ ){
                if(fcost[i][k]+fcost[k][j]<fcost[i][j]){
                    fcost[i][j]=fcost[i][k]+fcost[k][j];
                }
            }
        }
    }
}
vector<pair<int,double > > latency_k;
void what_we_want(){

    map<int,int> mp;
    map<int,int> :: iterator it;
    for(int i=1; i<=n; i++){

        mp[clusters[i]]++;
    }
    vector< double > pi_avg,pi_max,pi_ic;
    //pi_avg
    for(int i=1;i<=cluster_cnt;i++){
        int u=cluster_head[i];
        double dis=0.0;
        for(int j=1;j<=n;j++){
            if(clusters[j]==i) dis+=fcost[u][j];
        }
        dis/=double(mp[clusters[i]]);
        //cout<<"avg: "<<dis<<endl;
        pi_avg.pb(dis);
    }

    //pi_max
    for(int i=1;i<=cluster_cnt;i++) {
        pi_max.pb(cluster_radius[i]);
        //cout<<"radius:  "<<cluster_radius[i]<<endl;
    }

    //pi_ic;
    for(int i=1;i<=cluster_cnt;i++){
        int u=cluster_head[i];
        double dis=0.0;
        for(int j=1;j<=n;j++){
            if(clusters[j]!=i) dis+=fcost[u][j];
        }
        double c=double(n-mp[clusters[i]]);
        if(c==0) c=1;
        dis/=c;
        //cout<<"ic: "<<dis<<endl;
        pi_ic.pb(dis);
    }
    double ans=0.0;
    for(int i=1;i<=cluster_cnt;i++){
        double a=pi_avg[i-1]+pi_ic[i-1]+pi_max[i-1];
        //cout<<"a: "<<a<<endl;
        ans+=a;
    }
    //cout<<"latency: "<<ans<<nl;
    //cout<<"ans: "<<ans<<endl;
    latency_k.pb(mk(cluster_cnt,ans));


}
void print()
{
    ofstream myfile1, myfile2;
    myfile1.open ("output_pi.txt");
    myfile2.open ("output_k.txt");
    map<int, double> mp;
    for(int i=0;i<latency_k.size();i++){
        if(mp.find(latency_k[i].ff)!= mp.end() )mp[latency_k[i].ff ]= min( mp[latency_k[i].ff ], latency_k[i].ss);
        else mp[latency_k[i].ff] =latency_k[i].ss;
    }
    for(int i=0;i<latency_k.size();i++){
        latency_k[i].ss= mp[latency_k[i].ff];
    }
    UNIQUE(latency_k);

    for(int i=0;i<latency_k.size();i++){
        myfile1<<fixed<<setprecision(3)<<latency_k[i].ss<<nl;
        myfile2<<latency_k[i].ff<<nl;
    }
    myfile1.close();
    myfile2.close();
    /*
    cout<<"Cluster Count: "<<cluster_cnt<<endl;

    map<int,int> mp;
    map<int,int> :: iterator it;
    for(int i=1; i<=n; i++){

        mp[clusters[i]]++;
    }


    int yy=-1;
    for(it=mp.begin();it!=mp.end();it++){

        yy=max(yy,it->second);
    }
    cout<<"Max Number of nodes in a cluster: "<<yy<<endl;
    */
}
int main()
{
    cluster_cnt = 0;
    take_input();

    SPICi();
    floyd_warshall();
    controller_head_set();
    clusters_head_distance();
    cluster_diameter();
    what_we_want();

    for(Ts=.001;Ts<=2.0;Ts+=.001){
        for(Td=0.000;Td<=2.0;Td+=.1){
            SPICi();
            //floyd_warshall();
            controller_head_set();
            clusters_head_distance();
            cluster_diameter();
            what_we_want();
        }
    }
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
