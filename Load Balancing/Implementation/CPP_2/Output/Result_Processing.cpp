#include <bits/stdc++.h>
using namespace std;


struct data
{
    int node_count;
    int k_lbc, k_dbcp;
    double lbc, dbc, dbcp;
    bool operator<(const data &rhs) const
    {
        return node_count < rhs.node_count;
    }
};

/**
Expresses each algorithm latency as a
fraction of LBC latency and writes them onto
text files: "All_latency_[algorithm_name].txt"
*/
void compare_latency();
void performance_problems();
void write_dbcp_k();
void forced_comparison();
void k_comparison();

int main()
{
//    compare_latency();

    performance_problems();

//    write_dbcp_k();

//    forced_comparison();

//    k_comparison();

    return 0;
}
void k_comparison()
{
    ifstream fin1("DBCP.txt");
    ifstream fin2("best_LBC.txt");

    string file_name;
    struct data result;
    double net_cnt[10] = {}, ignore;

    double avg_dbcp_k[10] = {}, avg_lbc_k[10] = {};

    while(fin1>>file_name)
    {
        fin2>>file_name;

        fin1>>result.node_count;
        fin1>>ignore>>result.k_dbcp>>result.dbcp;

        fin2>>result.node_count;
        fin2>>ignore>>result.k_lbc>>result.lbc;

        if(result.node_count<100)
        {
            int rnd = result.node_count / 10;
//            cout<<result.node_count<<' '<<rnd<<endl;
//            cout<<result.k_dbcp<<' '<<result.k_lbc<<' '<<rnd<<endl;
            net_cnt[rnd]++;
            avg_dbcp_k[rnd] += result.k_dbcp;
            avg_lbc_k[rnd] += result.k_lbc;
        }
    }
    for(int i=0; i<7; i++)
    {
        cout<<avg_dbcp_k[i]/net_cnt[i]<<endl;
        cout<<avg_lbc_k[i]/net_cnt[i]<<endl;
    }


    fin1.close();
    fin2.close();
}

void forced_comparison()
{
    ifstream fin1("DBCP.txt");
    ifstream fin2("DBC.txt");
    ifstream fin3("LBC.txt");

    string file_name;
    struct data results[300];
    int net_cnt = 0, ignore;

    while(fin1>>file_name)
    {
        fin2>>file_name;
        fin3>>file_name;

        fin1>>results[net_cnt].node_count;
        fin1>>ignore>>ignore>>results[net_cnt].dbcp;

        fin2>>results[net_cnt].node_count;
        fin2>>ignore>>ignore>>results[net_cnt].dbc;

        fin3>>results[net_cnt].node_count;
        fin3>>ignore>>ignore>>results[net_cnt].lbc;

        net_cnt++;
    }

    ofstream fout1("DBCP_forced.txt");
    ofstream fout2("DBC_forced.txt");
    ofstream fout3("LBC_forced.txt");
    double sub_net_cnt[10];
    double avg_dbc[10], avg_dbcp[10];
    for(int i=0; i<10; i++)
    {
        sub_net_cnt[i] = 0.0;
        avg_dbc[i] = 0.0;
        avg_dbcp[i] = 0.0;
    }
    for(int i=0; i<net_cnt; i++)
    {
        if(results[i].node_count < 100)
        {
            int idx = results[i].node_count/10;
            sub_net_cnt[idx]++;
            avg_dbcp[idx] += results[i].dbcp/results[i].lbc;
            avg_dbc[idx] += results[i].dbc/results[i].lbc;
        }
    }
    for(int i=0; i<10; i++)
    {
        fout1<<i*10+5<<' '<<avg_dbcp[i]/sub_net_cnt[i]<<endl;
        fout2<<i*10+5<<' '<<avg_dbc[i]/sub_net_cnt[i]<<endl;
        fout3<<i*10+5<<' '<<1<<endl;
    }
    fin1.close();
    fin2.close();
    fin3.close();
    fout1.close();
    fout2.close();
    fout3.close();
}

