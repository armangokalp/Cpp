//Arman Gökalp 00029398
//17/11/2022
//Visual Studio 2019

#include <iostream>
#include <string>

#include "activity_bst.h"

string fill_with_zeros(int num_zeros, const string& input) {
    int zeros_to_add = num_zeros - input.length();
    if (zeros_to_add < 0) zeros_to_add = 0;
    return string(zeros_to_add, '0') + input;
}
string number_to_time(int number) {
    return fill_with_zeros(2, to_string(number / 100)) + ":" + fill_with_zeros(2,
        to_string(number % 100));
}

bool time_error_check(int time, string activity, int s) {

    string _time = number_to_time(time);    int hours = stoi((_time.substr(0, _time.find(":")))), minutes = stoi((_time.substr(_time.find(":") + 1, _time.size())));
    if ((hours < 0 || hours > 23) || (minutes < 0 || minutes > 59)) {
        switch (s) {
        case 1:
            cout << "ERROR! Could not add activity '" << activity << "' due to illegal time value" << endl;
            break;
        case 2:
            cout << "ERROR! Could not print activity at specific time due to illegal time" << endl;
            break;
        case 3:
            cout << "ERROR! Could not print activity at given time interval due to illegal time" << endl;
            break;
        }

        return true;
    }

    else return false;
}

int search(tree_node* root, int& time, string& activity, int index) {
    activity = "free";
    int begin_time = 0, end_time = 0;
    tree_node* first_root = root;

    //searching for activity
    while (root) {
        if (time == root->time) {
            activity = root->activity;
            begin_time = root->time;
            break;
        }
        else if (time < root->time) {
            if (root->left && time > root->left->time) {
                activity = root->left->activity;
                begin_time = root->left->time;
            }
            root = root->left;
        }
        else if (time > root->time) {
            if ((root->right && time < root->right->time) || !root->right) {
                activity = root->activity;
                begin_time = root->time;
            }
            root = root->right;
        }

    }
    root = first_root;

    if (index == 1) {

        time = begin_time;

        //searching for the next activity
        int temp = 0;
        while (root) {

            if (root->time > time && (root->time < temp || temp == 0)) {
                end_time = root->time;
                temp = root->time;
            }

            if (time < root->time) root = root->left;
            else                   root = root->right;
        }
        root = first_root;

    }
    return end_time;

}


void add_activity(tree_node*& root, int time, const string& activity) {
    if (time_error_check(time, activity, 1)) return;

    tree_node* newNode = new tree_node(time, activity), * first_root = root;

    //if its the first activity create root node
    if (!root) {
        root = newNode;
        cout << "Added activity '" << activity << "' at " << number_to_time(time) << endl;
        return;
    }

    //loops until new leaf is added to tree;
    while (1 == 1) {

        //adding leafs to the tree
        if (time > root->time) {

            if (root->right) {
                root = root->right;
                continue;
            }
            else {
                root->right = newNode;
                newNode->parent = root;
                break;
            }
        }
        else {

            if (root->left) {
                root = root->left;
                continue;
            }
            else {
                root->left = newNode;
                newNode->parent = root;
                break;
            }
        }
    }

    cout << "Added activity '" << activity << "' at " << number_to_time(time) << endl;

    root = first_root;
}


void print_activity(tree_node* root, int time) {
    if (time_error_check(time, "", 2)) return;

    string activity;
    //searches activity
    search(root, time, activity, 0);

    cout << "Time: " << number_to_time(time) << ", Activity: " << activity << endl;

}


void print_activity_and_duration(tree_node* root, int time) {
    if (time_error_check(time, "", 3)) return;

    string activity;
    int next_activity = 0, first_activity = 0;

    //finds the first activity
    first_activity = search(root, next_activity, activity, 1);

    //searches activity and looks for the next one
    next_activity = search(root, time, activity, 1);

    //if there is no activity at given time, set next activity equal to first one
    if (activity == "free")    next_activity = first_activity;

    cout << "Time period: [" << number_to_time(time) << " - " << number_to_time(next_activity) << "]: " << activity << endl;
}


void print_single_activity(tree_node* root, const string& activity) {
    int begin_time = 0, next_activity = 0;
    string str;

    //finds the first activity
    next_activity = search(root, begin_time, str, 1);

    //finds and print every occurance of the activity
    while (next_activity != 0 || !root) {
        begin_time = next_activity;
        next_activity = search(root, begin_time, str, 1);

        if (activity == "free") {
            cout << "Time period: [" << number_to_time(0) << " - " << number_to_time(begin_time) << "]: " << activity << endl;
            break;
        }
        else if (str == activity) {
            cout << "Time period: [" << number_to_time(begin_time) << " - " << number_to_time(next_activity) << "]: " << activity << endl;
        }
        else if (!root)     break;
    }
}


void print_all_activities(tree_node* root) {
    if (!root)    return;

    else {

        print_all_activities(root->left);

        cout << "[" << number_to_time(root->time) << "] - " << root->activity << endl;

        print_all_activities(root->right);

    }

}


void delete_tree(tree_node* root) {
    if (!root) return;

    delete_tree(root->left);
    delete_tree(root->right);

    delete root;
}