﻿SRM
============
SRM(Short Read Mapping) is a software designed to align short sequences back to the genome reference, returning the mapping sequences, position and the mapping score.

Getting started
--------------
### Compile the software
```shell
cd project
g++ main_ShortReadMapping.cpp -o main_ShortReadMapping
./main_ShortReadMapping
```
### Input the reference genome file as requested
* reference example: project/1.fna is part of the complete genome of Klebsiella pneumoniae

### Input the short query sequences as requested
* query example: project/2.fna is certain short sequences of Klebsiella pneumoniae

### Choose the Smith-Waterman penalty score
Three choices: -5, -10, -15

### Run the program
The output will be SRM.txt under the same directory, including the matched sequences, position, and the mapping score.

Data structures and Algorithms
----------------
* Single linked list: seedStorage is single linked list used to store seed

* Stack: Used to trace back in Smith-Waterman alignment

* KMP algorithm: Used to accurately map the seed. Time complexity: O(m+n)

* Dynamic programming: Used in Smith-Waterman algorithm

* Smith-Waterman algorithm: Dynamic optimal local alignment 

* Selection sort: Used in linking the prolonged seeds. Merits are less consuming of space and simple.

* Disjoint Sets: Used in linking the prolonged seeds.



【A.基本信息】
程序名称：ShortReadMapping
开发人员：生物信息与统计系—— 魏杨蓁 郑光辉 杨俊晨 夏云天 王如凡
程序功能：在长序列中寻找与给定短序列匹配（mapping)的局部片段及位置，并给出Smith-Waterman相似性得分
程序用途：生物序列局部比对；寻找局部相似片段；基因识别与定位；基因组转录本分析；
          蛋白质结构与功能预测；系统发生树的构建与物种进化关系分析比较；
          寻找序列上的motif；基因组表达水平研究；寻找基因组保守序列；
          二代测序技术（Next Generation Sequencing）中短读序拼接与组装

		  
【B.使用步骤】
1.进入project文件夹，编译运行程序main_ShortReadMapping.cpp;
2.(1)按照提示输入长序列fasta格式的文件名,例 1.fna（见于文件夹project下的1.fna示例，此为要测序的一种肺炎克雷伯菌
     的全基因组序列（假设还未测序），大小为1MB)；
  (2)按照提示输入短序列fasta格式的文件名，例 2.fna （见于文件夹project下的2.fna实例，此为肺炎克雷伯菌的一段保守序列）
  (3)选择Smith-Waterman罚分值，有3个选项，1.-5，2.-10，3.-15，通过输入1 2 或3进行选择。
3.运行程序，得到匹配结果（输出文件名为SRM.txt,在同一文件夹下）（输出结果包括序列，位置信息以及mapping的打分）。


【C.数据结构与算法思想】
单链表：储存seed的seedStorage为单链表。为了避免插入和删除的线性开销，我们需要允许表可以不连续存储，否则表的部分或全部
      需要整体移动。链表由一系列不必在内存中相连的结构组成。每一个结构均含有表元素和
      指向包含该元素后继元的结构的指针。

栈：在Smith-Waterman序列比对过程中用到了栈来进行回溯。

KMP算法：在利用seed作为模式串去寻找短序列中精确匹配时用到了KMP算法，时间复杂度为O（m+n)。

动态规划算法：在Smith-Waterman算法中用到，把多阶段过程转化为一系列单阶段问题，利用各阶段之间的关系，逐个求解。

Smith-Waterman算法：寻找序列最优相似比较的算法。先用迭代方法计算出两个序列的所有可能
      相似性比较的分值，然后通过动态规划的方法回溯寻找最优相似性比较

直接选择排序： 在将延长后的seed连接过程中用到了直接选择排序，
      该方法的优点是简单，减少了搬移的次数，而且占用的额外空间仅为O(1)

并查集：在在将延长后的seed连接过程中用到了并查集，并查集是一种树型的数据结构，用于处理一些不相交集合
     （Disjoint Sets）的合并及查询问题

		 
		 
【D.研究背景与目标问题】
生物信息学中，对各种生物大分子序列进行分析是一件非常基本的工作。从序列的片段测定，
拼接，基因的表达分析，到RNA和蛋白质的结构功能预测，物种亲缘树的构建都需要进行生物
分子序列相似性的比较。

在遗传物质长期的演化过程中，原本相同的DNA序列由于其中一条序列缺失了几个片段，或者
增加了几个片段，或某段子序列发生了位置的变化等，从而导致他们发生了不同，这两条序列
不一定能进行精确的匹配，但是他们有一定的相似度。对于这种情况，生物学家提出了用记分
函数来评定序列相似性。

在寻找序列最优相似比较的算法中有两种算法使用最为广泛：BLAST算法和Smith-Waterman算法，
BLAST算法的运行速度要比Smith-Waterman算法快，但是Smith-Waterman算法要比BLAST算法更为
精确。Smith-Waterman算法先用迭代方法计算出两个序列的所有相似性比较的分值，然后通过
动态规划的方法回溯寻找最优相似性比较。

Smith-Watermen算法虽然有着较好的比较结果，但是对于长度为n和m的两个待比较序列，该算法
的时间复杂度有O(mn)，这个时间复杂度对于序列匹配来说代价太大。

在生物信息学研究中，我们常常需要将某个短序列与长序列进行比较，或者在序列数据库中搜索
与短序列匹配的局部片段。通过这种方式我们可以识别基因位点或者motif位置，也可以帮助研究
物种亲缘关系远近，对于生物学研究有着重要意义。如果直接用Smith-Waterman算法将长序列和
短序列进行匹配，时间复杂度太大，耗费较多。

因此我们想出新的改进方法，旨在提高现有序列比对方法的速度和准确性。

【E.设计思路】
我们提出的改进方法基本思想为：首先将长序列切分为几bp的短片段，然后将待查询的短序列和
分割得到的所有短序列联配，找出精确匹配的“种子”，以“种子”为中心，使用动态规划法向两边
扩展成更长的联配。然后将扩展出来的联配之间的距离算出来，将高于连接距离阈值的联配进行
连接，使之成为更长的联配，分值也会更高。最后在一定精度范围内选取符合条件的联配按序输
出。得分最高的联配序列就是最优比对序列。

步骤如下：
1.文件输入：读入文件，存储长序列和短序列（代码见于getParameter.cpp，文件内有更详细的注释及说明)
2.序列分割及模式匹配：在长序列中切割，将每个切割的小片段与待查询的短序列进行比对，找出配对的seed（代码见于KMP.cpp，文件内有更详细的注释及说明）
3.seed扩展：将seed的两端分别用动态规划法扩展，直到分值降低到小于阈值（代码见于SW.cpp，文件内有更详细的注释及说明）
4.计算距离：计算两两联配之间的距离，并排序，选择小于阈值的（代码见于join.cpp，文件内有更详细的注释及说明）
5.连接联配：将距离小于阈值的联配连接，使之成为更长的联配(代码见于join.cpp，文件内有更详细的注释及说明）
6.文件输出：将联配后的信息进行输出，输出到SRM.txt文件中，信息包括序列、坐标及mapping打分(代码见于output.cpp，文件内有更详细的注释及说明）
