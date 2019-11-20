#pragma once

#ifndef _SEQUENCE_SET_
#define _SEQUENCE_SET_

#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class SequenceSet
{
private:
	const static int MAX_FIELDS = 7;
	const static int MAX_RECORDS = 30;
	const static int BLOCK_SIZE = 4;
	const static int BLOCKING_FACTOR = 3;
	int NumRecords;
	int NumFields;
	string ListName;
	ofstream Index;
	ofstream List;
	ifstream Input;

	struct Field 
	{
		string FieldName;
		int StartPosition;
		int StopPosition;
		string type;
	};
	
	Field Fields[MAX_FIELDS];

public:
	
	SequenceSet()
	{
		//Index.open("TestIndex");
		//List.open("TestList");
	}

	void PopulateSequenceSet(string input);
	



}; //end

//#include "SequenceSet.cpp"

#endif