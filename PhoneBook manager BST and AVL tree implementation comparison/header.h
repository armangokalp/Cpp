#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>

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
    bool addContact(T firstName, T lastName, T phoneNumber, T city, bool AVL);
    Contact* deleteContact(Contact* node, T fullName);
    void inOrderPrintToFile(Contact* node, ofstream &output);
    void preOrderPrintToFile(Contact* node, ofstream &output = ofstream());
    void drawTreeToFile(Contact* node, ofstream& output, int height = 0);

    // Helper functions
    Contact* search(Contact* node, T key);
    Contact* insertAVL(Contact* node, Contact* newNode);
    Contact* rotateNode(Contact* node, bool left);
    bool foundInSearch(T fullName, T input);
    T format(T firstName, T lastName);
    T format(T city);
    int height(Contact* node);
    void deallocateTree(Contact* node);


};


void PhonebookManager<string>::deallocateTree(Contact* node)
{
	if (node == nullptr) return;

	deallocateTree(node->left);
	deallocateTree(node->right);

	delete node;
}


int PhonebookManager<string>::height(Contact* node)
{
    if (node == nullptr)    return 0;
    
    return 1 + max(height(node->left), height(node->right));
}


PhonebookManager<string>::Contact* PhonebookManager<string>::rotateNode(Contact* node, bool left) {


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


string PhonebookManager<string>::format(string firstName, string lastName)
{
    for (unsigned int i = 0; i < firstName.length(); i++)    firstName[i] = toupper(firstName[i]);
    for (unsigned int i = 0; i < lastName.length(); i++)     lastName[i] = toupper(lastName[i]);

    return firstName + " " + lastName;
}


string PhonebookManager<string>::format(string city)
{
    city[0] = toupper(city[0]);
    for (unsigned int i = 1; i < city.length(); i++)    city[i] = tolower(city[i]);

    return city;
}