void write_dbcp_k()
{
    ifstream fin3("DBCP.txt");
    ofstream fout("All_k.txt");

    string file_name;
    int nodecnt, linkcnt, k;
    double flow_setup_latency;

    while(fin3>>file_name)
    {
        fin3>>nodecnt>>linkcnt;
        fin3>>k>>flow_setup_latency;
        fout<<k<<", ";
    }
    fout.close();
    fin3.close();
}

void performance_problems()
{

    ifstream fin1("LBC.txt");
    ifstream fin2("DBC.txt");
    ifstream fin3("DBCP.txt");
//    ofstream fout("Analysis.txt");

    string file_name;
    int nodecnt[3], linkcnt[3], k[3];
    double flow_setup_latency[3];
    double cnt = 0;
    double avg[3]={};
    int worse = 0;

    while(fin3>>file_name)
    {
        fin1>>file_name;
        fin1>>nodecnt[0]>>linkcnt[0];
        fin1>>k[0]>>flow_setup_latency[0];
        avg[0] += flow_setup_latency[0];

        fin2>>file_name;
        fin2>>nodecnt[1]>>linkcnt[1];
        fin2>>k[1]>>flow_setup_latency[1];
        avg[1] += flow_setup_latency[1];

        fin3>>nodecnt[2]>>linkcnt[2];
        fin3>>k[2]>>flow_setup_latency[2];
        avg[2] += flow_setup_latency[2];


        if(flow_setup_latency[0] <= flow_setup_latency[2])
        {
            cout<<file_name<<endl;
            cout<<k[0]<<' '<<nodecnt[0]<<' '<<linkcnt[0]<<endl;
            cout<<"LBC: "<<flow_setup_latency[0]<<"  DBC: "<<flow_setup_latency[1]<<"  DBCP: "<<flow_setup_latency[2]<<endl<<endl;
            worse++;
        }
        cnt++;
    }
    cout<<cnt<<' '<<worse<<endl;
    cout<<avg[0]/cnt<<' '<<avg[1]/cnt<<' '<<avg[2]/cnt<<endl;
//    fout.close();
    fin1.close();
    fin2.close();
    fin3.close();
}

void compare_latency()
{
    ifstream fin1("LBC.txt");
    ifstream fin2("DBC.txt");
    ifstream fin3("DBCP.txt");
    ofstream fout2("All_latency_DBC.txt");
    ofstream fout3("All_latency_DBCP.txt");

    string file_name;
    int nodecnt[3], linkcnt[3], k[3], testCase = 0;
    double flow_setup_latency[3];

    vector<pair<int, double> > dbc, dbcp, lbc;

    while(fin3>>file_name)
    {
        fin1>>file_name;
        fin1>>nodecnt[0]>>linkcnt[0];
        fin1>>k[0]>>flow_setup_latency[0];

        fin2>>file_name;
        fin2>>nodecnt[1]>>linkcnt[1];
        fin2>>k[1]>>flow_setup_latency[1];

        fin3>>nodecnt[2]>>linkcnt[2];
        fin3>>k[2]>>flow_setup_latency[2];

        lbc.push_back(make_pair(nodecnt[0], flow_setup_latency[0]));
        dbc.push_back(make_pair(nodecnt[1], flow_setup_latency[1]));
        dbcp.push_back(make_pair(nodecnt[2], flow_setup_latency[2]));

        testCase++;
    }
    cout<<testCase<<endl;
    fin1.close();
    fin2.close();
    fin3.close();

    sort(lbc.begin(), lbc.end());
    sort(dbc.begin(), dbc.end());
    sort(dbcp.begin(), dbcp.end());
    for(int i=0; i<testCase; i++)
    {
        fout2<<dbc[i].first<<' '<<dbc[i].second/lbc[i].second<<endl;
        fout3<<dbcp[i].first<<' '<<dbcp[i].second/lbc[i].second<<endl;
    }

    fout2.close();
    fout3.close();
}
