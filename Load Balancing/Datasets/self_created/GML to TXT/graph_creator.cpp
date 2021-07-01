#include <bits/stdc++.h>
#include<dirent.h>
using namespace std;

#define DATASET_INPUT_PATH "./GML"
#define DATASET_OUTPUT_PATH "./TXT"

int main()
{
    ifstream fin;
    ofstream fout;
    char file_in[100], file_out[100];
    char file_input[1000];
    int node_idx;
    int file_out_len;
    bool flag, speed;

    DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir(DATASET_INPUT_PATH) ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
            {
                //finding GML files
                if(entry->d_name[entry->d_namlen-3]=='g')
                {
                    //creating text file name
                    strcpy(file_in, DATASET_INPUT_PATH);
                    strcat(file_in, "/");
                    strcat(file_in, entry->d_name);
                    fin.open(file_in);

                    strcpy(file_out, DATASET_OUTPUT_PATH);
                    strcat(file_out, "/");
                    strcat(file_out, entry->d_name);
                    file_out_len = strlen(file_out);
                    file_out[file_out_len-1] = 't';
                    file_out[file_out_len-2] = 'x';
                    file_out[file_out_len-3] = 't';
                    fout.open(file_out);

                    node_idx = -2;
                    flag = true;

                    while(fin>>file_input)
                    {
                        if(strcmp(file_input, "label")==0 && flag)
                        {
                            if(++node_idx==-1)    continue;
                            fin.getline(file_input, 90);
                            fout<< node_idx << " " << file_input <<endl;
                        }
                        else if(strcmp(file_input, "source")==0)
                        {
                            if(flag)
                            {
                                flag = false;
                                fout<<"-1\n";
                            }
                            fin>>file_input;
                            fout<<file_input<<' ';
                        }
                        else if(strcmp(file_input, "target")==0)
                        {
                            fin>>file_input;
                            fout<<file_input<<' ';
                            speed = false;
                        }
                        else if(strcmp(file_input, "LinkSpeedRaw")==0)
                        {
                            speed = true;
                            double lspeed;
                            fin>>lspeed;
                            fout<<lspeed/1000000000.0<<endl;
                        }
                        else if(strcmp(file_input, "LinkLabel")==0)
                        {
                            fin>>file_input;
                            if(strcmp(file_input, "\"DWDM\"")==0)
                            {
                                fout<<2.5<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"STM-16\"")==0 || strcmp(file_input, "\"OC48/STM16\"")==0)
                            {
                                fout<<2.5<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"STM-64\"")==0 || strcmp(file_input, "\"OC192/STM64\"")==0)
                            {
                                fout<<10<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"STM-4\"")==0 || strcmp(file_input, "\"OC12/STM4\"")==0)
                            {
                                fout<<0.622<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"STM-1\"")==0 || strcmp(file_input, "\"OC3")==0)
                            {
                                fout<<0.155<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"T1\"")==0)
                            {
                                fout<<0.0015<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"T2\"")==0)
                            {
                                fout<<0.0063<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"T3\"")==0)
                            {
                                fout<<0.045<<endl;
                                speed = true;
                            }
                            else if(file_input[strlen(file_input)-5]=='G')
                            {
                                //Only for a special case
                                speed = true;
                                char data_rate[10];
                                double lspeed;
                                bool minus_gone = false;
                                int i, j;
                                for(i=0, j=0; file_input[i]; i++)
                                {
                                    if(file_input[i]=='-')
                                    {
                                        minus_gone = true;
                                        continue;
                                    }
                                    if(minus_gone && isdigit(file_input[i]))
                                        data_rate[j++] = file_input[i];
                                }
                                data_rate[j] = '\0';
                                lspeed = atof(data_rate);
                                fout<<lspeed<<endl;
                            }
                            else if(file_input[strlen(file_input)-5]=='M')
                            {
                                //Only for a special case
                                speed = true;
                                char data_rate[10];
                                double lspeed;
                                bool minus_gone = false;
                                int i, j;
                                for(i=0, j=0; file_input[i]; i++)
                                {
                                    if(file_input[i]=='-')
                                    {
                                        minus_gone = true;
                                        continue;
                                    }
                                    if(minus_gone && isdigit(file_input[i]))
                                        data_rate[j++] = file_input[i];
                                }
                                data_rate[j] = '\0';
                                lspeed = atof(data_rate);
                                fout<<lspeed/1000.0<<endl;
                            }
                        }
                        else if(strcmp(file_input, "LinkType")==0)
                        {
                            fin>>file_input;
                            if(strcmp(file_input, "\"STM-16\"")==0 || strcmp(file_input, "\"OC48/STM16\"")==0 || strcmp(file_input, "\"OC-48\"")==0)
                            {
                                fout<<2.5<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"STM-64\"")==0 || strcmp(file_input, "\"OC192/STM64\"")==0 || strcmp(file_input, "\"OC-192\"")==0)
                            {
                                fout<<10<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"STM-4\"")==0 || strcmp(file_input, "\"OC12/STM4\"")==0 || strcmp(file_input, "\"OC-12\"")==0)
                            {
                                fout<<0.622<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"STM-1\"")==0 || strcmp(file_input, "\"OC3")==0)
                            {
                                fout<<0.155<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"T1\"")==0)
                            {
                                fout<<0.0015<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"T2\"")==0)
                            {
                                fout<<0.0063<<endl;
                                speed = true;
                            }
                            else if(strcmp(file_input, "\"T3\"")==0)
                            {
                                fout<<0.045<<endl;
                                speed = true;
                            }
                        }
                        else if(!flag && !speed && strcmp(file_input, "[")==0)
                        {
                            fout<<1<<endl;
                        }
                    }
                    if(!speed)  fout<<1<<endl;
                    fin.close();
                    fout.close();
                }
            }
        }
        closedir(pDIR);
    }

    return 0;
}
