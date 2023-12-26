#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
using namespace std;

#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(const string& file_name) {
    ifstream fin(file_name);
    if (fin.fail()) {
        throw "File couldn't be opened";
    }
    // actual head_
    head_ = nullptr;
    // head_ of current row
    row_node* curr_head = nullptr;
    // row counter for error checking
    int row = 0;
    fin >> num_rows_ >> num_columns_;
    if (fin.fail()) {
        throw "Invalid number of rows or number of columns";
    }
    string line;
    getline(fin, line);
    while (getline(fin, line)) {
        int col = 0, val;
        stringstream ss(line);
        // if this is the first row
        if (head_ == nullptr) {
            head_ = curr_head = new row_node;
        }
        else {
            curr_head->down = new row_node;
            curr_head = curr_head->down;
        }
        // pointer at last cell added in this row
        cell_node* curr_cell = nullptr;
        while (ss >> val) {
            if (val != 0) {
                cell_node* new_cell = new cell_node;
                new_cell->value = val;
                new_cell->column = col;
                // if first cell in this row
                if (curr_cell == nullptr)
                    curr_head->cell = curr_cell = new_cell;
                else {
                    curr_cell->right = new_cell;
                    curr_cell->right->left = curr_cell;
                    curr_cell = curr_cell->right;
                }
            }
            col++;
            if (col > num_columns_) {
                throw "Too many columns in row " + to_string(row);
            }
        }
        row++;
        if (row > num_rows_) {
            throw "Too many rows";
        }
        if (col < num_columns_) {
            throw "Not enough columns in row " + to_string(row);
        }
    }
    if (row < num_rows_) {
        throw "Not enough rows";
    }
}

SparseMatrix::~SparseMatrix() {
    while (head_ != nullptr) {
        cell_node* cell = head_->cell;
        while (cell != nullptr) {
            cell_node* t = cell->right;
            delete cell;
            cell = t;
        }
        row_node* t = head_->down;
        delete head_;
        head_ = t;
    }
}

ostream& operator<<(ostream& out, const SparseMatrix& rhs) {
    int rows = 0;
    row_node* head = rhs.head_;
    while (head != nullptr) {
        // To know how many zeros to print between cells
        int col = -1;
        cell_node* cell = head->cell;
        while (cell != nullptr) {
            col++;
            // until we get to this cell's column, print zeros
            while (col < cell->column) {
                out << "0 ";
                col++;
            }
            out << cell->value << " ";
            col = cell->column;
            cell = cell->right;
        }
        // If there are more zeros after the last cell in the row,
        // print them
        while (col < rhs.num_columns_ - 1) {
            out << "0 ";
            col++;
        }
        out << endl;
        head = head->down;
        rows++;
    }
    // If there are rows with no cells at all, print zeros for them
    while (rows < rhs.num_rows_) {
        for (int i = 0; i < rhs.num_columns_; i++) {
            out << "0 ";
        }
        out << endl;
        rows++;
    }
    return out;
}

SparseMatrix::SparseMatrix(SparseMatrix&& rhs) {
    head_ = rhs.head_;
    rhs.head_ = nullptr;
    num_rows_ = rhs.num_rows_;
    num_columns_ = rhs.num_columns_;
}

SparseMatrix::SparseMatrix(row_node* head, int num_rows, int num_columns) : head_(head), num_rows_(num_rows), num_columns_(num_columns) {}

SparseMatrix::SparseMatrix(const SparseMatrix& rhs) : head_(nullptr), num_columns_(rhs.num_columns_), num_rows_(rhs.num_rows_) {
    copy(rhs, head_, num_rows_, num_columns_);
}

SparseMatrix& SparseMatrix::operator=(const SparseMatrix& rhs) {
    if (&rhs != this)
        copy(rhs, head_, num_rows_, num_columns_);
    return *this;
}

void SparseMatrix::copy(const SparseMatrix& rhs, row_node*& head, int& num_rows, int& num_columns) {
    num_rows = rhs.num_rows_;
    num_columns = rhs.num_columns_;
    if (rhs.num_rows_ == 0 || rhs.num_columns_ == 0) return;
    row_node* curr = head = new row_node;
    row_node* other_head = rhs.head_;
    while (other_head != nullptr) {
        cell_node* rhs_cell = other_head->cell;
        cell_node* my_cell = nullptr;
        while (rhs_cell != nullptr) {
            cell_node* new_cell = new cell_node(rhs_cell->column, rhs_cell->value);
            if (my_cell == nullptr) my_cell = curr->cell = new_cell;
            else {
                my_cell->right = new_cell;
                my_cell->right->left = my_cell;
                my_cell = my_cell->right;
            }
            rhs_cell = rhs_cell->right;
        }
        if (other_head->down != nullptr) {
            curr->down = new row_node;
            curr = curr->down;
        }
        other_head = other_head->down;
    }
}



void SparseMatrix::mat_mul(SparseMatrix& other, SparseMatrix& result, int start_row, int end_row) {

    row_node* curr_result_row = result.head_;
    for (int i = 0; i < start_row; i++) {
        if (curr_result_row && curr_result_row->down)
            curr_result_row = curr_result_row->down;
    }

    for (row_node* curr_row = head_; curr_row != nullptr && start_row < end_row; curr_row = curr_row->down, start_row++) {
        cell_node* curr_result_cell = nullptr;

        for (int i = 0; i < other.num_columns_; i++) {
            int cell_value = 0;

            for (cell_node* curr_cell = curr_row->cell; curr_cell != nullptr; curr_cell = curr_cell->right) {
                for (cell_node* other_cell = other.head_[curr_cell->column].cell; other_cell != nullptr; other_cell = other_cell->right) {

                    if (other_cell->column == i) {
                        cell_value += curr_cell->value * other_cell->value;
                        break;
                    }
                }
            }

            if (cell_value != 0) {
                if (curr_result_cell == nullptr) {
                    curr_result_row->cell = curr_result_cell = new cell_node(i, cell_value);
                }
                else {
                    curr_result_cell->right = new cell_node(i, cell_value);
                    curr_result_cell->right->left = curr_result_cell;
                    curr_result_cell = curr_result_cell->right;
                }
            }
        }
        curr_result_row = curr_result_row->down;
    }
}

SparseMatrix::SparseMatrix(int num_rows, int num_columns) : num_rows_(num_rows), num_columns_(num_columns) {
    head_ = nullptr;
}

void mulWrapper(SparseMatrix* obj, SparseMatrix& other, SparseMatrix& result, int start_row, int end_row)
{
    obj->mat_mul(other, result, start_row, end_row);
}

SparseMatrix SparseMatrix::mat_mul(SparseMatrix& other) {

    SparseMatrix result(num_rows_, other.num_columns_);

    vector<thread> thread_pool;

    int chunk_size = num_rows_ / 4;
    int start = 0;
    int end = min(0 + chunk_size, num_rows_);

    for (int i = 0; i < num_rows_; i += 1) {
        start = i;
        end = min(i + chunk_size, num_rows_);
        thread_pool.push_back(thread(mulWrapper, this, ref(other), ref(result), start, end));
    }

    for (auto& t : thread_pool) {
        t.join();
    }
    return result;
}




int main() {
    // reads the matrix stored in m1.txt
    SparseMatrix lhs("m1.txt");
    // reads the matrix stored in m2.txt
    SparseMatrix rhs("m2.txt");
    // Stores in the SparseMatrix "result" the result of the following operation
        // lhs * rhs
    SparseMatrix result = lhs.mat_mul(rhs);

    return 0;
}