// Arman Gökalp	- 29398
// Made with Visual Studio v16.11.25

#include "header.h"


PhonebookManager<string>::Contact* PhonebookManager<string>::insertAVL(Contact* node, Contact* newNode)
{
	
	if (node == nullptr)	return newNode;

	if (newNode->fullName < node->fullName)			node->left = insertAVL(node->left, newNode);
	else if (newNode->fullName > node->fullName)	node->right = insertAVL(node->right, newNode);
	else											return node;

	int balance = node ? ( height(node->left) - height(node->right) ) : 0;


	if (balance > 1 && newNode->fullName < node->left->fullName)	
		return rotateNode(node, 0);

	if (balance < -1 && newNode->fullName > node->right->fullName)	
		return rotateNode(node, 1);

	if (balance > 1 && newNode->fullName > node->left->fullName) {
		node->left = rotateNode(node->left, 1);
		node = rotateNode(node, 0);
	}

	if (balance < -1 && newNode->fullName < node->right->fullName) {
		node->right = rotateNode(node->right, 0);
		node = rotateNode(node, 1);
	}

	return node;
}


bool PhonebookManager<string>::addContact(string firstName, string lastName, string phoneNumber, string city, bool AVL)
{
	Contact* newNode = new Contact{ format(firstName, lastName), phoneNumber, format(city) };

	if (!root)	root = newNode;

	else if (!AVL)
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
	else
	{
		root = insertAVL(root, newNode);
		if (newNode->fullName == root->fullName)	return 1;
	}

	contacts.push_back(format(firstName, lastName));

	return 0;

}


PhonebookManager<string>::Contact* PhonebookManager<string>::deleteContact(Contact* root, string fullName) {

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
            } else	*root = *temp;

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

	for (auto it = contacts.begin(); it != contacts.end(); ++it)	if (*it == fullName) {	contacts.erase(it);	break;	}

    return root;
}


vector<string> PhonebookManager<string>::searchContact(const string& input)
{
	std::vector<string> matches;
	for (const auto& contact : contacts) {
		if (format(contact.substr(0, input.size())) == format(input)) {
			matches.push_back(contact);
		}
	}
	return matches;
}

void PhonebookManager<string>::inOrderPrintToFile(Contact* node, ofstream &output)
{
	if (!node)	return;

	inOrderPrintToFile(node->left, output);
	output << node->fullName << " " << node->phoneNumber << " " << node->city << endl;
	inOrderPrintToFile(node->right, output);
}

void PhonebookManager<string>::preOrderPrintToFile(Contact* node, ofstream &output)
{
	if (!node)	return;

	output << node->fullName << " " << node->phoneNumber << " " << node->city << endl;
	preOrderPrintToFile(node->left, output);
	preOrderPrintToFile(node->right, output);
}

void PhonebookManager<string>::drawTreeToFile(Contact* node, ofstream& output, int height)
{
	if (node == nullptr)	return;

	for (int i = 0; i < height; ++i)	output << "|  ";

	output << (!node->parent ? "|__" : node->parent->right == node ? "|__" : "|--") << node->fullName << "\n";

	drawTreeToFile(node->left, output, height + 1);
	drawTreeToFile(node->right, output, height + 1);
}


