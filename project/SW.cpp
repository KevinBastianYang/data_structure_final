//SW.cpp//Including two functions: SW() and SW_expand()
//1. SW()
//   函数SW将seedStorage中的seed（坐标）拿出，在两端分别进行
//   Smith-Waterman算法扩展
//2. SW_expand()
//   函数SW_expand分别从seed两边开始与shortseq的两端扩展进行Smith-Waterman算法
//   序列比对，一直比对到打分阈值，每次扩展后将扩展后的信息保存入c_seed数组中
//3. 进行Smith-Waterman算法扩展时利用到了工具数据结构栈
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include<string.h>
#include<cmath>
#include"shortReadMapping_class.h"
using namespace std;

#define TABLE_LENGTH 10      //SWtable边长
#define StackSize 1000	//向右下角延伸，回溯的时候用到了栈


//-------------------------------------------------------
//   函数SW将seedStorage中的seed（坐标）拿出，在两端分别进行
//   Smith-Waterman算法扩展
void shortReadMapping::SW()
{
	int i;
	seqPosition tmp;
	/*margin left_margin;
	margin right_margin;*/
	seed singleseed;


	singleseed.next=-1;	//默认为false
	singleseed.head=-1;
	singleseed.score=0;	//最终得分
	singleseed.texpanded="";//扩展过后的s
	singleseed.sexpanded="";//扩展过后的t
	singleseed.ss=0;
	singleseed.se=0;
	singleseed.ts=0;
	singleseed.te=0;

	c_seed = new seed[seedStorage.currentlength];
	for(i = 0; i < seedStorage.currentlength; i++)
	{
		tmp = seedStorage.visit(i); //获取单链表中第i个节点的seed及shortseq坐标信息
		singleseed = SW_expand(tmp);
		c_seed[i] = singleseed;
	}
}


struct Seq{
	string seqs;
	string seqt;
};

//工具数据结构栈的顺序实现
class Stack
{
	public:
		Stack();
		~Stack();
		bool IsEmpty() {return Top==0;};
		bool IsFull() {return Top==StackSize;};
		void Push(Seq Item);
		Seq Pop();
		void PrintPath();
	private:
		Seq Data[StackSize];
		unsigned int Top;
		int Size;
};

Stack::Stack()
{
	Top=0;
	Size=StackSize;
};
Stack::~Stack(){};
void Stack::Push(Seq Item)		//进栈
{
	if (IsFull()) cout<<"Stack is full"<<endl;
	else
	{
	        Top+=1;
	        Data[Top]=Item;
	}
};
Seq Stack::Pop()		//出栈
{
	if (IsEmpty()) {
		cout<<"Stack is empty"<<endl;
		return Data[0];}
	else
	{
	        return Data[Top--];
	}
};

struct Join{		//最后返回的几个值
		int serialNumber;	//第几个序列
		bool tail;	//是否是tail
		int toSeed;	//后面连接的是几号seed
		int score;	//打分
		string sexpanded;		//扩展后的s序列
		string texpanded;		//扩展后的t序列
		int ss;//s start坐标
		int se;//s end坐标
		int ts;//t start坐标
		int te;//t end坐标
};



