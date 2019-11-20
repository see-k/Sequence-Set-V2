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
	List.open("TestList");
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
			Fields[i].StartPosition = stoi(temp, nullptr, 10);

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
		//------CREATE HEADER RECORD???

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

	}//endWhile






	return;
}//endPopulate