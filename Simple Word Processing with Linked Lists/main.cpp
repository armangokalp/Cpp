//Made with Visual Studio 2019 v16.11.11
//Arman Gokalp    00029398

#include <iostream>
#include <fstream>
#include <string>


struct node
{
    //this struct represents an element of a linked-list and stores both the value of the element and the memory location of the next element
    char ch;
    node* next;
};


std::ifstream Input(int count)
{
    std::ifstream input;    std::string input_name;

    do
    {
        std::cout << "Please enter the file " << count << " name: ";    std::cin >> input_name;
        input.open(input_name);

    } while (input.fail());

    return input;

}   //this function takes user input and opens the entered file


void addToTheLinkedList(node** head, char new_char)
{
    //creates the node;
    node* new_node = new node();
    new_node->ch = new_char;    new_node->next = NULL;

    //checks if the node is the head of the LinkedList
    if (*head == NULL)
    {
        *head = new_node;   return;
    }

    //finds the last element of the LinkedList
    node* last_node = *head;
    while (last_node->next != NULL)     last_node = last_node->next;

    //inserts the node to the end
    last_node->next = new_node;
}


void printLinkedList(node* n)
{
    while (n != NULL)
    {
        std::cout << n->ch;
        n = n->next;
    }
}


int similarityBetweenTheFiles(node* head1, node* head2)
{
    int count = -1;

    while (head1 != NULL && head2 != NULL)
    {
        if (head1->ch == head2->ch)         count++;

        head1 = head1->next;    head2 = head2->next;
    }

    return count;

}   //returns that how many of the nodes have the same character and place


std::string Command(std::string input)
{
    std::string result;

    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ' ') break;

        result += input[i];
    }

    return result;

}   //returns the command type from the input as a string


void insertAfterChar(node* prev, std::string input, int listNo)
{
    char new_char, prev_char;
    bool argument = false, new_charInitialized = false;
    node* first = prev;

    //takes the previous char and new char from the input
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ' ')
        {
            argument = true;
        }
        else if (argument)
        {
            if (new_charInitialized)    prev_char = input[i];
            else {
                new_char = input[i];
                new_charInitialized = true;     
            }
        }
    }

    std::cout << "Inserting " << new_char << " after " << prev_char << " in List" << listNo << ".\n";

    //equalizes the order of the first occurance of the prev_char with the node element
    while (prev != NULL)
    {
        if (prev->ch == prev_char)
        {
            std::cout << "The node with " << new_char << " has been inserted after the first occurrence of a node with " << prev_char << " value.\n";
            break;
        }

        if (prev->next != NULL) {
            prev = prev->next;
        }
        else    //inserts at the beginning if the prev_char doesn't exist
        {
            std::cout << "The node with " << prev_char << " value does not exist. Therefore, Inserting " << new_char << " at the beginning of the list.\n";

            prev = first;           break;
        }
    }

    //creates the node;
    node* new_node = new node();
    new_node->ch = new_char;

    //inserts the new_char after the previous
    new_node->next = prev->next;
    prev->next = new_node;

    std::cout << "The current content of List" << listNo << " is: ";    printLinkedList(first);
}


void deleteNode(node* delN, std::string input, int listNo, int recursive = 0, bool isDeleted = false)
{
    char char_toDelete = NULL;
    bool argument = false;
    node* head = delN;

    //takes the char to be deleted from the input
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ' ')
        {
            argument = true;
        }
        else if (argument)
        {
            char_toDelete = input[i];
            break;
        }
    }

    node* temp = head;
    //looks for char_toDelete and removes the element if it's found
    while (delN != NULL)
    {
        if (delN->ch == char_toDelete)
        {
            temp->next = delN->next;
            isDeleted = true;
        }

        temp = delN;
        if (delN != NULL) {
            delN = delN->next;
        }
        else break;
    }

    if (recursive != 2)  deleteNode(head, input, listNo, ++recursive, isDeleted);   //this function is recursive because in cases that char_toDeletes are successive the code ignores the repeated char 
    else
    {
        if (isDeleted) {
            std::cout << "Deleting all occurrences of " << char_toDelete << " in List" << listNo << ".\n" << "The current content of List" << listNo << " is: ";    printLinkedList(head);
        }
        else {
            std::cout << "No deletion as the value " << char_toDelete << " was not found in the list";
        }
    }
}


bool isArgumentVaild(std::string input)
{
    bool argument = false;

    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ' ') argument = true;

        if (argument && input[i] != ' ' && (!isalpha(input[i]) || !islower(input[i])))
        {
            return false;
        }
    }

    return true;

}   //checks if the input has arguments as lowercase letters


void commandInput(node* head1, node* head2)
{
    std::string input;
    std::cout << "\n\nEnter a command and its corresponding argument(s), if any: ";
    std::getline(std::cin, input);

    if (input == "exit")
    {
        std::cout << "Clearing the two lists and saying Goodbye!";
        return;
    }

    if (input == "sim")
    {
        std::cout << "Displaying the contents of the two lists:\nList1: ";   printLinkedList(head1);
        std::cout << "\nList2: ";                                            printLinkedList(head2);
        std::cout << "\nThere is/are " << similarityBetweenTheFiles(head1, head2) << " letter(s) matched in the same positions in both lists.";
    }
    else if (Command(input) == "insert1")
    {
        if (isArgumentVaild(input))
        {
            insertAfterChar(head1, input, 1);
        }
        else std::cout << "Invalid arguments.";
    }
    else if (Command(input) == "insert2")
    {
        if (isArgumentVaild(input))
        {
            insertAfterChar(head2, input, 2);
        }
        else std::cout << "Invalid arguments.";
    }
    else if (Command(input) == "del1")
    {
        if (isArgumentVaild(input))
        {
            deleteNode(head1, input, 1);
        }
        else std::cout << "Invalid arguments.";
    }
    else if (Command(input) == "del2")
    {
        if (isArgumentVaild(input))
        {
            deleteNode(head2, input, 2);
        }
        else std::cout << "Invalid arguments.";
    }
    else
    {
        std::cout << "Invalid command.";
    }

    commandInput(head1, head2);

}   //all user commands are managed in this function including input checks


int main()
{
    std::ifstream file1 = Input(1);        std::ifstream file2 = Input(2);
    char new_char;

    //head elements for both LinkedLists
    node* head1 = new node();
    node* head2 = new node();

    //writes the data from the txt files into seperate LinkedLists
    while (file1.get(new_char)) {
        if (!isspace(new_char))
            addToTheLinkedList(&head1, new_char);
    }
    while (file2.get(new_char)) {
        if (!isspace(new_char))
            addToTheLinkedList(&head2, new_char);
    }

    //displays the data in the files
    std::cout << "List1: ";       printLinkedList(head1);
    std::cout << "\nList2: ";     printLinkedList(head2);

    std::cin.ignore();  //ignores the getline command before the user enters an input
    commandInput(head1, head2);     //takes command inputs from the user

    //program closes the files and dellocates memory
    file1.close();                    file2.close();
    head1->next = NULL;          head2->next = NULL;
    delete head1;                      delete head2;
}
