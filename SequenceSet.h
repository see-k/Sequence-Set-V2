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
	int NumRecords;
	int NumBlocks;
	int RecordLength;
	int BlockSpacerLength;
	int BlockLength;
	int NumFields;
	string ListName;
	ofstream Index;
	ofstream List;
	ifstream Input;
	



	int IndexKey[MAX_RECORDS] = { 0 };
	int IndexAddress[MAX_RECORDS] = { 0 };
	
	/**
	//  @brief Field structure for storing header record data and other config data, dynamic
	*/
	struct Field 
	{
		 string FieldName;
		unsigned int StartPosition;
		unsigned int StopPosition;
		string type;
	};
	
	/**
	//  @brief Record structure for memory, static, needs some changes to make it work for other input files
	*/
	struct Record
	{
		unsigned int Zip = 0;
		string Place = "";
		string State = "";
		string County = "";
		float Latitude = 0;
		float Longitude = 0;
	};

	/**
	//  @brief array of blocks BLOCK_SIZE in length
	*/
	struct Block
	{
		Record Records[BLOCK_SIZE];
	};

	// Prints all of the information in a record.
	/**
	//	PrintRecord
	//	@brief Function for printing records to std out field by field
	//	@pre Input record fields are properly populated
	//	@post none
	*/
	void PrintRecord(Record ToPrint);

	// Searches recursively for a specific item.
	/**
	//	binarySearch
	//  @brief Basic binary search used for finding records within the index array
	//  @pre Index array must be sorted with no duplicate entries
	//  @post none
	*/
	int binarySearch(int arr[], int l, int r, int x);

	// Returns the offset for a block with a certain zip code.
	/**	Search
	//  @brief Returns the byte offset of a specific zipcode within the blocked data file, 
	//  @pre binarySearch() able to run properly
	//  @post none
	*/
	int Search(int Zip);

	// Returns the offset for a block with a certain zip code.
	/**
	//	GetBlock
	//  @brief Pulls the block at the specified offset within the data file into memory
	//  @pre data file built properly
	//  @post none
	*/
	Block GetBlock(int ByteOffset);

	// Returns the offset for a block with a certain zip code.
	/**
	//  @brief an array of fields for building the header record
	*/
	Field Fields[MAX_FIELDS];

public:
	
	// Constructor
	SequenceSet()
	{
		NumRecords = 0;
		NumBlocks = 0;
		RecordLength = 81;
		BlockSpacerLength = 4;
		BlockLength = (BLOCK_SIZE * RecordLength);
		NumFields = 0;
		PopulateSequenceSet("TestInput.txt");
	}

	~SequenceSet()
	{

	}

	bool errorCheck_state(const string& is_state);

	bool errorCheck_zip(const string& is_zip);

	/**
	//	PopulateSequenceSet
	//  @brief Reads input data file and populates blocked record file and index file
	//  @pre input data file header structured appropriately
	//  @post none
	*/
	void PopulateSequenceSet(string input);

	// Searches for a record that contains a certain zip code.
	/**
	//	ZipSearch
	//  @brief Allows user to search for and print a specific zip code
	//  @pre data files built properly
	//  @post none
	*/
	void ZipSearch(int Zipcode);

	// Searches for Northernmost, Southernmost, Easternmost, and Westernmost Zip Codes in a State.
	/**
	//	StateSearch
	//  @brief Scans the data file for the furthest N, S, E, and W place in a specified state
	//  @pre input state in the correct format, data file formated correctly
	//  @post none
	*/
	void StateSearch(string State);

	void UpdateRecord(int zip, int field, string change)
	{
		Block Update;
		int RecordLocation = 0;
		int Offset = Search(zip);
		Update = GetBlock(Offset);

		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			if (Update.Records[i].Zip == zip)
			{
				RecordLocation = i;
				break;
			}
		}

		//update field in memory
		if (field == 0)
		{
			Update.Records[RecordLocation].Zip = stoi(change);
		}
		else if (field == 1)
		{
			Update.Records[RecordLocation].Place = change;
		}
		else if (field == 2)
		{
			Update.Records[RecordLocation].State = change;
		}
		else if (field == 3)
		{
			Update.Records[RecordLocation].County = change;
		}
		else if (field == 4)
		{
			Update.Records[RecordLocation].Latitude = stof(change);
		}
		else
		{
			Update.Records[RecordLocation].Longitude = stof(change);
		}

		//Write block back out to file
		//
		//if (!List.is_open())
		//{
		//	List.open("TestList.bin", ios::binary);
		//}

		//List.seekp(Offset);
		//for (int i = 0; i < BLOCK_SIZE; i++)
		//{
		//	List.write(reinterpret_cast<char*>(&Update.Records[i].Zip), sizeof(Update.Records[i].Zip));
		//	List << Update.Records[i].Place;
		//	List << Update.Records[i].State;
		//	List << Update.Records[i].County;
		//	List.write(reinterpret_cast<char*>(&Update.Records[i].Latitude), sizeof(Update.Records[i].Latitude));
		//	List.write(reinterpret_cast<char*>(&Update.Records[i].Longitude), sizeof(Update.Records[i].Longitude));
		//}
	}

}; //end


#endif