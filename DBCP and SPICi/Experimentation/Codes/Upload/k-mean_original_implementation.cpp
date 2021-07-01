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
double centers[1000][2];
double points[1000][2];
double center_node_cost[100][10000];
double node_centers[10000];
int n,k;
void print_centers(){
    cout<<"The centers are: "<<nl;
    for(int i=0;i<k;i++){
        cout<<centers[i][0]<<", "<<centers[i][1]<<nl;
    }
    cout<<nl;

}
void clusters_print(){
    cout<<"clusters are: "<<nl;
    for(int i=1;i<=n;i++){
        cout<<"point "<<i<<": "<< node_centers[i]+1<<nl;
    }


}
void input(){
    cout<<"Input number of points: ";
    cin>>n;
    cout<<"Input number of segments: ";
    cin>>k;
    cout<<"Give the n points: "<<nl;
    for(int i=0;i<n;i++){
        cin>>points[i][0]>>points[i][1];
    }


}




void random_center_selection(){

    srand(time(NULL));
    int num=0;
    while(num<k){
        int a=rand();
        int b=rand();
        a%=10;
        b%=10;
        centers[num][0]=a;
        centers[num][1]=b;
        num++;
    }
    //print_centers();


}


double dis(double x1, double y1, double x2, double y2){
    return sqrt(sq(x1-x2)+ sq(y1-y2));


}
void center_all_node_cost_calculate(){


    for(int i=0;i<n;i++){
        for(int j=0;j<k;j++){
            center_node_cost[j][i]=dis(centers[j][0],centers[j][1], points[i][0],points[i][1]);
        }

    }

}


void center_assign(){
    for(int i=1;i<=n;i++){
        double tocenter, cost=inf;
        for(int j=0;j<k;j++){
            if(center_node_cost[j][i]<cost){
                cost=center_node_cost[j][i];
                tocenter=j;
                //cout<<"point :  "<<i<<"  center: "<<j<<"   cost: "<<cost<<endl;
            }
        }
        node_centers[i]=tocenter;

    }

}

void new_center_selection(){
    for(int j=0;j<k;j++){

        double x=0,y=0;
        int ct=0;
        for(int i=1;i<=n;i++){
            if(node_centers[i]==j){
                ct++;
                x+=points[i][0];
                y+=points[i][1];

            }
        }
        if(ct!=0){
            x/=ct;
            y/=ct;
            centers[j][0]=x;
            centers[j][1]=y;
        }

    }

}

int main(){


    input();
    random_center_selection();

    int num_iteration=10;
    for(int i=0;i<num_iteration;i++){
        center_all_node_cost_calculate();
        center_assign();
        //clusters_print();
        new_center_selection();
        //print_centers();


    }
    print_centers();
    clusters_print();



}
