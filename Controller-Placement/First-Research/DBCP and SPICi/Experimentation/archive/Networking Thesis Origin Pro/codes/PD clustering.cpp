
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
#define inf            9999999.0
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
double Ts=0.5, Td=.05;
int tg,myc;
int cluster_head[1001];
double deg[1001];
int mincluster,maxcluster;

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
        fcost[u][v] = w; fcost[v][u] = w;
        pair <int, double> p;
        p.first = v; p.second = w;
        graph[u].push_back(p);
        p.first = u;
        graph[v].push_back(p);
        cost[u][v] = 1.0/w; cost[v][u] =1.0/ w;


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
                wdeg += 1.0/graph[i][j].second;
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

int second_seed(int u)
{
    vector <int> bin[5];
    for(int i=0; i<graph[u].size(); i++)
    {
        int nxtnode = graph[u][i].first;
        if(clusters[nxtnode]!=-1)   continue;
        double cost = 1.0/graph[u][i].second;
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
                int nxtnode = graph[node][k].first;
                if(clusters[nxtnode]!=-1)   continue;
                wdeg += 1.0/graph[node][k].second;
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

double density(vector <int> S, int addnode)
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
    cluster_cnt = 1;
    memset(clusters, -1, sizeof clusters);
    //while all nodes not clustered
    while(cnt<n)
    {
        //find max weighted degree of unclustered nodes
        if(cluster_cnt==maxcluster) break;
        pair <int,double> p = find_max_wdeg();
        int u = p.first;
        //cout<<"max weighted unclustered node: ";
        //cout<<u<<'('<<p.second<<')'<<endl;



        //if I want isolated parts
        if(p.second == 0)
        {
            int newhead; double distance = inf;
            for(int i=0; i<graph[u].size(); i++)
            {
                int adj = graph[u][i].second;
                int head = cluster_head[adj];
                if(fcost[u][head]<distance)
                {
                    distance = fcost[u][head];
                    cluster_head[u]=head;
                    clusters[u]=clusters[head];
                }
            }
            cnt++;
            continue;
        }

        //*********************************
        int v = second_seed(u);
        cluster_head[u] = u;

        //no second seed
        if(v==-1)
        {
            cluster_cnt++;
            clusters[u] = cluster_cnt;
            cnt++;
            //cout<<"NO SECOND SEED"<<endl;
        }
        //Expand cluster
        else
        {
            //cout<<"Second Seed: "<<v<<endl;
            cluster_cnt++; double cluster_size = 2.0;
            clusters[u] = cluster_cnt;
            clusters[v] = cluster_cnt;
            cnt += 2;
            vector <int> S; S.push_back(u); S.push_back(v);
            set <int> Candidates;
            set <int> :: iterator itr;
            //insert adjacent unclustered nodes into set Candidates
            for(int i=0; i<2; i++)
            {
                int node = S[i];
                for(int j=0; j<graph[node].size(); j++)
                {
                    int nxtnode = graph[node][j].first;
                    if(clusters[nxtnode]!=-1)   continue;
                    Candidates.insert(nxtnode);
                }
            }
            bool stop_clustering = 0;
            while(!Candidates.empty())
            {
                if(stop_clustering) {
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
                    //cout<<"Candidates are: "<<node<<"=>"<<sup<<endl;
                    if(sup>max_sup)
                    {
                        max_sup = sup;
                        t = node;
                    }
                }
                //insert into Set of Candidates or stop expanding cluster
                //cout<<"Working with Candidate: ";
                //cout<<t<<'('<<max_sup<<')'<<endl;
                if(max_sup>=Ts*cluster_size*density(S,-1) && density(S,t)>Td)
                {
                    S.push_back(t);
                    cluster_size++; cnt++;
                    clusters[t] = cluster_cnt;
                    for(int j=0; j<graph[t].size(); j++)
                    {
                        int nxtnode = graph[t][j].first;
                        if(clusters[nxtnode]!=-1)   continue;
                        Candidates.insert(nxtnode);
                    }
                }
                else    stop_clustering = 1;
            }
        }
    }
}

void find_degree(){
    vector< pair<double,int > > degree;
    for(int i=1; i<=n; i++)
    {
        double d = (double)graph[i].size();
        pair <double, int> p;
        p.first = d;
        p.second = i;
        degree.push_back(p);
        deg[i] = d;
        //cout<<i<<' '<<deg[i]<<endl;
    }
    //cout<<n<<endl;
    sort(degree.begin(), degree.end());
    int idx = (myc*n)/5;
//    cout<<"Degrees limit:   "<<degree[idx].first<<endl;
    tg = degree[idx].first;
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
        mdcost[i][i]=0.0;
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
double m_metric_generate(){

    double sum=0.0;
    for(int i=1;i<n;i++){
        int u=clusters[i];
        int uh=cluster_head[u];

        double dis1=fcost[i][uh];
        //cout<<"i: "<<i<<"  uh:  "<<uh<<endl;
        //cout<< "dis1: "<<fixed<<setprecision(5)<<dis1 <<endl;
        //if(dis1>=inf-1000) cout<<"i  "<<i<<endl;
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
    return (sum);


}

void  Assign_unclustered_node(){
    for(int i=1;i<=n;i++){
        if(clusters[i]==-1){
            double min_dis=inf, v;
            for(int k=1;k<=cluster_cnt;k++){
                int u=cluster_head[k];
                double dis=fcost[i][u];
                if(dis<min_dis){
                    min_dis=dis;
                    v=u;
                }


            }
            clusters[i]=v;
        }
    }

}
int local_check[1000][1000];
int ht=0;
void local_search(double temp){
    if(ht==10000) return;
    ht++;
    map<int,int> mp;
    for(int i=1; i<=n; i++) mp[clusters[i]]++;
    int c=rand();
    double p;
    c=c%100+50;
    for(int i=0;i<c;i++){
        int b= (rand()%cluster_cnt)+1;
        int a=(rand()%n) +1;
        //if(local_check[a][b]) continue;
        local_check[a][b]=1;

        int yy=clusters[a];
        if(mp[yy]==1) continue;
        clusters[a]=b;
        controller_head_set();
        p=m_metric_generate();
        if(p<temp){

            local_search(p);
            break;
        }
        else clusters[a]=yy;

    }
    return ;

}
void print()
{
    cout<<"Cluster Count: "<<cluster_cnt<<endl;
    cout<<"Nodes and their Clusters:"<<endl;
    for(int i=1; i<=n; i++)
        cout<<i<<'('<<clusters[i]<<')'<<endl;
}






void Algo(){

    mincluster=8;
    maxcluster=8;
    Ts=.1;
    cluster_cnt=0;
    while(cluster_cnt<mincluster){
        SPICi();
        if(cluster_cnt<mincluster) Ts+=0.05;
        //break;
        //cout<<"- "<<cluster_cnt<<endl;
    }
    if(cluster_cnt<mincluster){
        cout<<"Max number of cluster possible is : "<<cluster_cnt<<endl;
        return;
    }
    controller_head_set();
    //print();
    Assign_unclustered_node();
    controller_head_set();
    double m_metric_value=m_metric_generate();
    //cout<<"m: "<<m_metric_value<<endl;
    srand(time(nullptr));
    for(int i=1;i<=n;i++){
        int j=clusters[i];
        local_check[i][j]=1;
    }
    local_search(m_metric_value);
    controller_head_set();
    m_metric_value=m_metric_generate();
    cout<<cluster_cnt<<"  ";
    cout<<fixed<<setprecision(3)<<m_metric_value<<endl;
    //print();



}

int main()
{
    myc=4;
    take_input();
    floyd_warshall();
    find_degree();

    m_metric_dijakstra();
    Algo();

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
