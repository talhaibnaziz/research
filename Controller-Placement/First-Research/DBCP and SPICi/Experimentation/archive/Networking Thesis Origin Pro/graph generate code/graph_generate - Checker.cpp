//First line will contain T, the number of Test case
// for every test case there will be two integers , n and m; the number of nodes and edges
// then m line will follow each containing three integers a,b,c which mean there is
// an edge between a and b with weight c.
// node are numbered from 1 to n.
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
int ara[1005][1005];

int pr[1005];
int findp(int r)
{
   return (pr[r]==r) ? r:  pr[r]=findp(pr[r]);
}
struct node{
    int a,b,c;
    node(){};
    node(int x,int y, int z){
        a=x;
        b=y;
        c=z;
    }
};
int main(){
    freopen("graph_input.txt","r",stdin);
    int T;
    cin>>T;
    for(int tc=1;tc<=T;tc++){
        int n,m;
        cin>>n>>m;
        int limit=(n*(n-1))/2;

        for(int i=0;i<1005;i++) pr[i]=i;
        while(m){
            int a,b,c;
            cin>>a>>b>>c;
            m--;
            int u = findp(a);
            int v= findp(b);
            if(u!=v){
                pr[u]=v;
            }
        }
        set<int> st;
        for(int i=1;i<=n;i++){
            st.insert(findp(i));
        }
        if(m>limit) cout<<tc<<":  Limit Crossed "<<n<<"    "<<m<<endl;
        if(st.size()>1) cout<<"invalid graph  "<<tc<<"    size: "<< st.size()<<endl;



    }


}
