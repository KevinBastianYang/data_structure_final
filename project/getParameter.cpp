//getparameter.cpp
//Including Four functions: shortReadMapping constructor, get_longseqfile(), get_shortseqfile(), and get_parameter() 
//1. get_longseqfile():
//   Get long sequence file's name
//2. get_shortseqfile():
//   Get short sequence file's name
//3. get_parameter():
//   Get parameter from the user
//4. Constructor:
//   Initialize the shortReadMapping class and 
//   store the long sequence and short sequence
//	 in the string longseq and shortseq seperately.
#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<cstdio>
#include<string.h>
#include<cmath>
#include"shortReadMapping_class.h"

using namespace std;

#define MAXLINELENGTH 150     //Maximum length of input file's line lenth

shortReadMapping::shortReadMapping()
{
	ifstream infile;
	infile.open(get_longseqfile().c_str());  //Get long sequence file's name
	while(!infile.is_open()) 
	{
		cout<<" The file doesn't exist or can't be opened, please try again!"<<endl; 
		infile.open(get_longseqfile().c_str());
	}

	ifstream shortfile;
	shortfile.open(get_shortseqfile().c_str());  //   Get short sequence file's name
    while(!shortfile.is_open()) 
    {
    	cout<<" The file doesn't exist or can't be opened, please try again!"<<endl; 
    	shortfile.open(get_longseqfile().c_str());
    }


	char line[MAXLINELENGTH];
	int i,k = 0;
	longseq="";  //Initialize the string longseq and shortseq
	shortseq="";

	get_parameter(); //Get parameter from the user

	//store the long sequence in the string longseq
	while(infile.getline(line,sizeof(line)))
	{
		if(line[0] == '>') continue;
		while(line[k] != '\0')
		{
			longseq+=line[k];
			k++;
		}
		k = 0;
	}
	longseq_length = longseq.length();   //longseq's length

	k = 0;

	//store the short sequence in the string shortseq
	while(shortfile.getline(line,sizeof(line)))
	{
		if(line[0] == '>') continue;
		while(line[k] != '\0')
		{
			shortseq += line[k];
			k++;
		}
		k = 0;
	}

	shortseq_length = shortseq.length(); //shortseq's length
}

string shortReadMapping::get_longseqfile()//   Get long sequence file's name
{
	string input_long;
	cout<<"Please input a fasta file  as the long sequence: "<<endl;
	cin >> input_long;
	return input_long;
}

string shortReadMapping::get_shortseqfile()//Get short sequence file's name
{
	string input_short;
	cout<<"Please input a fasta file  as the short sequence: "<<endl;
	cin >> input_short;
	return input_short;
}

void shortReadMapping::get_parameter()  //   Get parameter from the user
{
	int seed_option,SW_threshold;
	/*do
	{
	cout<<"Please choose the option below of the seed length that you want to set: "<<endl;
	cout<<"1.  8bp                       2.  10bp                         3.  12bp"<<endl;
	cout<<":";
	cin>>seed_option;
	} while(seed_option != 1 && seed_option != 2 && seed_option != 3);
	switch(seed_option)
	{
		case 1:
			SPLIT_LENGTH = 9;
			break;
		case 2:
			SPLIT_LENGTH = 10;
			break;
		case 3:
			SPLIT_LENGTH = 12;
			break;
	}*/
	SPLIT_LENGTH = 10;

	do
	{
		cout<<"Please choose the option below of the Smith-Waterman penalty score: "<<endl;
		cout<<"1.  -5                    2.  -10                           3.  -15 "<<endl;
		cout<<":";
		cin>>SW_threshold;
	}  while(SW_threshold != 1 && SW_threshold != 2 && SW_threshold != 3);
	switch(SW_threshold)
	{
		case 1:
			TOLERANT_SCORE = -5;
			break;
		case 2:
			TOLERANT_SCORE = -10;
			break;
		case 3:
			TOLERANT_SCORE = -15;
			break;
	}

	cout<<"The default gap open, gap extension, match and mismatch score is -1, -1, 1 and 0."<<endl;
}




