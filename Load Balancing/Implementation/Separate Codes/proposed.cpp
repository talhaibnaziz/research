#include <bits/stdc++.h>

using namespace std;

double switchcnt;
double linkcnt;
double controller_bw;
double switch_load[10000];

void take_input();

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(); cout.tie();
    take_input();
}

void take_input()
{
    cout<<"Enter number of switches: ";
    cin>>switchcnt;
    cout<<"Enter number of links: ";
    cin>>linkcnt;
    cout<<"Enter controller bandwidth: ";
    cin>>controller_bw;
    count<<"Enter node id with load:\n"

    int a; double load;
    for(int i=0; i<switchcnt; i++)
    {
        cin>>a>>load;
        switch_load[a] = load;
    }
    for(int i=0; i<linkcnt; i++)
    {

    }
}
