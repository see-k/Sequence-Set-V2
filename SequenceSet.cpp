#include <iostream>
#include <fstream>
#include "pch.h"
#include "SequenceSet.h"
#include <string>

using namespace std;

//SequenceSet::SequenceSet()
//{
//	SequenceSet.Index.open("");
//}

// Private Members
void SequenceSet::PrintRecord(Record ToPrint)
{
	cout << ToPrint.Zip << " ";
	cout << ToPrint.Place << " ";
	cout << ToPrint.State << " ";
	cout << ToPrint.County << " ";
	cout << ToPrint.Latitude << " ";
	cout << ToPrint.Longitude << " ";
	cout << endl;
}//endPrintRecord

int SequenceSet::binarySearch(int arr[], int l, int r, int x)
{
	int mid = l + (r - l) / 2;
	if (r >= l) {
		// If the element is present at the middle itself 
		if (arr[mid] == x)
			return mid;

		// If element is smaller than mid, then it can only be present in left subarray 
		if (arr[mid] > x)
			return binarySearch(arr, l, mid - 1, x);

		// Else the element can only be present in right subarray 
		return binarySearch(arr, mid + 1, r, x);
	}

	// We reach here when element is not present in array 
	return (mid - 1);
}//endbinarySearch

int SequenceSet::Search(int Zip)
{
	int Length = NumBlocks;
	int Position = binarySearch(IndexKey, IndexKey[0], Length, Zip);
	int Offset = IndexAddress[Position];
	return Offset;
}//endSearch

SequenceSet::Block SequenceSet::GetBlock(int ByteOffset)
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
	return GetBlock;
}//endGetBlock

// Public Members
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
			cout << "NumFields failed";
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
			cout << "Field parsing failed";
		}

	}


	//------CREATE HEADER RECORD
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
					List << "F";
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

	return;
}//endPopulate

void SequenceSet::ZipSearch(int Zipcode)
{
	int Offset = Search(Zipcode);
	Block FoundBlock = GetBlock(Offset);
	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		if (FoundBlock.Records[i].Zip == Zipcode)
		{
			cout << "The State for the Zip Code " << Zipcode << " is " << FoundBlock.Records[i].State << endl;
			cout << "The Place Name for the Zip Code " << Zipcode << " is " << FoundBlock.Records[i].Place << endl;
		}
	}
}//endZipSearch

void SequenceSet::StateSearch(string State)
{
	bool First = true;
	Record NorthernMost;
	Record SouthernMost;
	Record WesternMost;
	Record EasternMost;
	for (int i = 0; i < NumBlocks; i++)
	{
		Block FBlock = GetBlock(67 + (i * 328));
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			if (FBlock.Records[j].State == State)
			{
				if (First)
				{
					EasternMost = FBlock.Records[j];
					SouthernMost = FBlock.Records[j];
					First = false;
				}
				if (FBlock.Records[j].Latitude > NorthernMost.Latitude)
				{
					NorthernMost = FBlock.Records[j];
				}
				if (FBlock.Records[j].Latitude < SouthernMost.Latitude)
				{
					if (FBlock.Records[j].Latitude != 0)
					{
						SouthernMost = FBlock.Records[j];
					}
				}
				if (FBlock.Records[j].Longitude > EasternMost.Longitude)
				{
					if (FBlock.Records[j].Latitude != 0)
					{
						EasternMost = FBlock.Records[j];
					}
				}
				if (FBlock.Records[j].Longitude < WesternMost.Longitude)
				{
					WesternMost = FBlock.Records[j];
				}
			}
		}
	}
	cout << "The Northernmost Zip Code in " << State << " is " << NorthernMost.Zip << endl;
	cout << "The Southernmost Zip Code in " << State << " is " << SouthernMost.Zip << endl;
	cout << "The WesternMost Zip Code in " << State << " is " << WesternMost.Zip << endl;
	cout << "The EasternMost Zip Code in " << State << " is " << EasternMost.Zip << endl;
}//endStateSearch

// @Function: Error check
// @Param/input: is_state
// @Precondition: is_state is constant
// @Postcondition: N/A
// @output: boolean
bool SequenceSet::errorCheck_state(const string& is_state)
{
	bool flag;
	return  flag = (is_state.length() > 2) ? true : false;

}

// @Function: Error check
// @Param/input: is_zip
// @Precondition: is_zip is constant
// @Postcondition: N/A
// @output: boolean
bool SequenceSet::errorCheck_zip(const string& is_zip)
{
	bool flag = false;
	for (int i = 0; i < is_zip.length() && !flag; i++)
	{
		if (!isdigit(is_zip[i]))
			flag = true;
	}
	return flag;
}