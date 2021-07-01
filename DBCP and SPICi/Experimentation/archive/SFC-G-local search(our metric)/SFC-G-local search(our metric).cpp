
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
double Ts=0.5, Td=.5;
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
double haha_dis1, haha_dis2;
pair<int,double> our_metric_generate(){
    map<int,int> mp;
    map<int,int> :: iterator it;
    for(int i=1; i<=n; i++){

        mp[clusters[i]]++;
    }

    double dis1=0.0;
    for(int i=1;i<=cluster_cnt;i++){
        int u=cluster_head[i];

        for(int j=1;j<=n;j++){
            if(clusters[j]==i) dis1+=fcost[u][j];
        }
    }
    dis1=dis1/(double(cluster_cnt));

    double dis2=0.0;
    for(int i=1;i<=cluster_cnt;i++){
        int u=cluster_head[i];
        for(int j=1;j<=cluster_cnt;j++){
            int v=cluster_head[j];
            dis2+=fcost[u][v];
        }
    }
    dis2=dis2/(2.0*cluster_cnt);
    haha_dis1=dis1;
    haha_dis2=dis2;
    return (mk(cluster_cnt,dis1+dis2));


}

int local_check[1000][1000];
int ht=0;
void local_search(pair<int,double> temp){
    if(ht==100) return;
    ht++;
    map<int,int> mp;
    for(int i=1; i<=n; i++) mp[clusters[i]]++;
    int c=rand();
    pair<int,double> p;
    c=c%100+50;
    for(int i=0;i<c;i++){
        int b= (rand()%cluster_cnt)+1;
        int a=(rand()%n) +1;
        if(local_check[a][b]) continue;
        local_check[a][b]=1;

        int yy=clusters[a];
        if(mp[yy]==1) continue;
        clusters[a]=b;
        controller_head_set();
        clusters_head_distance();
        cluster_diameter();
        p=our_metric_generate();
        if(p.ss<temp.ss){

            local_search(p);
            break;
        }
        else clusters[a]=yy;

    }
    return ;

}
void process(){
    double low=0.000001;
    double high=1.500000;
    double mid1,mid2;
    pair<int,double> temp1,temp2;
    for(int i=0;i<=60;i++){
        double add=(high-low)/3.0;
        mid1=low+add;
        mid2=mid1+add;
        Ts=mid1;
        SPICi();
        controller_head_set();
        clusters_head_distance();
        cluster_diameter();
        temp1=our_metric_generate();
        Ts=mid2;
        SPICi();
        controller_head_set();
        clusters_head_distance();
        cluster_diameter();
        temp2=our_metric_generate();
        if(temp1.ss<temp2.ss) high=mid2;
        else low=mid1;


    }
    srand(time(nullptr));
    for(int i=1;i<=n;i++){
        int j=clusters[i];
        local_check[i][j]=1;
    }
    local_search(temp1);
    controller_head_set();
    clusters_head_distance();
    cluster_diameter();
    temp1=our_metric_generate();
    cout<<temp1.ff<<"   "<<temp1.ss <<"  "<<haha_dis1<<"  "<<haha_dis2<<nl;
}

int main()
{
    cluster_cnt = 0;
    take_input();
    floyd_warshall();
    process();
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
