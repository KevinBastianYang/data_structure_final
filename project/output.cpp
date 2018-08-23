//output.cpp
//将连接后的mapping序列信息输出
//输出信息包括mapping的位置、mapping的打分

#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include <string.h>
#include<cmath>
#include"shortReadMapping_class.h"
using namespace std;

void shortReadMapping::output()
{
    ofstream outfile("SRM.txt",ofstream::out);

    outfile<<"ShortReadMapping Result:"<<endl;
    outfile<<"  Explanation:"<<endl;
    outfile<<"    The Mapping sequence in the long sequence and the short sequence will be shown below"<<endl;
    outfile<<"    And the position information is shown as <--start    end-->"<<endl;
    outfile<<"    Other information includes the mapping score"<<endl;
    outfile<<"<---------------------------------------------------------------------------------------------->"<<endl;
    outfile<<"Results:"<<endl;
    for (int i=0;i<seedStorage.currentlength;i++)
    {
        if (c_seed[i].head != -1)
        {
            outfile<<"score:"<<c_seed[i].score<<endl;
            outfile<<"longseq : "<<"<--"<<c_seed[i].ss<<"            "<<c_seed[i].se<<"-->"<<endl;
            
            outfile<<"          "<<c_seed[i].sexpanded<<endl;
            outfile<<"shortseq: "<<"<--"<<c_seed[i].ts<<"            "<<c_seed[i].te<<"-->"<<endl;
      
            outfile<<"          "<<c_seed[i].texpanded<<endl;
            outfile<<endl;
            outfile<<endl;
        }
    }
    outfile<<"<---------------------------------------------------------------------------------------------->"<<endl;
    outfile.close();
}
