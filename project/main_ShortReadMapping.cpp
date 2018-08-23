//算法与数据结构大作业：ShortReadMapping
//小组成员：魏杨蓁、杨俊晨、夏云天、郑光辉、王如凡
//此cpp文件为主函数
//本程序以shortReadMapping为类封装，初始化及文件读入功能分装到getParameter.cpp文件中；
//切割长序列及字符串模式匹配功能分装到KMP.cpp中；Smith-Waterman序列比对功能分装到SW.cpp中；
//序列连接分装到join.cpp中，文件输出功能分装到output.cpp中,其余涉及到的工具像链表则分装到singlelist.cpp中
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include<string.h>
#include<cmath>
#include"shortReadMapping_class.h"
#include"getParameter.cpp"
#include"singlelist.cpp"
#include"KMP.cpp"
#include"SW.cpp"
#include"join.cpp"
#include"output.cpp"
using namespace std;


int main()
{
    //创建 shortReadMapping类的对象 object
	shortReadMapping object;
	//切割长序列，将片段与短序列模式匹配，有命中则将坐标储存
	object.split();
	//将储存的seed向两端进行Smith-Waterman算法扩展
	object.SW();
	//计算扩展完的seed间的距离
	object.distance_calculate();
	//将符合距离要求的扩展seed连接
	object.join();
	//将连接完的信息进行输出，包括坐标和序列信息
    object.output();
	return 0;
}





