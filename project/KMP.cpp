//KMP.cpp
//Including three functions: cal_next, KMP and split
//1. split:
//   将longseq分割为SPLIT_LENGTHbp的片段并通过KMP算法与shortseq匹配
//2. KMP:
//   模式匹配算法：将split函数所切割的片段作为模式串与shortseq匹配
//   匹配成功就将seed的坐标和shortseq中对应的坐标存入seedStorage中
//3. cal_next:
//   计算模式串失配时所返回位置数组
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include <string.h>
#include<cmath>
#include"shortReadMapping_class.h"
using namespace std;

void shortReadMapping::cal_next(const char * p, int next[]) //计算模式串失配时所返回位置数组
{
    int pLen = SPLIT_LENGTH;
    next[0] = -1;
    int k = -1;
    int j = 0;
    while (j < pLen - 1)
    {
        //匹配时
        if (k == -1 || p[j] == p[k])
        {
            ++j;
            ++k;
            if (p[j] != p[k])
                next[j] = k;  
            else next[j] = next[k];
        }
        else k = next[k];  //失配就不断迭代
    }
}

//   模式匹配算法：将split函数所切割的片段作为模式串与shortseq匹配
//   匹配成功就将seed的坐标和shortseq中对应的坐标存入seedStorage中
int shortReadMapping::KMP(int left_begin,string str,const char ptr[])
{
    int i = 0;
    int j = 0;

    int sLen = str.length(); //shortseq长度
    int pLen = SPLIT_LENGTH;  //模式串长度
    int * next = new int[pLen];
    seqPosition tmp;

    cal_next(ptr,next); //计算next数组

    while(true)
    {
        while (i < sLen && j < pLen)
        {
            //如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++
            if ((j == -1) || (str[i] == ptr[j]))
            {
                i++;
                j++;
            }
            else
            {
                //如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]
                //next[j]即为j所对应的next值
                j = next[j];
            }
        }

        if (j == pLen && i < sLen)  //如果匹配成功就将seed的坐标和shortseq中对应的坐标存入seedStorage中
         {
            tmp.seedStart = left_begin;
            tmp.seedEnd = left_begin + SPLIT_LENGTH - 1;
            tmp.shortseqStart = i - SPLIT_LENGTH + 1;
            tmp.shortseqEnd = i;
            seedStorage.insert(tmp);
            j = 0;  //继续寻找下一处匹配
            i++;
        }
        else  return 1;
    }
}

//   将longseq分割为SPLIT_LENGTHbp的片段并通过KMP算法与shortseq匹配
void shortReadMapping::split()
{
    char tmp[SPLIT_LENGTH];
    int i,j,k;
    for(i = 0; i <= longseq_length - SPLIT_LENGTH; i += SPLIT_LENGTH)
    {
        for(j = 0,k = i; j < SPLIT_LENGTH;j++,k++)  tmp[j] = longseq[k];
        //tmp[SPLIT_LENGTH] = '\0';
        KMP(i,shortseq,tmp);
    }
}
//------------------------------------------------------------------------