shortReadMapping::seed shortReadMapping::SW_expand(seqPosition tmp)
{
	//设置gap open等罚分。之后可改为用户设置,但是还是有一个默认值。

	go=-1;	//gap open penalty
	ge=-1;//gap extension penalty
	match=1;//match 得分
	mismatch=0;//mismatch得分

	//读入长序列、短序列以及起始位置
	//string s="AGCTAACCCAPCACACAACGAGACAACAGTTT";  //shortseq
	//string t="ACTACACTACTATCATCTACTAGCATCGATCGACTGCTGACTAGCATCGATCGACTAGCTACGATCGACACACTACTATCATCTACTAGCATCGATCGACTGCTGACTAGCATCGATCGACTAGCTACGATCGACTGATCGACGATGCTACGACTATCGAGCATCGA";
	int shortseqStart = tmp.shortseqStart;
	int shortseqEnd = tmp.shortseqEnd;
	int seedstart = tmp.seedStart;
	int seedend = tmp.seedEnd;

	struct Matrix{
		int score; //存储这一格的分数
		int source;//标明这个得分的来源。其中1来自左上角，2来自上方，3来自左方
	};

	Matrix mat[TABLE_LENGTH][TABLE_LENGTH];
	memset(mat,0,sizeof(mat));
	int ls = shortseq_length;
	int lt = longseq_length;

	//向右下角延伸
	int maxScore1=0;
	int maxi1=0;
	int maxj1=0;//indicating the location where the max score is obtained
	int a=0;

	for (int a=0; (maxScore1>=TOLERANT_SCORE)&&(a<TABLE_LENGTH) && (a+shortseqEnd<=ls) && (a+seedend<=lt);a++)
		{
			//初始化第一行第一列
			for (int i=0; i<=a; i++)
			{
						if (i==0)
						{
						mat[i][a].score=0;//第一行等于零
						mat[i][a].source=3;

						mat[a][i].score=(-1)*a;//第一列为0，-1，-2，-3……
						mat[a][i].source=2;
						}

					//打分
						else
						{
						int score1;
						int score2;
						int score3;
						//The ath column
						if (shortseq[shortseqEnd+i]==longseq[seedend+a]){//判断从左上角来的
							score1=mat[i-1][a-1].score+match;
						}
						else{
							score1=mat[i-1][a-1].score+mismatch;
						}
						if (mat[i-1][a].source==2){//判断从上面来的
							score2=mat[i-1][a].score+ge;//gap extension
						}
						else{
							score2=mat[i-1][a].score+go;//gap open
						}
						if (mat[i][a-1].source==3){//判断从左边来的
							score3=mat[i][a-1].score+ge;
						}
						else{
							score3=mat[i][a-1].score+go;
						}
						//存储三个得分中的最大score以及source
						if (score1>=score2){
							if (score1>=score3){
							mat[i][a].score=score1;
							mat[i][a].source=1;
							}
							else{
								mat[i][a].score=score3;
								mat[i][a].source=3;
							}
						}
						else{
							if (score2>=score3){
							mat[i][a].score=score2;
							mat[i][a].source=2;
							}
							else{
								mat[i][a].score=score3;
								mat[i][a].source=3;
							}
						}

						//The ath row
						if (shortseq[shortseqEnd+a]==longseq[seedend+i]){//判断从左上角来的
							score1=mat[a-1][i-1].score+match;
						}
						else{
							score1=mat[a-1][i-1].score+mismatch;
						}
						if (mat[a-1][i].source==2){//判断从上面来的
							score2=mat[a-1][i].score+ge;//gap extension
						}
						else{
							score2=mat[a-1][i].score+go;//gap open
						}
						if (mat[a][i-1].source==3){//判断从左边来的
							score3=mat[a][i-1].score+ge;
						}
						else{
							score3=mat[a][i-1].score+go;
						}
						//存储三个得分中的最大score以及source
						if (score1>=score2){
							if (score1>=score3){
							mat[a][i].score=score1;
							mat[a][i].source=1;
							}
							else{
								mat[a][i].score=score3;
								mat[a][i].source=3;
							}
						}
						else{
							if (score2>=score3){
							mat[a][i].score=score2;
							mat[a][i].source=2;
							}
							else{
								mat[a][i].score=score3;
								mat[a][i].source=3;
							}
						}
					}
			}

			//在第a行以及第a列中找出最大score以及source，作为最终得分
			for (int m=0; m<=a; m++){
				if (mat[a][m].score>maxScore1){
					maxScore1=mat[a][m].score;
					maxi1=a;
					maxj1=m;
				}
				if (mat[m][a].score>maxScore1){
					maxScore1=mat[m][a].score;
					maxi1=m;
					maxj1=a;
				}
		}
	}


	//向左上角延伸，同上，只有第252 293行有修改
	int maxScore2=0;
	int maxi2=0;
	int maxj2=0;//indicating the location where the max score is obtained

	for (int a=0; (maxScore2>=TOLERANT_SCORE)&& (a<TABLE_LENGTH) && (a<=shortseqStart) && (a<=seedstart);a++)
		{
			//初始化第一行第一列
			for (int i=0; i<=a; i++)
			{
						if (i==0){
						mat[i][a].score=0;//第一行等于零
						mat[i][a].source=3;

						mat[a][i].score=(-1)*a;//第一列为0，-1，-2，-3……
						mat[a][i].source=2;
					}

					//打分
					else{
						int score1;
						int score2;
						int score3;
						//The ath column
						if (shortseq[shortseqEnd-i]==longseq[seedend-a]){//判断从左上角来的
							score1=mat[i-1][a-1].score+match;
						}
						else{
							score1=mat[i-1][a-1].score+mismatch;
						}
						if (mat[i-1][a].source==2){//判断从上面来的
							score2=mat[i-1][a].score+ge;//gap extension
						}
						else{
							score2=mat[i-1][a].score+go;//gap open
						}
						if (mat[i][a-1].source==3){//判断从左边来的
							score3=mat[i][a-1].score+ge;
						}
						else{
							score3=mat[i][a-1].score+go;
						}
						//存储三个得分中的最大score以及source
						if (score1>=score2){
							if (score1>=score3){
							mat[i][a].score=score1;
							mat[i][a].source=1;
							}
							else{
								mat[i][a].score=score3;
								mat[i][a].source=3;
							}
						}
						else{
							if (score2>=score3){
							mat[i][a].score=score2;
							mat[i][a].source=2;
							}
							else{
								mat[i][a].score=score3;
								mat[i][a].source=3;
							}
						}

						//The ath row
						if (shortseq[shortseqEnd-a]==longseq[seedend-i]){//判断从左上角来的
							score1=mat[a-1][i-1].score+match;
						}
						else{
							score1=mat[a-1][i-1].score+mismatch;
						}
						if (mat[a-1][i].source==2){//判断从上面来的
							score2=mat[a-1][i].score+ge;//gap extension
						}
						else{
							score2=mat[a-1][i].score+go;//gap open
						}
						if (mat[a][i-1].source==3){//判断从左边来的
							score3=mat[a][i-1].score+ge;
						}
						else{
							score3=mat[a][i-1].score+go;
						}
						//存储三个得分中的最大score以及source
						if (score1>=score2){
							if (score1>=score3){
							mat[a][i].score=score1;
							mat[a][i].source=1;
							}
							else{
								mat[a][i].score=score3;
								mat[a][i].source=3;
							}
						}
						else{
							if (score2>=score3){
							mat[a][i].score=score2;
							mat[a][i].source=2;
							}
							else{
								mat[a][i].score=score3;
								mat[a][i].source=3;
							}
						}
					}
			}

			//在第a行以及第a列中找出最大score以及source，作为最终得分
			for (int m=0; m<=a; m++){
				if (mat[a][m].score>maxScore1){
					maxScore1=mat[a][m].score;
					maxi1=a;
					maxj1=m;
				}
				if (mat[m][a].score>maxScore1){
					maxScore1=mat[m][a].score;
					maxi1=m;
					maxj1=a;
				}
		}
	}


	//Traceback 1（右下角延伸的）

	string rs1; 	//用于存储result s1
	string rt1;	//用于存储result t1
	int ns1=0;	//用于计算se
	int nt1=0;//用于计算te
	Stack Stack;
	Seq CurrentST;
	CurrentST.seqs=shortseq[shortseqEnd+maxi1];
	CurrentST.seqt=longseq[seedend+maxj1];
	Stack.Push(CurrentST);

	//获得每一个字符
	while (maxi1>=0 &&maxj1>=0){
		if (mat[maxi1][maxj1].source==2){
			maxi1=maxi1-1;
			CurrentST.seqs=shortseq[shortseqEnd+maxi1];
			CurrentST.seqt="-";
			ns1++;
		}
		else if (mat[maxi1][maxj1].source==3){
			maxj1=maxj1-1;
			CurrentST.seqs="-";
			CurrentST.seqt=longseq/*shortseq*/[seedend+maxj1];
			nt1++;
		}
		else if (mat[maxi1][maxj1].source==1){
			maxi1=maxi1-1;
			maxj1=maxj1-1;
			CurrentST.seqs=shortseq[shortseqEnd+maxi1];
			CurrentST.seqt=longseq[seedend+maxj1];
			ns1++;
			nt1++;
		}

		Stack.Push(CurrentST);
	}
	//将字符连接成串
	while (!Stack.IsEmpty())
	{
	    CurrentST=Stack.Pop();
	    rs1+=CurrentST.seqs;
	    rt1+=CurrentST.seqt;
	}


	//Traceback 2
	string rs2;	//result s2
	string rt2;//result t2
	int ns2=0;	//用于计算ss
	int nt2=0;//用于计算ts
	rs2=shortseq[shortseqStart-maxi2];
	rt2=longseq[seedstart-maxj2];
	//获得每一个字符
	while (maxi2>=0 &&maxj2>=0){
		if (mat[maxi2][maxj2].source==2){
			maxi2=maxi2-1;
			rs2+=shortseq[shortseqStart-maxi2];
			rt2+="-";
			ns2++;
		}
		else if (mat[maxi2][maxj2].source==3){
			maxj2=maxj2-1;
			rs2+="-";
			rt2+=longseq[seedstart-maxj2];
			nt2++;
		}
		else if (mat[maxi2][maxj2].source==1){
			maxi2=maxi2-1;
			maxj2=maxj2-1;
			rs2+=shortseq[shortseqStart-maxi2];
			rt2+=longseq[seedstart-maxj2];
			ns2++;
			nt2++;
		}
		else{cout<<"Error!!!!!!"<<endl;}
	}

	//扩展后的信息保存入c_seed数组中
	int count=1;

	seed singleseed;
	singleseed.next=-1;	//默认为false
	singleseed.head=-1;
	singleseed.score=(seedend-seedstart)*match+maxScore1+maxScore2;	//最终得分
	singleseed.texpanded=rs1+shortseq.substr(shortseqStart,(shortseqEnd-shortseqStart))+rs2;//扩展过后的s
	singleseed.sexpanded=rt1+longseq.substr(seedstart, (seedend-seedstart))+rt2;//扩展过后的t
	singleseed.ss=seedstart-nt2;
	singleseed.se=seedend+nt1;
	singleseed.ts=shortseqStart-ns2;
	singleseed.te=shortseqEnd+ns1;

/*    singleseed.next=-1;	//默认为false
	singleseed.head=-1;
	singleseed.score=0;	//最终得分
	singleseed.texpanded="";//扩展过后的s
	singleseed.sexpanded="";//扩展过后的t
	singleseed.ss=0;
	singleseed.se=0;
	singleseed.ts=0;
	singleseed.te=0;*/
	return singleseed;
}//The end.
//----------------------------------------------------------

