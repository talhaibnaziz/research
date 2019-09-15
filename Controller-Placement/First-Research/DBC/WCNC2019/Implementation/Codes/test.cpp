#include <bits/stdc++.h>
using namespace std;

struct node
{
    int cost;
    int id;
    bool operator<(const node& rhs) const
    {
        if(cost == rhs.cost)    id < rhs.id;
        return cost < rhs.cost;
    }
};

int main()
{
    int arr[10];
    for(int i=0; i<10; i++) arr[i]=10-i;
    sort(arr, arr+10);
    for(int i=0; i<10; i++) cout<<arr[i]<<endl;
    return 0;
}
