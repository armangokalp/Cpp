// Arman Gökalp	-- 29398
// Made with Visual Studio v16.11.25

#include "header"


void quickSort(vector<array<string, 3>>& input, int low, int high)
{
	if (low < high) 
	{
		array<string, 3> pivot = input[low + (high - low) / 2];
		int i = low - 1;
		swap(input[low + (high - low) / 2], input[high]);

		for (int j = low; j <= high - 1; j++)
		{
			if (input[j][0] < pivot[0])
			{
				i++;
				swap(input[i], input[j]);
			}
		}
		swap(input[i + 1], input[high]);

		int pi = i + 1;

		quickSort(input, low, pi - 1);
		quickSort(input, pi + 1, high);
	}

}


void insertionSort(vector<array<string, 3>> &input)
{
	array<string, 3> key;	int j;

	for (int i = 1; i < input.size(); i++) 
	{
		key = input[i];
		j = i - 1;

		while (j >= 0 && input[j][0] > key[0])
		{
			input[j + 1] = input[j];
			j--;
		}
		input[j + 1] = key;
	}

}


void mergeSort(vector<array<string, 3>> &input, int left, int right)
{
	if (left < right)
	{
		int median = left + (right - left) / 2;

		mergeSort(input, left, median);
		mergeSort(input, median + 1, right);

		inplace_merge(input.begin() + left, input.begin() + median + 1, input.begin() + right + 1);
	}
}


void heapify(vector<array<string, 3>>& input, int n, int i) 
{
	int largest = i, l = 2 * i + 1, r = 2 * i + 2;

	if (l < n && input[l][0] > input[largest][0])	largest = l;

	if (r < n && input[r][0] > input[largest][0])	largest = r;

	if (largest != i) 
	{
		swap(input[i], input[largest]);
		heapify(input, n, largest);
	}
}


void heapSort(vector<array<string, 3>> &input)
{
	// Building heap
	for (int i = input.size()/2-1; i >= 0; i--)		heapify(input, input.size(), i);

	// Extracting element from the heap one-by-one
	for (int i = input.size()-1; i >= 0; i--) 
	{
		swap(input[0], input[i]);
		heapify(input, i, 0);
	}
}


vector<array<string, 3>> binarySearch(vector<array<string, 3>> &contacts, const string &word) 
{
	int left = 0, right = contacts.size() - 1;

	vector<array<string, 3>> matches;

	while (left <= right) 
	{
		int mid = left + (right - left) / 2;
		string currPrefix = contacts[mid][0].substr(0, word.size());

		if (currPrefix == word) 
		{
			int start = mid, end = mid;

			// Finding matching elements
			while (start > 0 && contacts[start - 1][0].substr(0, word.size()) == word)	start--;
			while (end < contacts.size() - 1 && contacts[end + 1][0].substr(0, word.size()) == word)	end++;

			for (int i = start; i <= end; i++)	matches.push_back(contacts[i]);

			break;  // Exit when all matches have been found
		}
		else if (currPrefix < word)	left = mid + 1;
		else						right = mid - 1;
	}

	return matches;
}


vector<array<string, 3>> sequentialSearch(vector<array<string, 3>> &contacts, const string &word) 
{
	vector<array<string, 3>> matches;

	for (const auto& contact : contacts)	if (contact[0].substr(0, word.size()) == word)	matches.push_back(contact);

	return matches;
}


int main()
{
	string file_name, query;
	cout << "Please enter the contact file name:\n";		cin >> file_name;
	cout << "Please enter the word to be queried:\n";		cin >> query;
	ifstream file(file_name);

	vector<array<string, 3>> contacts[4], matches[2];	string first, last, phone, city;
	float time[6];

	while (file >> first >> last >> phone >> city)	contacts[0].push_back({ first + " " + last, phone, city});	// Inserting the contact list

	contacts[3] = contacts[2] = contacts[1] = contacts[0];

	cout << "\nSorting the vector copies\n======================================\n";

	auto start = chrono::high_resolution_clock::now();
	quickSort(contacts[0], 0, contacts[0].size()-1);
	time[0] = float(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count());
	start = chrono::high_resolution_clock::now();
	insertionSort(contacts[1]);
	time[1] = float(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count());
	start = chrono::high_resolution_clock::now();
	mergeSort(contacts[2], 0, contacts[2].size()-1);
	time[2] = float(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count());
	start = chrono::high_resolution_clock::now();
	heapSort(contacts[3]);
	time[3] = float(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count());

	if (contacts[0].size() < 10)	time[0] = time[1];

	cout << "Quick Sort Time: " << time[0] << " Nanoseconds\nInsertion Sort Time: " << time[1] << " Nanoseconds\nMerge Sort Time: " << time[2] << " Nanoseconds\nHeap Sort Time: " << time[3] << " Nanoseconds\n\nSearching for " << query << "\n======================================\n";

	start = chrono::high_resolution_clock::now();
	matches[0] = binarySearch(contacts[0], format(query));
	time[4] = float(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count());
	start = chrono::high_resolution_clock::now();
	matches[1] = sequentialSearch(contacts[0], format(query));
	time[5] = float(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count());

	if (!matches[0].empty())
	{
		for (const auto& match : matches[0])	cout << match[0] << " " << match[1] << " " << match[2] << endl;
	}
	else cout << format(query, "") << "does NOT exist in the dataset\n";
	cout << "Binary Search Time: " << time[0] << " Nanoseconds\n\nSearch results for Sequential Search:\n";

	if (!matches[1].empty())
	{
		for (const auto& match : matches[1])	cout << match[0] << " " << match[1] << " " << match[2] << endl;
	}
	else cout << format(query, "") << "does NOT exist in the dataset\n";
	cout << "\nSequential Search Time: " << time[1] << " Nanoseconds\n\n";

	cout << "SpeedUp between Search Algorithms\n======================================\n(Sequential Search / Binary Search) SpeedUp = " << time[5] / time[4] << "\n\nSpeedUp between Sorting Algorithms\n======================================\n(Insertion Sort / Quick Sort) SpeedUp = " << time[1] / time[0] << "\n(Merge Sort / Quick Sort) SpeedUp = " << time[2] / time[0] << "\n(Heap Sort / Quick Sort) SpeedUp = " << time[3] / time[0] << endl;

	
	
}