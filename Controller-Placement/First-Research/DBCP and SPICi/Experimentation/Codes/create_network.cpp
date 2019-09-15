#include <bits/stdc++.h>
using namespace std;

int main()
{
    ofstream fout("random_input.txt");
    fout<<"400 500"<<endl;
    for(int i=1; i<400; i++)
    {
        for(int j=i+1; j<=400; j++)
        {
            fout<<i<<' '<<j<<"\n";
        }
    }
}
