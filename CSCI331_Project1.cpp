// CSCI331_Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include "SequenceSet.h"
//#include "SequenceSet.cpp"

using namespace std;

/**
//	DisplayMenu
//  @brief displays simple user interface for guiding user input
//  @pre none
//  @post none
*/
void DisplayMenu()
{
	cout << "\nTEST PROGRAM MENU\n";
	cout << "Enter 1-4 to do the following:\n";
	cout << "1. Populate the Sequence Set.\n";
	cout << "2. Search for Northernmost, Southernmost, Easternmost, and Westernmost Zip Codes in a State.\n";
	cout << "3. Search for a specific Zip Code.\n";
	cout << "4. Exit Program.\n";
	return;
}

int main()
{
	SequenceSet Test;
	int UserInput = 0;
	string StateInput;
	int ZipCodeInput;
	string Zip;
	int FieldToChange = 0;
	string change = "";
	bool throw_error;
	while (UserInput != 4)
	{
		DisplayMenu();
		cin >> UserInput;

		if (UserInput == 1)
		{
			Test.PopulateSequenceSet("TestInput.txt");
			cout << "\nSequence Set Populated\n";
		}
		else if(UserInput == 2)
		{
			cout << "\nEnter the State you would like to search through in abbreviated form (For Example, MN for Minnesota): \n";
			//ERROR CHECK: State
			throw_error = true;
			while (throw_error)
			{
				cout << "State: ";
				cin >> StateInput;
				throw_error = Test.errorCheck_state(StateInput);
				if (throw_error)
					cout << "|>error:|State entry| Please use standard state abbreviations[ST].." << endl;
				else throw_error = false;
			}

			Test.StateSearch(StateInput);
		}
		else if (UserInput == 3)
		{
			cout << "\nEnter the Zip Code you would like to search for: \n";
			//ERROR CHECK: Zip Code
			throw_error = true;
			while (throw_error)
			{
				cout << "Zip Code: ";
				cin >> Zip;
				throw_error = Test.errorCheck_zip(Zip);
				//throw_error = StateInput.find_first_not_of(ZipCodeInput);
				if (throw_error)
					cout << "|>error:|Zip Code entry| Please enter number only.." << endl;
				else
				{
					throw_error = false;
					std::istringstream iss(Zip);
					iss >> ZipCodeInput;
				}
			}
			Test.ZipSearch(ZipCodeInput);
		}
		else if (UserInput != 4)
		{
			cout << "\nInvalid input, try again.\n";
		}
	}
	cout << "\nCLEAN EXIT\n";

	return 0;
}

