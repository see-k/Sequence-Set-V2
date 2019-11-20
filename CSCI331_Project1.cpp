// CSCI331_Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
	while (UserInput != 'q')
	{
		DisplayMenu();
		cin >> UserInput;

		if (UserInput == 'a')
		{
			Test.PopulateSequenceSet("TestInput.txt");

		}

	}

	cout << "\nCLEAN EXIT\n";
	return 0;
}

