#pragma once
//Test
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
	const static int MAX_RECORDS = 100000;
	const static int BLOCK_SIZE = 4;
	const static int BLOCK_FILL = 3;
	int NumRecords = 0;
	int NumBlocks = 0;
	int RecordLength = 81;
	int BlockSpacerLength = 4;
	int BlockLength = (BLOCK_SIZE * RecordLength);
	int NumFields;
	string ListName;
	ofstream Index;
	ofstream List;
	ifstream Input;

	int IndexKey[MAX_RECORDS] = { 0 };
	int IndexAddress[MAX_RECORDS] = { 0 };

	struct Field
	{
		string FieldName;
		unsigned int StartPosition;
		unsigned int StopPosition;
		string type;
	};

	struct Record
	{
		unsigned int Zip;
		string Place;
		string State;
		string County;
		float Latitude;
		float Longitude;
	};

	struct Block
	{
		Record Records[BLOCK_SIZE];
	};



	Block GetBlock(int ByteOffset)
	{
		Block GetBlock;
		if (List.is_open())
		{
			List.close();
		}

		if (Input.is_open())
		{
			Input.close();
		}

		string temp;
		char byte;
		Input.open("TestList.bin", ios::binary);
		Input.seekg(ByteOffset);

		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			Input.read(reinterpret_cast<char*>(&GetBlock.Records[i].Zip), sizeof(GetBlock.Records[i].Zip));
			temp = "";
			for (int j = 0; j < 31; j++)
			{
				Input.read(&byte, 1);
				temp = temp + byte;
			}
			GetBlock.Records[i].Place = temp;

			temp = "";
			for (int j = 0; j < 2; j++)
			{
				Input.read(&byte, 1);
				temp = temp + byte;
			}
			GetBlock.Records[i].State = temp;

			temp = "";
			for (int j = 0; j < 36; j++)
			{
				Input.read(&byte, 1);
				temp = temp + byte;
			}
			GetBlock.Records[i].County = temp;


			Input.read(reinterpret_cast<char*>(&GetBlock.Records[i].Latitude), sizeof(GetBlock.Records[i].Latitude));
			Input.read(reinterpret_cast<char*>(&GetBlock.Records[i].Longitude), sizeof(GetBlock.Records[i].Latitude));
		}
		cout << "shit";
		return GetBlock;
	}


	Field Fields[MAX_FIELDS];

public:

	SequenceSet()
	{
		//Index.open("TestIndex");
		//List.open("TestList");
	}

	// Populates the Sequence Set
	void PopulateSequenceSet(string input);

	// Searches through the Sequence Set for a record
	Record SearchSequenceSet(string item);
	

}; //end

//#include "SequenceSet.cpp"

#endif