
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
            //cout<<"Cluster Number "<<k<<" node : "<<u<<"  latency: "<<laten<<endl;
            //cout<<radius<<" "<<pi_avg<<" "<<pi_ic<<nl;
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
        mdcost[i][i]=0.0;
        pq.push(mk(0,i));
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
            //cout<<"i: "<<i<<"   j: "<<j<<nl;
            while(k!=i){
                //cout<<"k: "<<k<<nl;
                int v=clusters[k];
                int vh=cluster_head[v];
                double dis2=fcost[k][vh]+fcost[uh][vh];
                k=mdparent[i][k];
                maxv=max(maxv,dis1+dis2);

            }
            //cout<<"NODE PAIRS: "<<i<<" "<<j<<"  LATENCY: "<<maxv<<nl;
            sum+=maxv;

        }

    }
    sum/=double((n)* (n-1));
    return (mk(cluster_cnt,sum));


}


int local_check[1000][1000];
int ht=0;
void local_search(pair<int,double> temp){
    if(ht==100) return;
    ht++;
    map<int,int> mp;
    for(int i=1; i<=n; i++) mp[clusters[i]]++;
    pair<int,double> p;
    int c=n*cluster_cnt;
    for(int i=0;i<c;i++){
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
    srand(time(nullptr));
    map<int,int> cluster_head_taken;
    for(int i=1;i<=cluster_cnt;i++){
        int a= rand()%n + 1;
        if(cluster_head_taken[a]==1){
            i--;
            continue;
        }
        cluster_head_taken[a]=1;
        cluster_head[i]=a;

    }
    for(int i=1;i<=n;i++){
        int minv=inf,pos;
        for(int j=1;j<=cluster_cnt;j++){
            if(fcost[i][j]<minv){
                minv=fcost[i][j];
                pos=j;
            }
        }
        clusters[i]=pos;
    }

    pair<int,double> temp1;
    controller_head_set();
    temp1=m_metric_generate();
    local_search(temp1);
    controller_head_set();
    cout<<"Cluster head: "<<cluster_head[1]<<nl;
    temp1=m_metric_generate();
    cout<<temp1.ff<<"   "<<temp1.ss<<nl;

}

int main()
{
    //fwrite
    cluster_cnt = 1;
    take_input();

    floyd_warshall();
    m_metric_dijakstra();
    cluster_cnt;
    process();


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

