#include <iostream>
#include <string>
#include <sstream>

#include "activity_bst.h"

using namespace std;

int main() {
    tree_node *root = nullptr;
    string user_input;
    cout << "Welcome to your time manager. Enter a command: ";
    while (getline(cin, user_input)) {
        stringstream ss(user_input);
        int action;
        int time;
        string activity;
        ss >> action;
        if (ss.fail()) {
            cout << "ERROR! Action entered is not a number\n";
        } else {
            switch (action) {
                case 0: // Add an activity
                    cout << " **** Add activity ****" << endl;
                    ss >> time >> activity;
                    if (ss.fail()) {
                        cout << "ERROR! Time or activity is invalid\n";
                    } else {
                        add_activity(root, time, activity);
                    }
                    break;
                case 1:
                    cout << " **** Check activity ****" << endl;
                    ss >> time;
                    if (ss.fail()) {
                        cout << "ERROR! Time is invalid\n";
                    } else {
                        print_activity(root, time);
                    }
                    break;
                case 2:
                    cout << " **** Check activity and duration ****" << endl;
                    ss >> time;
                    if (ss.fail()) {
                        cout << "ERROR! Time is invalid\n";
                    } else {
                        print_activity_and_duration(root, time);
                    }
                    break;
                case 3:
                    cout << " **** Check all occurrences of an activity ****"
                         << endl;
                    ss >> activity;
                    if (ss.fail()) {
                        cout << "ERROR! Activity is invalid\n";
                    } else {
                        print_single_activity(root, activity);
                    }
                    break;
                case 4:
                    cout << " **** Print all occurrences of an activity ****"
                         << endl;
                    print_all_activities(root);
                    break;
                default:
                    cout << "Action not recognized" << endl;
            }
        }
        cout << "-------------------" << endl << endl;
        cout << "Enter a command: ";
    }
    delete_tree(root);
    return 0;
}
