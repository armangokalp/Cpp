#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;


template <class T>
class PhonebookManager {

    struct Contact
    {
        T fullName, phoneNumber, city;
        Contact* left = nullptr, * right = nullptr, * parent = nullptr;
        Contact(T fullName, T phoneNumber, T city) : fullName(fullName), phoneNumber(phoneNumber), city(city) {};
    };

    vector<string> contacts;

public:
    Contact* root = nullptr;

    ~PhonebookManager() { deallocateTree(root); }

    vector<T> searchContact(const T& input);
    bool addContact(T firstName, T lastName, T phoneNumber, T city);
    Contact* deleteContact(Contact* node, T fullName);

    Contact* rotateNode(Contact* node, bool left);
    T format(T firstName, T lastName);  T format(T city);
    int height(Contact* node);
    void deallocateTree(Contact* node);


};


bool PhonebookManager<string>::addContact(string firstName, string lastName, string phoneNumber, string city)
{
	Contact* newNode = new Contact{ format(firstName, lastName), phoneNumber, format(city) };

	if (!root)	root = newNode;

	else
	{
		Contact* temp = root;
		while (true)
		{
			if (newNode->fullName < temp->fullName)
			{
				if (temp->left == nullptr) {
					temp->left = newNode;
					newNode->parent = temp;
					break;
				}
				else						temp = temp->left;
			}
			else if (newNode->fullName > temp->fullName)
			{
				if (temp->right == nullptr) {
					temp->right = newNode;
					newNode->parent = temp;
					break;
				}
				else						temp = temp->right;
			}
			else
			{
				return 1;
			}
		}
	}

	contacts.push_back(format(firstName, lastName) + " " + phoneNumber + " " + format(city));

	return 0;

}


PhonebookManager<string>::Contact* PhonebookManager<string>::deleteContact(Contact* root, string fullName) 
{

	if (root == nullptr) return root;
	if (fullName < root->fullName) root->left = deleteContact(root->left, fullName);
	else if (fullName > root->fullName) root->right = deleteContact(root->right, fullName);

	else
	{
		if ((root->left == nullptr) || (root->right == nullptr))
		{
			Contact* temp = root->left ? root->left : root->right;

			if (temp == nullptr) {
				temp = root;
				root = nullptr;
			}
			else	*root = *temp;

			delete temp;
		}
		else
		{
			Contact* temp = root->right;
			while (temp->left) temp = temp->left;

			root->fullName = temp->fullName;
			root->right = deleteContact(root->right, temp->fullName);
		}
	}

	if (root == nullptr) return root;


	int balance = height(root->left) - height(root->right);

	if (balance > 1 && height(root->left->left) >= height(root->left->right))
		return rotateNode(root, 0);

	if (balance < -1 && height(root->right->right) >= height(root->right->left))
		return rotateNode(root, 1);

	if (balance > 1 && height(root->left->left) < height(root->left->right)) {
		root->left = rotateNode(root->left, 1);
		return rotateNode(root, 0);
	}

	if (balance < -1 && height(root->right->right) < height(root->right->left)) {
		root->right = rotateNode(root->right, 0);
		return rotateNode(root, 1);
	}


	for (auto it = contacts.begin(); it != contacts.end(); ++it)	if (*it == fullName + " " + root->phoneNumber + " " + root->city) { contacts.erase(it);	break; }


	return root;
}


vector<string> PhonebookManager<string>::searchContact(const string& input)
{
	vector<string> matches;
	for (const auto& contact : contacts) {
		if (format(contact.substr(0, input.size())) == format(input)) {
			matches.push_back(contact);
		}
	}
	return matches;
}


void PhonebookManager<string>::deallocateTree(Contact* node)
{
    if (node == nullptr) return;

    deallocateTree(node->left);
    deallocateTree(node->right);

    delete node;
}

template<>
int PhonebookManager<string>::height(Contact* node)
{
    if (node == nullptr)    return 0;

    return 1 + max(height(node->left), height(node->right));
}

template<>
PhonebookManager<string>::Contact* PhonebookManager<string>::rotateNode(Contact* node, bool left) 
{
    Contact* newNode = nullptr, * temp = nullptr;

    if (left)
    {
        newNode = node->right;

        temp = newNode->left;

        newNode->left = node;
        node->right = temp;

        if (temp != nullptr) temp->parent = node;
        newNode->parent = node->parent;
        node->parent = newNode;
    }
    else
    {
        newNode = node->left;

        temp = newNode->right;

        newNode->right = node;
        node->left = temp;

        if (temp != nullptr) temp->parent = node;
        newNode->parent = node->parent;
        node->parent = newNode;
    }

    return newNode;
}

template<>
string PhonebookManager<string>::format(string firstName, string lastName)
{
    for (unsigned int i = 0; i < firstName.length(); i++)    firstName[i] = toupper(firstName[i]);
    for (unsigned int i = 0; i < lastName.length(); i++)     lastName[i] = toupper(lastName[i]);

    return firstName + " " + lastName;
}

template<>
string PhonebookManager<string>::format(string city)
{
    city[0] = toupper(city[0]);
    for (unsigned int i = 1; i < city.length(); i++)    city[i] = tolower(city[i]);

    return city;
}