// CSCI331_Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//test test test

#include "pch.h"
#include <iostream>
#include "SequenceSet.h"
//#include "SequenceSet.cpp"

using namespace std;

void DisplayMenu()
{
	cout << " it me, menu!\n";
	return;
}

int main()
{
	SequenceSet Test;
	char UserInput = ' ';
	string zipCodeSelection = ' ';
	while (UserInput != 'q')
	{
		DisplayMenu();
		cin >> UserInput;

		if (UserInput == 'a')
		{
			Test.PopulateSequenceSet("TestInput.txt");

		}

		else if (UserInput == 'b')
		{
			cout << "Please enter a zipcode you would like to search for:" << endl;
			cin >> zipCodeSelection;
			Test.SearchSequenceSet(zipcodeSelection);

		}

	}

	cout << "\nCLEAN EXIT\n";
	return 0;
}

