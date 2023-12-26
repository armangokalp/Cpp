// Arman Gökalp	- 29398
// Made with Visual Studio v16.11.25

#include "header.h"


struct Contact
{
	string fullName, phoneNumber, city;

	Contact(string name, string number, string city) : fullName(name), phoneNumber(number), city(city) {}
};

class PhonebookManagerHashtable
{
	unordered_map<string, Contact*> contacts;

	double maxLoadFactor, minLoadFactor;
	int numElements, tableSize;

public:
	PhonebookManagerHashtable() : maxLoadFactor(0.9), minLoadFactor(0.25), numElements(0), tableSize(53) {}

	void addContact(string name, string number, string city);

	vector<Contact*> searchContact(string name);

	void deleteContact(string name);

	~PhonebookManagerHashtable() { for (auto& pair : contacts)		delete pair.second; }

	void rehash();

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


	int size() const {	return numElements;	}

	float loadFactor() const {	return static_cast<float>(numElements) / tableSize;	}

};




void PhonebookManagerHashtable::addContact(string name, string number, string city) 
{
	if (loadFactor() > maxLoadFactor)	rehash();

	Contact* newContact = new Contact(name, number, format(city));
	contacts[name] = newContact;
	numElements++;
}


vector<Contact*> PhonebookManagerHashtable::searchContact(string name)
{
	vector<Contact*> matches;
	for (const auto& contact : contacts)	if (format(contact.second->fullName.substr(0, name.size())) == format(name))	matches.push_back(contact.second);
	return matches;
}


void PhonebookManagerHashtable::deleteContact(string name)
{
	if (contacts.find(name) != contacts.end()) 
	{
		delete contacts[name];	
		contacts.erase(name);
		numElements--;

		if (loadFactor() < minLoadFactor)	contacts.rehash(max(numElements, 1));
	}
}


void PhonebookManagerHashtable::rehash()
{
	int oldSize = tableSize;
	tableSize *= 2;
	contacts.rehash(tableSize);

	cout << "rehashed...\nprevious table size: " << oldSize << ", new table size: " << tableSize << ", current unique item count : " << numElements << ", current load factor: " << loadFactor() << endl;
}




int main()
{
	PhonebookManager<string> BST;
	PhonebookManagerHashtable Hash;

	string file_name, Hline;
	cout << "Enter the file name: ";	cin >> file_name;
	ifstream file(file_name), fileHashtable(file_name);
	vector<Contact*> Hmatches;

	string first, last, phone, city;	vector<string> matches;
	bool contactAlreadyExist = false;

	// Inserting phonebook data into a BST
	auto startBST = chrono::high_resolution_clock::now();
	while (file >> first >> last >> phone >> city)
	{
		BST.addContact(first, last, phone, city);
	}
	float BSTTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startBST).count();


	// Hashtable insertion
	auto startHashtable = chrono::high_resolution_clock::now();

	while (getline(fileHashtable, Hline)) 
	{
		istringstream iss(Hline);
		if (!(iss >> first >> last >> phone >> city)) 
		{
			cerr << "Error reading line from file: " << Hline << endl;	continue;
		}

		Hash.addContact(Hash.format(first, last), phone, Hash.format(city));
	}

	fileHashtable.close();

	cout << "\nAfter preprocessing, the contact count is " << Hash.size() << ". Current load ratio is " << Hash.loadFactor() << endl;

	float HashtableTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startHashtable).count();


	file.close();




	// Take user input for actions

	while (true)
	{
		int input;
		cout << "\nChoose which action to perform from 1 to 4:\n1 - Search a phonebook contact\n2 - Adding a phonebook contact\n3 - Deleting a phonebook contact\n4 - Press 4 to exit\n";	cin >> input;

		switch (input)
		{
		case 1:
			cin.ignore();
			cout << "\nSearch for a contact: ";	getline(cin, first); cout << endl;

			// Search for BST
			cout << "\nSearching an item in the phonebook (BST) . . .\n\nPhonebook: Searching for: ( " << BST.format(first, "") << ")\n====================================\n";
			startBST = chrono::high_resolution_clock::now();
			matches = BST.searchContact(first);
			for (const auto& match : matches) cout << match << endl;
			BSTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startBST).count();
			if (matches.empty())	cout << "Name not found!\n";
			cout << "\nBST Search Time: " << BSTTime << endl;

			// Search for Hashtable
			cout << "\nSearching an item in the phonebook (HashTable) . . .\n\nPhonebook: Searching for: ( " << Hash.format(first, "") << ")\n====================================\n";
			startHashtable = chrono::high_resolution_clock::now();
			Hmatches = Hash.searchContact(first);
			HashtableTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startHashtable).count();
			for (Contact* contact : Hmatches)	cout << Hash.format(contact->fullName, "") << " " << contact->phoneNumber << " " << Hash.format(contact->city) << endl;
			if (Hmatches.empty())	cout << "Name not found!\n";
			cout << "\nHash Table Search Time: " << HashtableTime;
			cout << "\nSpeed Up: " << BSTTime / HashtableTime << endl;

			break;

		case 2:
			cout << "\nEnter the information of the contact to be added:\nName: ";	cin >> first >> last;
			cout << "\nTel: ";														cin >> phone;
			cout << "\nCity: ";														cin >> city;

			// Adding contact to BST
			cout << "\nAdding an item to the phonebook BST . . .\n====================================\n";
			startBST = chrono::high_resolution_clock::now();
			contactAlreadyExist = BST.addContact(first, last, phone, city);
			BSTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startBST).count();
			if (!contactAlreadyExist)	cout << "Contact has been added successfully to the BST\n\n";
			else
			{
				cout << "The given contact full name already exists in the database.\n";	break;
			}

			// Adding contact to Hashtable tree
			cout << "Adding an item to the phonebook Hashtable . . .\n====================================\n";
			startHashtable = chrono::high_resolution_clock::now();
			Hash.addContact(Hash.format(first, last), phone, city);
			HashtableTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startHashtable).count();
			cout << "Contact has been added successfully to the Hashtable\n\n";

			cout << "Adding a contact to the Binary Tree took " << BSTTime << " nanoseconds. . .\n";
			cout << "Adding a contact to the Hashtable took " << HashtableTime << " nanoseconds. . .\n";
			break;

		case 3:
			cout << "\nDeleting an item from the phonebook . . .\nEnter the fullname of the contact to be deleted: ";	cin >> first >> last;

			// Deleting element from BST
			startBST = chrono::high_resolution_clock::now();
			BST.deleteContact(BST.root, BST.format(first, last));
			BSTTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startBST).count();

			// Deleting element from Hashtable tree
			startHashtable = chrono::high_resolution_clock::now();
			Hash.deleteContact(Hash.format(first, last));
			HashtableTime = chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - startHashtable).count();

			cout << "\nDeleted succcessfully. . .\n\n";

			cout << "Deletion from the Binary Tree took " << BSTTime << " nanoseconds. . .\n";
			cout << "Deletion from Hashtable took " << HashtableTime << " nanoseconds. . .\n";
			break;

		case 4:
			return 0;
		}
	}

}