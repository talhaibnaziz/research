#include <iostream>
#include <fstream>
#include "input.h"
#include "dbcp.h"
#include "spici.h"
#include "idbc.h"
#include <map>
#include <algorithm>
using namespace std;

int spicik[] = {8,11,11,14,15,18,22};
int uspicik[] = {6,6,9,10,16,14,12};

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cout<<"DEBUG:   "<<endl;
    //Take Input
    input Input[10] = input();

    ofstream fout("mspici_example.txt");
    spici Spici = spici();

    for(int i=8; i<9; i++)
    {
        string inputstr = "1";
        inputstr.push_back(i+48);
        inputstr.push_back('.');
        inputstr.push_back('t');
        inputstr.push_back('x');
        inputstr.push_back('t');
        cout<<inputstr<<endl;
        Input[i].take_input(inputstr);
//        Input[i].make_unweighted();
        Spici.set_input(Input[i], 1);
        Spici.set_myc(4);
        Spici.find_degree();
        double minans = infinity;
        for(double ts=0.5; ts<=0.5; ts+=0.01)
        {
            Spici.set_ts(ts);
            for(double td=0.5; td<=0.5; td+=0.01)
            {
                Spici.set_td(td);
                Spici.cluster();
                Spici.set_controllers();
                double ans = Spici.newlatency();
                Spici.latency = ans;
                ans = Spici.local_search(100, 0);
                if(minans>ans)  minans = ans;
//                vector <double> v = Spici.myoutput();
//                if(minans>v[0]) minans = v[0];
            }
        }
        fout<<i<<' '<<Input[i].get_nodescnt()<<' '<<Input[i].get_edgecnt()<<' '<<Spici.cluster_cnt<<' '<<minans<<endl;
    }
    fout.close();


//    ofstream fout("dbcp_new.txt");
//    dbcp Dbcp = dbcp();
//
//    for(int i=1; i<8; i++)
//    {
//        string inputstr = "1";
//        inputstr.push_back(i+48);
//        inputstr.push_back('.');
//        inputstr.push_back('t');
//        inputstr.push_back('x');
//        inputstr.push_back('t');
//        cout<<inputstr<<endl;
//        Input[i].take_input(inputstr);
//
//        Dbcp.set_input(Input[i], 1);
//
//        Input[i].make_unweighted();
//
//        double diameter = Input[i].find_diameter();
//        cout<<"DIAMETER: "<<diameter<<endl;
//        map <int, vector <double> > mpres;
//        vector <int> kvalues;
//
//        double dcstart = diameter*0.3, dcend = diameter*0.5;
//        for(double dc=dcstart; dc<=dcend; dc+=1)
//        {
//            Dbcp.set_dc(dc);
//            Dbcp.refresh(1);
//            int k = Dbcp.find_k();
////            cout<<"K: ("<<dc<<")  "<<k<<endl;
////            if(1<k) Dbcp.clustermod(1);
////            else    Dbcp.cluster(1);
////            k = 1;
//            if(k==1)    k=2;
//            bool check = Dbcp.cluster(k);
//            if(!check)  continue;
//            Dbcp.set_controllers(k);
//            Dbcp.calculate_dis();
////            Dbcp.set_output(Dbcp.newlatency());
////            vector <double> ans = Dbcp.local_search(5,0,0);
//            vector <double> ans = Dbcp.newlatency();
//            if(k!=0)
//            {
//                if(mpres.count(k)==0)    kvalues.push_back(k);
//                if(mpres.count(k)==0 || mpres[k][0]>ans[0]) mpres[k] = ans;
//            }
//        }
//        sort(kvalues.begin(), kvalues.end());
//        int outk; double outl = infinity;
//        for(int j=0; j<kvalues.size(); j++)
//        {
////            fout<<i<<' '<<kvalues[j]<<":    "<<mpres[kvalues[j]][0]<<endl;
//            if(outl>mpres[kvalues[j]][0])
//            {
//                outk = kvalues[j];
//                outl = mpres[kvalues[j]][0];
//            }
//        }
//        fout<<i<<' '<<outk<<' '<<outl<<endl;
//    }
//    fout.close();

//    Spici Modules at work
//    spici Spici = spici();
//    Spici.set_input(Input, 0);
//    Spici.set_ts(0.5);
//    Spici.set_td(0.5);
//    Spici.find_degree();
//    Spici.cluster();
//    Spici.set_controllers();
//    double ans = Spici.newlatency();
//    cout<<ans<<endl;

    //DBCP Modules at work
//    dbcp Dbcp = dbcp();
//    Dbcp.set_input(Input);
//    double diameter = Input.find_diameter();
//    map <int, double> mpres;
//    vector <int> kvalues;
//
//    double dcstart = diameter*0.3, dcend = diameter*0.5;
//    for(double dc=dcstart; dc<=dcend; dc+=0.5)
//    {
//        Dbcp.set_dc(dc);
//        int k = Dbcp.find_k();
//        Dbcp.cluster(k);
//        Dbcp.set_controllers(k);
//        Dbcp.calculate_dis();
//        double ans = Dbcp.newlatency();
//        if(k!=0)
//        {
//            if(mpres.count(k)==0)    kvalues.push_back(k);
//            if(mpres.count(k)==0 || mpres[k]>ans) mpres[k] = ans;
//        }
//    }
//    sort(kvalues.begin(), kvalues.end());
//    for(int i=0; i<kvalues.size(); i++)
//    {
//        cout<<kvalues[i]<<":    "<<mpres[kvalues[i]]<<endl;
//    }

//    ofstream fout;
//    fout.open("dbcp_1.txt");
//
//    sort(kvalues.begin(), kvalues.end());
//    for(int i=0; i<kvalues.size(); i++)
//    {
//        cout<<kvalues[i]<<":    "<<mpres[kvalues[i]][0]<<endl;
//        int tempk = kvalues[i];
//        fout<<tempk<<' '<<mpres[tempk][0]<<' '<<mpres[tempk][1];
//        fout<<' '<<mpres[tempk][2]<<' '<<mpres[tempk][3]<<endl;
//    }
//    fout.close();
//
//    //DBCP Local Search Modules at work
//    mpres.clear(); kvalues.clear();
//    for(double dc=dcstart; dc<=dcend; dc+=0.5)
//    {
//        Dbcp.set_dc(dc);
//        int k = Dbcp.find_k();
//        Dbcp.cluster(k);
//        Dbcp.set_controllers(k);
//        Dbcp.calculate_dis();
//        Dbcp.set_output(Dbcp.output());
//        vector <double> v = Dbcp.local_search(5,0);
//        if(k!=0)
//        {
//            if(mpres.count(k)==0)    kvalues.push_back(k);
//            if(mpres.count(k)==0 || mpres[k][0]>v[0]) mpres[k] = v;
//        }
//    }
//
//    fout.open("ldbcp_1.txt");
//
//    sort(kvalues.begin(), kvalues.end());
//    for(int i=0; i<kvalues.size(); i++)
//    {
//        cout<<kvalues[i]<<":    "<<mpres[kvalues[i]][0]<<endl;
//        int tempk = kvalues[i];
//        fout<<tempk<<' '<<mpres[tempk][0]<<' '<<mpres[tempk][1];
//        fout<<' '<<mpres[tempk][2]<<' '<<mpres[tempk][3]<<endl;
//    }
//    fout.close();


    return 0;
}
