//singlelist.cpp
//工具数据结构单链表的实现文件
//包括插入，查找，删除等基本操作
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include <string.h>
#include<cmath>
#include"singlelist.h"

using namespace std;
//-----------------------------------------------------------------------
template <class elemType>
typename singlelinklist<elemType>::node * singlelinklist<elemType>::move(int i) const //找到第i个节点
{
    node * p = head->next;
    while(i--) p = p->next;
    return p;
}

template<class elemType>
elemType singlelinklist<elemType>::visit(int i) const
{
    node * p = move(i);
    return p->data;
}

template<class elemType>
singlelinklist<elemType>::singlelinklist() //构造函数：给头节点空间
{
	head = new node;
	head->next = NULL;
	currentlength = 0;
}

template<class elemType>
void singlelinklist<elemType>::clear() //clear:清空头节点的全部后继节点
{
	node * p = head->next, *q;
    while(p != NULL)
    {
        q = p->next;
        delete p;
        p = q;
    }
    head->next = NULL;
    currentlength = 0;
}

template<class elemType>
void singlelinklist<elemType>::insert(const elemType & x)//在最后一个节点后插入值
{
	node * pos;
	pos = head;
	while(pos->next != NULL) pos = pos->next;
	pos->next = new node(x);
	++currentlength;
}

template<class elemType>
void singlelinklist<elemType>::remove(int i) //删除第i个节点
{
	node * pos, * tmp;
	pos = move(i-1);
	tmp = pos->next;
	pos->next = tmp->next;
	delete tmp;
	--currentlength;
}
//----------------------------------------------------