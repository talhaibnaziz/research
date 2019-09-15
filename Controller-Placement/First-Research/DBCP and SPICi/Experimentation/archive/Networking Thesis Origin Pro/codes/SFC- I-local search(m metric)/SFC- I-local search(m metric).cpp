//Taken from Spici Full complete
//No post processing is done.
//Head is selected according to the value of support. No extra algorithm.
//but the value of cost is inverse
// Max_cost= ( max value between all the edges +1)
// In find_max_wedge, density and support function the cost is changed to max_cost-cost.
// Because spicy always try to maximize.As the cost is inverse now it will try to minimize.

//Existing functions have been modified.
// New function is added for the result analysis
// from spici AX
// head choosing function is changed.
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
double Ts=.995, Td=0.0;
int cluster_head[1001];
double max_cost = 0;
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
        max_cost = max(max_cost, w);
    }
    max_cost++;
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
                wdeg += max_cost-graph[i][j].second;
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
            ret += max_cost- cost[S[i]][S[j]];
    if(s>1) ret = ret / (s*(s-1)/2.0);
    return ret;
}

double support(int t, vector <int> S)
{
    double ret = 0;
    for(int i=0; i<S.size(); i++)
        ret += max_cost-cost[S[i]][t];
    return ret;
}

void SPICi()
{
    memset(clusters, -1, sizeof clusters);
    int cnt = 0;
    cluster_cnt=0;
    while(cnt<n)
    {
        pair <int,double> p = find_max_wdeg();
        int u = p.first;

        cluster_cnt++; double cluster_size = 1.0;
        clusters[u] = cluster_cnt; cnt++;
        vector <int> S; S.push_back(u);
        set <int> Candidates;
        set <int> :: iterator itr;
        for(int j=0; j<graph[u].size(); j++)
        {
            int nxtnode = graph[u][j].first;
            if(clusters[nxtnode]!=-1)   continue;
            Candidates.insert(nxtnode);
        }
        while(!Candidates.empty())
        {
            int t;
            double max_sup = -1;
            for(itr=Candidates.begin(); itr!=Candidates.end(); ++itr)
            {
                int node = *itr;
                double sup = support(node, S);
                if(sup>max_sup)
                {
                    max_sup = sup;
                    t = node;
                }
            }
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
    for(int k=1;k<=cluster_cnt;k++){
        vector<int> cluster_set;
        For(i,1,n){
            if(clusters[i]==k) cluster_set.pb(i);

        }
        double maxv=inf;
        int head_node;
        for(int i=0;i<cluster_set.size();i++){
            int u=cluster_set[i];
            double pi_avg=0,pi_ic=0;
            double radius=-inf;
            for(int j=1;j<=n;j++){
                if(clusters[j]==k) {
                    pi_avg+=fcost[u][j];
                    radius=max(radius,fcost[u][j]);
                }
                if(clusters[j]!=k) pi_ic+=fcost[u][j];
            }

            pi_avg/=double(cluster_set.size());
            double c=double(n-cluster_set.size());
            if(c==0) c=1;
            pi_ic/=c;
            double laten= radius+pi_avg+pi_ic;
            if(laten<maxv){
                maxv=laten;
                head_node=u;
            }
        }
        cluster_head[k]=head_node;
    }


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

double mdcost[1000][1000];
int mdparent[1000][1000];
void m_metric_dijakstra(){
    for(int i=0;i<1000;i++){
        for(int j=0;j<1000;j++) mdcost[i][j]=inf,mdparent[i][j]=j;
    }
    for(int i=1;i<=n;i++){
        priority_queue<pair<double, int > > pq;
        pq.push(mk(0,i));
        mdcost[i][i]=0;
        while(!pq.empty()){
            pair<double,int> p,r;
            p=pq.top();
            int c=-p.ff;
            int u=p.ss;
            pq.pop();
            for(int j=0;j<graph[u].size();j++){
                r=graph[u][j];
                int v=r.ff;
                int new_c=c+r.ss;
                if(new_c<mdcost[i][v]){
                    mdcost[i][v]=new_c;
                    mdparent[i][v]=u;
                    pq.push(mk(-new_c,v));
                }
            }
        }
    }

}
pair<int,double> m_metric_generate(){

    double sum=0.0;
    for(int i=1;i<n;i++){
        int u=clusters[i];
        int uh=cluster_head[u];

        double dis1=fcost[i][uh];
        for(int j=i+1;j<=n;j++){
            double maxv=-1.0;
            int k=j;
            while(k!=i){
                int v=clusters[k];
                int vh=cluster_head[v];
                double dis2=fcost[k][vh]+fcost[uh][vh];
                k=mdparent[i][k];
                maxv=max(maxv,dis1+dis2);

            }
            sum+=maxv;

        }

    }
    sum/=double((n)* (n-1));
    return (mk(cluster_cnt,sum));


}

int local_check[1000][1000];
int ht=0;
void local_search(pair<int,double> temp){
    if(ht==1000) return;
    ht++;
    //cout<<ht<<nl;
    map<int,int> mp;
    for(int i=1; i<=n; i++) mp[clusters[i]]++;
    pair<int,double> p;
    int c=n*cluster_cnt;
    for(int i=0;i<c;i++){
        //cout<<i<<nl;
        int b= (rand()%cluster_cnt)+1;
        int a=(rand()%n) +1;

        if(local_check[a][b]==ht) {
            i--;
            continue;
        }
        local_check[a][b]=ht;

        int yy=clusters[a];
        if(mp[yy]==1) continue;
        if(clusters[a]==b) continue;
        clusters[a]=b;
        controller_head_set();
        p=m_metric_generate();
        if(p.ss<temp.ss){

            local_search(p);
            break;
        }
        else clusters[a]=yy;

    }
    return ;

}
void process(){

    pair<int,double> temp;
    SPICi();
    controller_head_set();
    temp=m_metric_generate();

    srand(time(nullptr));
    for(int i=1;i<=n;i++){
        int j=clusters[i];
        local_check[i][j]=1;
    }
    local_search(temp);
    controller_head_set();
    temp=m_metric_generate();
    cout<<temp.ff<<"   "<<temp.ss<<nl;
}


int main()
{
    cluster_cnt = 0;
    take_input();
    floyd_warshall();
    m_metric_dijakstra();
    process();
    cout<<"Controllers:\n";
    for(int i=0; i<cluster_cnt; i++)    cout<<cluster_head[i]<<endl;
    /*
    what_we_want();

    //for(Ts=.001;Ts<=2.0;Ts+=.001){
        for(Td=0.000;Td<=2.0;Td+=.001){
            SPICi();
            //floyd_warshall();
            controller_head_set();
            clusters_head_distance();
            cluster_diameter();
            what_we_want();
            our_metric_generate();
        }
    //}
    //print();
    //our_metric_print();
    */
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
