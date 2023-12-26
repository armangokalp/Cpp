#include <string>
using namespace std;

#ifndef SPARSE_MATRIX_2D_LINKED_LIST_H
#define SPARSE_MATRIX_2D_LINKED_LIST_H

struct cell_node{
    cell_node* right;
    cell_node* left;
    int column;
    int value;
    cell_node(): right(nullptr), left(nullptr){}
};

struct row_node{
    row_node* down;
    cell_node* cell;
    row_node(): cell(nullptr), down(nullptr){}
};

// Reads the matrix stored in `filename` and returns a `row_node` pointer
// pointing at its head. Also sets `num_rows` and `num_columns` to the number
// of rows and columns in the read matrix.
row_node* read_matrix(const string& filename, int& num_rows, int& num_columns);

// Prints the matrix pointed at by `head` with `num_rows` rows and `num_columns` columns
// to console output (cout)
void print_matrix(row_node* head, int num_rows, int num_columns);

// deletes the data of the matrix pointed at by `head`
void delete_matrix(row_node* head);

// Adds two matrices, one pointed at by `left` and the other pointed at by `right`.
// Both matrices will have `num_rows` rows, and `num_columns` columns.
row_node* add_matrices(row_node* left, row_node* right, int num_rows, int num_columns);

// Checks if the matrix pointed at by `head` with `num_rows` rows and `num_column` columns
// is symmetric or not.
bool is_symmetric(row_node* head, int num_rows, int num_columns);

// Generate a new sparse matrix that is the transpose of the matrix pointed at by
// `head` with `num_rows` rows and `num_columns` columns
row_node* transpose(row_node* head, int num_rows, int num_columns);


#endif //SPARSE_MATRIX_2D_LINKED_LIST_H
