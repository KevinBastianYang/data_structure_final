//shortReadMapping_class.h
//类shortReadMapping的头文件
//类shortReadMapping包裹的主要成员函数及数据成员见于下方具体介绍
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include<string.h>
#include<cmath>
#include"singlelist.h"
using namespace std;
#define MAXLINELENGTH 150     //读fasta格式每行长度上限

#ifndef shortReadMapping_h
#define shortReadMapping_h

//----------------------shortReadMapping---------------------
class shortReadMapping
{
private:
// SeqPosition保存seed在长序列中的起始位置seedStart和seedEnd，
// 以及短序列中的起始位置shortseqStart和shortseqEnd
	struct seqPosition
	{
		int seedStart;
		int seedEnd;
		int shortseqStart;
		int shortseqEnd;
	};

	struct seed_dis
	//存储可以连接的两个seed之间的距离
	{
	    int seed1;
	    int seed2;
	    float dis;
	};

	struct seed
	//存储seed信息
	{
	    int score;//打分
	    string sexpanded;//长序列扩展结果
	    string texpanded;//短序列扩展结果
	    int ss;//长序列开头
	    int se;//长序列结尾
	    int ts;//短序列开头
	    int te;//短序列结尾
	    int head;//连接的前一个
	    int next;//连接的下一个
	};

	int go,ge,match,mismatch;//SWtable
    seed * c_seed;//complete seed
    int join_no,join_threshold;
    seed_dis * join_seed;

    int SPLIT_LENGTH; //对长序列切割时所选择的片段长度
    int SW_threshold;
    int TOLERANT_SCORE; //Smith-Waterman的罚分下限

	string shortseq;  //储存长序列
	string longseq;   //储存短序列

	long int longseq_length;  //长序列长度
	long int shortseq_length;  //短序列长度

	singlelinklist<seqPosition> seedStorage;  //储存seed
public:
//   Initialize the shortReadMapping class and
//   store the long sequence and short sequence
//	 in the string longseq and shortseq seperately.
	shortReadMapping();

//  output():将连接完的信息进行输出，包括坐标和序列信息
	void output();

//  get_longseqfile():Get long sequence file's name
	string get_longseqfile();

//  get_shortseqfile(): Get short sequence file's name
	string get_shortseqfile();

//  get_parameter():Get parameter from the user
	void get_parameter();

//  split():将longseq分割为SPLIT_LENGTHbp的片段并通过KMP算法与shortseq匹配
	void split();

//  KMP():
//  模式匹配算法：将split函数所切割的片段作为模式串与shortseq匹配
//  匹配成功就将seed的坐标和shortseq中对应的坐标存入seedStorage中
	int KMP(int left_begin,string str, const char ptr[]);

//  cal_next():
//  计算模式串失配时所返回位置数组
	void cal_next(const char * str, int *next);

//  SW():
//  函数SW将seedStorage中的seed（坐标）拿出，在两端分别进行
//  Smith-Waterman算法扩展
	void SW();

//  SW_expand()
//  函数SW_expand分别从seed两边开始与shortseq的两端扩展进行Smith-Waterman算法
//  序列比对，一直比对到打分阈值，每次扩展后将扩展后的信息保存入c_seed数组中
	seed SW_expand(seqPosition tmp);

//
	void distance_calculate();

//
	void join();

//析构函数：回收空间
	~shortReadMapping() {delete [] c_seed;delete [] join_seed;}

};

#endif

