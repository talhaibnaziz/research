
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
double Ts=0.0, Td=0.0;
double max_cost = 0;
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

int second_seed(int u)
{
    vector < pair<double, int> > bin;
    for(int i=0; i<graph[u].size(); i++)
    {
        int nxtnode = graph[u][i].first;
        if(clusters[nxtnode]!=-1)   continue;
        double cost = max_cost-graph[u][i].second;
        bin.push_back(mk(cost,nxtnode));
    }
    SORT(bin);
    reverse(all(bin));
    int s_seed; double max_wdeg = -1;
    for(int j=0; j<bin.size(); j++)
    {
        int node = bin[j].ss;
        double wdeg = 0;
        for(int k = 0; k<graph[node].size(); k++)
        {
            int nxtnode = graph[node][k].first;
            if(clusters[nxtnode]!=-1)   continue;
            wdeg +=max_cost- graph[node][k].second;
        }
        if(wdeg>max_wdeg)
        {
            max_wdeg = wdeg;
            s_seed = node;
        }
    }
    if(max_wdeg>-1) return s_seed;
    return -1;
}

double density(vector <int> S, int addnode)
{
    if(addnode!=-1) S.push_back(addnode);
    double ret = 0;
    double s = S.size();
    for(int i=0; i<S.size()-1; i++)
        for(int j=i+1; j<S.size(); j++)
            ret += max_cost-cost[S[i]][S[j]];
    ret = ret / (s*(s-1)/2.0);
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
    int cnt = 0;
    //while all nodes not clustered
    while(cnt<n)
    {
        //find max weighted degree of unclustered nodes
        pair <int,double> p = find_max_wdeg();
        int u = p.first;
        //cout<<"max weighted unclustered node: ";
        //cout<<u<<'('<<p.second<<')'<<endl;
        int v = second_seed(u);

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
                if(stop_clustering) break;
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
               // cout<<t<<'('<<max_sup<<')'<<endl;
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
                break;
            }
        }
    }
}

void print()
{
    cout<<"Cluster Count: "<<cluster_cnt<<endl;
    cout<<"Nodes and their Clusters:"<<endl;
    for(int i=1; i<=n; i++)
        cout<<i<<'('<<clusters[i]<<')'<<endl;
}

int main()
{
    cluster_cnt = 0;
    memset(clusters, -1, sizeof clusters);
    take_input();
    SPICi();
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
