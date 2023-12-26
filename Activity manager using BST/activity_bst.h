#include <string>

using namespace std;
#ifndef ACTIVITY_BST_H
#define ACTIVITY_BST_H

struct tree_node {
    int time; // activity time in 24-hour notation
    // 0 -> 00:00, 1000 -> 10:00, 1312 -> 13:12
    string activity; // activity name
    tree_node *right;
    tree_node *left;
    tree_node *parent;

    tree_node(int t, const string &a) : time(t), activity(a),
                                        right(nullptr), left(nullptr),
                                        parent(nullptr) {}
};

// Add an activity with name 'activity' and time 'time'
void add_activity(tree_node *&root, int time, const string &activity);

// Print the name of the activity occurring at time 'time'
void print_activity(tree_node *root, int time);

// Print the duration of the activity occurring at time 'time'
void print_activity_and_duration(tree_node *root, int time);

// Print the duration of every occurrence of activity 'activity'
void print_single_activity(tree_node *root, const string &activity);

// Print the starting time of every activity
void print_all_activities(tree_node *root);

// Delete the tree pointed at by `root`
void delete_tree(tree_node *root);

#endif //ACTIVITY_BST_H
