#include <bits/stdc++.h>
using namespace std;


int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie();
    cout.tie();

    int nodecnt;
    double linkcnt;
    double avg_linkcnt;
    for(int testcase=1; testcase<=7; testcase++)
    {
        //string file_name="Input/test_1.";
        string file_name="Input/";
        file_name.push_back(testcase+48);
        file_name.push_back('.');
        file_name.push_back('t');
        file_name.push_back('x');
        file_name.push_back('t');
        ifstream fin(file_name);

        avg_linkcnt = 0.0;
        for(int tc = 0; tc<10; tc++)
        {
            fin>>nodecnt>>linkcnt;
            int u, v; double w;
            for(int i=0; i<linkcnt; i++)
            {
                fin>>u>>v>>w;
            }
            avg_linkcnt += linkcnt;
        }
        avg_linkcnt /= 10.0;
        cout<<nodecnt<<' '<<avg_linkcnt<<' '<<avg_linkcnt/(double)nodecnt<<endl;
    }

    return 0;
}
