#include <iostream>
#include <fstream>
#include "pch.h"
#include "SequenceSet.h"

using namespace std;

//SequenceSet::SequenceSet()
//{
//	SequenceSet.Index.open("");
//}

void SequenceSet::PopulateSequenceSet(string input)
{
	string temp;
	int temp_length;
	char ch;
	bool done = false;

	Index.open("TestIndex");
	List.open("TestList.bin", ios::binary);
	Input.open(input);


	//------GET FIELD INFO
	while (!done)
	{
		//get file name
		getline(Input, temp);
		ListName = temp;

		//get num fields
		getline(Input, temp);
		NumFields = temp[temp.length() - 1] - '0';
		if (NumFields > MAX_FIELDS)
		{
			cout << "ya fucked up, NumFields";
		}

		for (int i = 0; i < NumFields; i++)
		{


			Input >> ch;
			while (ch != '|')
			{
				Fields[i].FieldName = Fields[i].FieldName + ch;
				Input >> ch;
			}

			Input >> ch;
			temp = "";
			while (ch != '-')
			{
				temp = temp + ch;
				Input >> ch;
			}
			Fields[i].StartPosition = stoi(temp);

			Input >> ch;
			temp = "";
			while (ch != '|')
			{
				temp = temp + ch;
				Input >> ch;
			}
			Fields[i].StopPosition = stoi(temp, nullptr, 10);

			getline(Input, Fields[i].type);
		}

		getline(Input, temp);
		if (temp == "!!!!!")
		{
			done = true;
		}
		else
		{
			cout << "ya fucked up Field parsing";
		}

	}


	//------CREATE HEADER RECORD???
	float testing = 0;
	unsigned int HeaderLength;
	List << "000";
	for (int i = 0; i < NumFields; i++)
	{

		//List.write((char*)&Fields[i].FieldName, 7);
		List << Fields[i].FieldName << "|";
		if (Fields[i].type == "u_integer")
		{
			List << to_string(sizeof(Fields[i].StartPosition));
		}
		else if (Fields[i].type == "text")
		{
			List << to_string(Fields[i].StopPosition - Fields[i].StartPosition + 1);
		}
		else
		{
			List << to_string(sizeof(testing));
		}
		List << "|";
	}
	HeaderLength = List.tellp();
	List.seekp(0, List.beg);
	List << HeaderLength << "|";

	//------POPULATE RECORDS
	//now that we know the start and stop points for each field we can
	//set up some loops to read in each field and throw that in a big loop that will do ever line
	//just do a getline and parse by index within the string. we can have 1 line from the file in
	//main memory at a time and just slap it into a record and repeat
	//
	//Need to consider the following:
	//-header record creation
	//-block/linked list architecture
	//-how we are writing this to the file
	//char* buffer;
	char buffer[81]; //TODO make dynamic
	List.seekp(HeaderLength);


	unsigned int writeInt;
	float writeFloat;
	//string slices[MAX_FIELDS];
	string snip;
	int RecordCount = 0;
	while (!Input.eof())
	{
		getline(Input, temp);
		for (int i = 0; i < NumFields; i++)
		{
			snip = temp.substr(Fields[i].StartPosition - 1, Fields[i].StopPosition - Fields[i].StartPosition + 1);
			if (i == 0 && RecordCount == 0)
			{
				IndexKey[NumBlocks] = stoi(snip);
				IndexAddress[NumBlocks] = HeaderLength + (NumBlocks * ((81 * 4) + 4));
				Index << snip << "!" << to_string(HeaderLength + (NumBlocks * ((81 * 4) + 4))) << endl;
			}

			//write record
			if (Fields[i].type == "u_integer")
			{
				writeInt = stoi(snip);
				List.write(reinterpret_cast<const char*>(&writeInt), sizeof(writeInt));
			}
			else if (Fields[i].type == "text")
			{
				//List.write(reinterpret_cast<const char*>(&snip), (Fields[i].StopPosition - Fields[i].StartPosition + 1));
				List << snip;
			}
			else
			{
				writeFloat = stof(snip);
				List.write(reinterpret_cast<const char*>(&writeFloat), sizeof(writeFloat));
			}
		}

		NumRecords++;
		RecordCount++;
		if (RecordCount == BLOCK_FILL)
		{
			while (RecordCount < BLOCK_SIZE)
			{
				for (int j = 0; j < 81; j++)
				{
					List << "0";
				}
				RecordCount++;
			}

			//block spacer
			//TODO make configurable via user input, use spacer length and spacer character from user
			List << "!!!!";
			RecordCount = 0;
			NumBlocks++;
		}
	}
	//TestingGetBlock
	Block test;
	test = GetBlock(67);
	//cout << test.Records[1].Zip;


	//Learning binary read/write, just test stuff here
	//unsigned int test = 56303;
	//List.write(reinterpret_cast<const char*>(&test),sizeof(test));
	//test = 0;
	//List.seekp(HeaderLength);
	//List.close();
	//ifstream poop;
	//poop.open("TestList.bin", ios::binary);
	//poop.seekg(HeaderLength);
	//poop.read(reinterpret_cast<char*>(&test), sizeof(test));
	//cout << test;




	return;
}//endPopulate

Record SequenceSet::SearchSequenceSet(string item)
{
	// Get index from index file
	Index.open("TestIndex");

	// Search through index file for "item" to get byte offset
	int ByteOffset;

	// Use byte offset to get the block of the record
	Block FoundBlock = GetBlock(ByteOffset);

}//endSearch