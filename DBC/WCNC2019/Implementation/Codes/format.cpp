#include <bits/stdc++.h>
using namespace std;

int main()
{
    /*OMEGA VS K*/
//    ifstream fin("Results/k_vs_omega_u70_dbc.txt");
//    ofstream fout1("Results/formatted1.txt");
//    ofstream fout2("Results/formatted2.txt");
//    int i = 1; double t;
//    while(fin>>t)
//    {
//        if(i%2==0)  fout2<<t<<endl;
//        else    fout1<<t<<endl;
//        cout<<t<<endl;
//        i++;
//    }
//    fin.close();
//    fout1.close();
//    fout2.close();

    /*DBCP vs DBC*/
    ifstream fin1("Results/our_avg_unweighted_dbc.txt");
    ifstream fin2("Results/avg_unweighted_dbcp.txt");
    ofstream fout1("Results/formatted1.txt");
    ofstream fout2("Results/formatted2.txt");
    int i = 1; double t1, t2;
    while(fin1>>t1)
    {
        fin2>>t2;
        if((i-2)%3==0)
        {
            fout1<<t1<<endl;
            fout2<<t2<<endl;
        }
        i++;
    }
    fin1.close();
    fin2.close();
    fout1.close();
    fout2.close();

    /*IMPROVEMENT vs K*/
//    ifstream fin1("Results/improvement_vs_k_u70.txt");
//    ifstream fin2("Results/improvement_vs_k_u80.txt");
//    ofstream fout1("Results/formatted1.txt");
//    ofstream fout2("Results/formatted2.txt");
//    int i = 1; double t1, t2;
//    while(fin1>>t1)
//    {
//        fin2>>t2;
//        if(i%3==0)
//        {
//            fout1<<t1<<endl;
//            fout2<<t2<<endl;
//        }
//        i++;
//    }
//    fin1.close();
//    fin2.close();
//    fout1.close();
//    fout2.close();

    return 0;
}
