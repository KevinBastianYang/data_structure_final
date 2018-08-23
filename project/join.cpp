//join.cpp
//Including 2 functions: distance_calculate() and join()
//1. distance_calculate():
//   计算两个延长后的seed之间的距离
//2. join ():
//   将距离上限以内的序列连接
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include <string.h>
#include<cmath>
#include"shortReadMapping_class.h"


using namespace std;

//--------------------join-----------------------------
void shortReadMapping::distance_calculate()//计算每两个seed的距离，可以连接的加进join_seed
{
    int i,j,o;
    seed_dis distance[seedStorage.currentlength * seedStorage.currentlength / 2];
    seed_dis temp;
    int distance_length = 0;
    for (i=0;i<seedStorage.currentlength-1;i++)
        for (j=i+1;j<seedStorage.currentlength;j++)
        // 首先辨别出两个seed哪一个在左上，然后计算每两个seed的距离，他们的信息存进distance数组，用于排序
            {
                if ((c_seed[i].se<=c_seed[j].ss)&&(c_seed[i].te<=c_seed[j].ts))
                    {
                        distance[distance_length].dis=sqrt((c_seed[i].se-c_seed[j].ss)*(c_seed[i].se-c_seed[j].ss)-(c_seed[i].te-c_seed[j].ts)*(c_seed[i].te-c_seed[j].ts));
                        distance[distance_length].seed1 = i;
                        distance[distance_length].seed2 = j;
                        distance_length++;
                    }
                if ((c_seed[j].se<=c_seed[i].ss)&&(c_seed[j].te<=c_seed[i].ts))
                    {
                        distance[distance_length].dis=sqrt((c_seed[j].se-c_seed[i].ss)*(c_seed[j].se-c_seed[i].ss)-(c_seed[j].te-c_seed[i].ts)*(c_seed[j].te-c_seed[i].ts));
                        distance[distance_length].seed1 = j;
                        distance[distance_length].seed2 = i;
                        distance_length++;
                    }
            }
    for (i=0;i<distance_length-1;i++)
    for (j=i+1;j<distance_length;j++)
    //直接选择排序
    {
        if (distance[i].dis>distance[j].dis)
        {
            temp = distance[i];
            distance[i] = distance[j];
            distance[j] = temp;
        }
    }
    join_no = 0;
    while (distance[join_no].dis<=join_threshold && i<distance_length) join_no++;// 看看有多少seed之间的距离小于threshold，存入seed_join用于连接
    join_seed = new seed_dis[join_no];
    for (i=0;i<join_no;i++) join_seed[i] = distance[i];

}


void shortReadMapping::join()
{
    int length,score,score1,score2,score3,seed1,seed2,length_s,length_t,i,j,k,o;
    for (i=0;i<join_no;i++)
    {
        seed1 = join_seed[i].seed1;
        seed2 = join_seed[i].seed2;//两个待组合的seed，seed1-seed2
        if ((c_seed[seed1].next == -1)&&(c_seed[seed2].head == -1))//两个seed可以组合
        {
            //将seed中间部分带上头尾
            length_s = c_seed[seed2].ss - c_seed[seed1].se + 1;
            length_t = c_seed[seed2].ts - c_seed[seed1].te + 1;
            int matrix[length_s+1][length_t+1];
            int source[length_s+1][length_t+1];
            memset(source,0,sizeof(source));
            memset(matrix,0,sizeof(matrix));
            char s[length_s];
            char t[length_t];
            for (j=0;j<=length_s;j++) s[j] = longseq[j+c_seed[seed1].ss];
            for (j=0;j<=length_t;j++) t[j] = longseq[j+c_seed[seed1].ts];
            //初始化，-1，-2，，，
            for (j=1;j<=length_s;j++) {matrix[j][0] = -1 * j; source[j][0] = 3;}
            for (k=1;k<=length_t;k++) {matrix[0][k] = -1 * k; source[0][k] = 2;}
            //全局比对
            for (j=0;j<=length_s;j++)
                for (k=0;k<=length_t;k++)
                {
                    //来自左上1
                    if (s[j] == t[k]) score1 = matrix[j][k] + match;
                    else score1 = matrix[j][k] + mismatch;
                    //来自上面2
                    if (source[j][k+1] == 2) score2 = matrix[j][k+1] + ge;
                    else score2 = matrix[j][k+1] + go;
                    //来自左面3
                    if (source[j+1][k] == 3) score3 = matrix[j+1][k] + ge;
                    else score3 = matrix[j+1][k] + go;
                    if ((score1 >= score2)&&(score1 >= score3))
                    {
                        matrix[j+1][k+1] = score1;
                        source[j+1][k+1] = 1;
                    }
                    if ((score2 >= score1)&&(score2 >= score3))
                    {
                        matrix[j+1][k+1] = score2;
                        source[j+1][k+1] = 2;
                    }
                    if ((score3 >= score2)&&(score3 >= score1))
                    {
                        matrix[j+1][k+1] = score3;
                        source[j+1][k+1] = 3;
                    }
                }
            score = matrix[length_s][length_t];//score记录中间部分的分值
            if (length_s > length_t) length = length_s;//length记录匹配部分的长度
            else length = length_t;
            char sexpend[length],texpend[length];//sexpend记录长序列的匹配串,texpend记录短序列的匹配串
            for (o=0;o<length;o++) sexpend[o] = '-';
            sexpend[length-1] = '\0';
            for (o=0;o<length;o++) texpend[o] = '-';
            texpend[length-1] = '\0';
            o = length-1;
            j = length_s;
            k = length_t;
            while (o>=0)
            {
                if (source[j][k] == 1)
                {
                    sexpend[o] = s[j-1];
                    texpend[o] = t[k-1];
                    j = j - 1;
                    k = k - 1;
                    o = o - 1;
                    continue;
                }
                if (source[j][k] == 2)
                {
                    sexpend[o] = '-';
                    texpend[o] = t[k-1];
                    k = k - 1;
                    o = o - 1;
                    continue;
                }
                if (source[j][k] == 3)
                {
                    sexpend[o] = s[j-1];
                    texpend[o] = '-';
                    j = j - 1;
                    o = o - 1;
                    continue;
                }
            }
            c_seed[seed1].next = seed2;
            c_seed[seed2].head = seed1;
            while (c_seed[seed1].head != -1) seed1 = c_seed[seed1].head;
            c_seed[seed1].score = c_seed[seed1].score + score + c_seed[seed2].score;
            c_seed[seed1].sexpanded = c_seed[seed1].sexpanded + sexpend + c_seed[seed2].sexpanded;
            c_seed[seed1].texpanded = c_seed[seed1].texpanded + texpend + c_seed[seed2].texpanded;
            c_seed[seed1].se = c_seed[seed2].se;
            c_seed[seed1].te = c_seed[seed2].te;
        }
    }
}
//----------------------------------------------------------