int main() 
{
	PhonebookManager<string> BST, AVL;
	
	string file_name;
	cout << "Please enter the contact file name: ";		cin >> file_name;
	ifstream file(file_name), fileAVL(file_name);	ofstream output;

	string first, last, phone, city;	vector<string> matches;
	int BST_leftH = 0, BST_rightH = 0, AVL_leftH = 0, AVL_rightH = 0;
	bool contactAlreadyExist = false;

	// Inserting phonebook data into a BST
	cout << "Loading the phonebook into a BST." << endl;

	auto startBST = chrono::high_resolution_clock::now();
	while (file >> first >> last >> phone >> city)
	{
		BST.addContact(first, last, phone, city, 0);
	}
	auto stopBST = chrono::high_resolution_clock::now();

	cout << "Phonebook creation in BST took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . ." << endl;

	// Calculating height of the BST
	if (BST.root) {
		BST_leftH = BST.height(BST.root->left); BST_rightH = BST.height(BST.root->right);
	}
	if (abs(BST_leftH - BST_rightH) <= 1)	cout << "The tree is balanced" << endl;
	else									cout << "The tree is not balanced" << endl;
	cout << "The heights of BST are for left: " << BST_leftH << " and right: " << BST_rightH << endl;

	// Inserting phonebook data into an AVL tree
	cout << "loading the phonebook into an AVL" << endl;

	auto startAVL = chrono::high_resolution_clock::now();
	while (fileAVL >> first >> last >> phone >> city)
	{
		AVL.addContact(first, last, phone, city, 1);
	}
	auto stopAVL = chrono::high_resolution_clock::now();

	cout << "Phonebook creation in AVL took " << chrono::duration_cast<chrono::milliseconds>(stopAVL - startAVL).count() << " milliseconds. . ." << endl;

	// Calculating height of the AVL tree
	if (AVL.root) {
		AVL_leftH = AVL.height(AVL.root->left);	AVL_rightH = AVL.height(AVL.root->right);
	}
	if (abs(AVL_leftH - AVL_rightH) <= 1)	cout << "The tree is balanced" << endl;
	else									cout << "The tree is not balanced" << endl;
	cout << "The heights of AVL are for left: " << AVL_leftH << " and right: " << AVL_rightH << endl;

	file.close();




	// Take user input for actions

	while (true) 
	{
		int input;
		cout << "\nChoose which action to perform from 1 to 6:\n1 - Search a phonebook contact\n2 - Adding a phonebook contact\n3 - Deleting a phonebook contact\n4 - Print the phonebook to a file(inorder)\n    Print the phoenbook to a file(preorder)\n5 - Draw the phonebook as a Tree to a file\n6 - Press 6 to exit\n";	cin >> input;

		switch (input)
		{
		case 1:
			cin.ignore();
			cout << "\nSearch for a contact: ";	getline(cin, first); cout << endl;

			cout << "\nSearching an item in the phonebook (BST) . . .\n\nPhonebook: Searching for: ( " << BST.format(first, "") << ")\n====================================\n";
			startBST = chrono::high_resolution_clock::now();
			matches = BST.searchContact(first);
			for (const auto& match : matches) cout << match << endl;
			stopBST = chrono::high_resolution_clock::now();

			cout << "\nSearching an item in the phonebook (AVL) . . .\n\nPhonebook: Section: ( " << BST.format(first, "") << ")\n====================================\n";
			startAVL = chrono::high_resolution_clock::now();
			matches = AVL.searchContact(first);
			for (const auto& match : matches) cout << match << endl;
			stopAVL = chrono::high_resolution_clock::now();

			cout << "\nThe search in BST took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . .\n";
			cout << "The search in AVL took " << chrono::duration_cast<chrono::milliseconds>(stopAVL - startAVL).count() << " milliseconds. . .\n";

			break;

		case 2:
			cout << "\nEnter the information of the contact to be added:\nName: ";	cin >> first >> last;
			cout << "\nTel: ";														cin >> phone;
			cout << "\nCity: ";														cin >> city;

			// Adding contact to BST
			cout << "\nAdding an item to the phonebook BST . . .\n====================================\n";
			startBST = chrono::high_resolution_clock::now();
			contactAlreadyExist = BST.addContact(first, last, phone, city, 0);
			stopBST = chrono::high_resolution_clock::now();
			if (!contactAlreadyExist)	cout << "Contact has been added successfully to the BST\n\n";
			else
			{
				cout << "The given contact full name already exists in the database.\n";	break;
			}

			// Adding contact to AVL tree
			cout << "Adding an item to the phonebook AVL . . .\n====================================\n";
			startAVL = chrono::high_resolution_clock::now();
			AVL.addContact(first, last, phone, city, 1);
			stopAVL = chrono::high_resolution_clock::now();
			cout << "Contact has been added successfully to the AVL\n\n";

			cout << "Adding a contact to the Binary Tree took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . .\n";
			cout << "Adding a contact to the AVL Tree took " << chrono::duration_cast<chrono::milliseconds>(stopAVL - startAVL).count() << " milliseconds. . .\n";
			break;

		case 3:
			cout << "\nDeleting an item from the phonebook . . .\nEnter the fullname of the contact to be deleted: ";	cin >> first >> last;

			// Deleting element from BST
			startBST = chrono::high_resolution_clock::now();
			BST.deleteContact(BST.root, BST.format(first, last));
			stopBST = chrono::high_resolution_clock::now();

			// Deleting element from AVL tree
			startAVL = chrono::high_resolution_clock::now();
			AVL.deleteContact(AVL.root, AVL.format(first, last));
			stopAVL = chrono::high_resolution_clock::now();

			cout << "\nDeleted succcessfully. . .\n\n";

			cout << "Deletion from the Binary Tree took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . .\n";
			cout << "Deletion from AVL Tree took " << chrono::duration_cast<chrono::milliseconds>(stopAVL - startAVL).count() << " milliseconds. . .\n";
			break;

		case 4:
			cout << endl;

			// In order BST print
			output.open("phonebookInOrderBST.txt");
			startBST = chrono::high_resolution_clock::now();
			BST.inOrderPrintToFile(BST.root, output);
			stopBST = chrono::high_resolution_clock::now();
			output.close();

			// In order AVL tree print
			output.open("phonebookInOrderAVL.txt");
			startAVL = chrono::high_resolution_clock::now();
			AVL.inOrderPrintToFile(AVL.root, output);
			stopAVL = chrono::high_resolution_clock::now();
			output.close();

			cout << "Printing in order to file from the Binary Tree took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . .\n";
			cout << "Printing in order to file from the AVL Tree took " << chrono::duration_cast<chrono::milliseconds>(stopAVL - startAVL).count() << " milliseconds. . .\n";

			// Pre order BST print
			output.open("phonebookPreOrderBST.txt");
			startBST = chrono::high_resolution_clock::now();
			BST.preOrderPrintToFile(BST.root, output);
			stopBST = chrono::high_resolution_clock::now();
			output.close();

			// Pre order AVL print
			output.open("phonebookPreOrderAVL.txt");
			startAVL = chrono::high_resolution_clock::now();
			AVL.preOrderPrintToFile(AVL.root, output);
			stopAVL = chrono::high_resolution_clock::now();
			output.close();

			cout << "Printing pre order to file from the Binary Tree took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . .\n";
			cout << "Printing pre order to file from AVL Tree took " << chrono::duration_cast<chrono::milliseconds>(stopAVL - startAVL).count() << " milliseconds. . .\n";
			break;

		case 5:
			cout << endl;

			// Printing binary tree
			output.open("phonebookTreeBST.txt");
			startBST = chrono::high_resolution_clock::now();
			BST.drawTreeToFile(BST.root, output);
			stopBST = chrono::high_resolution_clock::now();
			output.close();

			// Printing AVL tree
			output.open("phonebookTreeAVL.txt");
			startAVL = chrono::high_resolution_clock::now();
			AVL.drawTreeToFile(AVL.root, output);
			stopAVL = chrono::high_resolution_clock::now();
			output.close();

			cout << "Drawing tree to file from the Binary Tree took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . .\n";
			cout << "Drawing tree to file from AVL Tree took " << chrono::duration_cast<chrono::milliseconds>(stopBST - startBST).count() << " milliseconds. . .\n";
			break;

		case 6:
			cout << "Exiting...\n";
			return 0;
		}
	}

}