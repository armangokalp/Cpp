#include <string>
#include <iostream>
using namespace std;
#ifndef SPARSEMATRIX_X
#define SPARSEMATRIX_X

struct cell_node{
    cell_node* right;
    cell_node* left;
    int column;
    int value;
    cell_node(): right(nullptr), left(nullptr){}
    cell_node(int c, int v): right(nullptr), left(nullptr), column(c), value(v){}
};

struct row_node{
    row_node* down;
    cell_node* cell;
    row_node(): cell(nullptr), down(nullptr){}
};

class SparseMatrix {
public:
    SparseMatrix(const string &file_name);
    SparseMatrix(row_node * head, int num_rows, int num_columns);
    SparseMatrix(SparseMatrix&& rhs);
    SparseMatrix(const SparseMatrix& rhs);
    void mat_mul(SparseMatrix& other, SparseMatrix& result, int start_row, int end_row);
    SparseMatrix mat_mul(SparseMatrix& other);
    SparseMatrix(int num_rows, int num_columns);
    SparseMatrix& operator=(const SparseMatrix& rhs);
    ~SparseMatrix();
    friend ostream& operator<<(ostream& out, const SparseMatrix& rhs);
private:
    row_node* head_;
    int num_rows_, num_columns_;
    static void copy(const SparseMatrix& rhs, row_node*& head, int& num_rows, int& num_columns);

};


#endif
