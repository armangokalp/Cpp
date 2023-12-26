#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <array>
#include <string>
#include <algorithm>


using namespace std;


void quickSort(vector<array<string, 3>> &input, int low, int high);
void insertionSort(vector<array<string, 3>> &input);
void mergeSort(vector<array<string, 3>> &input, int left, int right);
void heapSort(vector<array<string, 3>> &input);
vector<array<string, 3>> binarySearch(vector<array<string, 3>> &contacts, const string &word);
vector<array<string, 3>> sequentialSearch(vector<array<string, 3>> &contacts, const string &word);



string format(string firstName, string lastName)
{
	for (unsigned int i = 0; i < firstName.length(); i++)    firstName[i] = toupper(firstName[i]);
	for (unsigned int i = 0; i < lastName.length(); i++)     lastName[i] = toupper(lastName[i]);

	return firstName + " " + lastName;
}


string format(string city)
{
	city[0] = toupper(city[0]);
	for (unsigned int i = 1; i < city.length(); i++)    city[i] = tolower(city[i]);

	return city;
}