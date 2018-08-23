//singlelist.h
//工具数据结构单链表的头文件
//包括插入，查找，删除等基本操作

#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include<string.h>
#include<cmath>
using namespace std;
#define MAXLINELENGTH 150     //读fasta格式每行长度上限

#ifndef linklist_h
#define linklist_h

//---------------------singlelist------------------------------------------------
template <class elemType> //单链表抽象类
class list
{
public:
    virtual void clear() = 0;
    virtual void insert(const elemType &x) = 0;
    virtual void remove(int i) = 0;
    virtual ~list() {};
};

template <class elemType>
class singlelinklist:public list<elemType>  //单链表类的实现
{
friend class shortReadMapping;
private:
    struct node{    //单链表内嵌节点类
        elemType data; //模板数据成员
        node *next;
        node(const elemType &x, node *n = NULL) //节点类构造
        {
            data = x; next = n;
        }
        node():next(NULL){}
        ~node(){}
    };

    node * head; //头节点
    int currentlength; //表长
    node * move(int i) const;
public:
    singlelinklist();
    ~singlelinklist() {clear();delete head;}
    void clear();
    void insert(const elemType &x);
    void remove(int i);
    elemType visit(int i) const;
};
//-----------------------------------------------------------

#endif