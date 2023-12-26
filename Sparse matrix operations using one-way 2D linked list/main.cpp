//Arman Gökalp
//11.03.2022
//Visual Studio 2019


#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "sparse_matrix_2d_linked_list.h"
using namespace std;

bool structure_is_okay(row_node* head, int num_rows, int num_columns) {
    int rows = 0;
    while (head != nullptr) {
        if (rows >= num_rows) {
            cout << "Too many rows!\n";
            return false;
        }
        cell_node* cell = head->cell;
        if (cell != nullptr) {
            if (cell->left != nullptr) {
                cout << "Bad first node!\n";
                return false;
            }
            while (cell != nullptr) {
                if (cell->right != nullptr) {
                    if (cell->right->left != cell) {
                        cout << "Bad connection!\n";
                        return false;
                    }
                    if (cell->right->column <= cell->column) {
                        cout << "Columns should be increasing in a row!\n";
                        return false;
                    }
                }
                if (cell->value == 0) {
                    cout << "You can't store zeros in cells!\n";
                    return false;
                }
                if (cell->column >= num_columns) {
                    cout << "Too many columns at row " << rows << "!\n";
                    return false;
                }
                cell = cell->right;
            }
        }
        head = head->down;
        rows++;
    }
    if (rows < num_rows) {
        cout << "Too few rows!\n";
        return false;
    }
    return true;
}



row_node* read_matrix(string& filename, int& num_rows, int& num_columns) {
    ifstream input(filename);
    string str, s, temp;
    getline(input, str);
    num_rows = stoi(str.substr(0));
    num_columns = stoi(str.substr(str.find(" "), str.length()));
    int col = 0;
    int row = 0;

    row_node* head = new row_node;
    row_node* real_head = head;
    cell_node* previous;
    while (getline(input, str)) {
        cell_node* cell = new cell_node;
        cell->left = nullptr;
        cell->right = nullptr;
        cell->column = 0;   cell->value = 0;
        head->cell = cell;
        previous = nullptr;
        col = 0;
        istringstream s(str);
        while (s >> temp) {
            if (temp != "0") {
                cell->left = previous;
                cell->column = col;
                cell->value = stoi(temp);
                if (col + 1 < num_columns) {
                    cell->right = new cell_node;
                    cell->right->right = nullptr;
                    previous = cell;
                    cell = cell->right;
                }
            }
            col++;
        }
        cout << cell->column << "\t" << endl;
        if (row + 1 < num_rows) {
            head->down = new row_node;
            head = head->down;
            row++;
        }
        if (head->cell && head->cell->value == 0) {
            delete head->cell;
            head->cell = nullptr;
        }

    }

    input.close();
    return real_head;
}

void print_matrix(row_node* head, int num_rows, int num_columns) {

    cell_node* cell = NULL;
    for (int i = 0; i < num_rows; i++) {
        if (head)  cell = head->cell;
        for (int k = 0; k < num_columns; k++) {

            if (cell && cell->column == k) {
                cout << cell->value << "\t";
                cell = cell->right;
            }
            else cout << 0 << "\t";
        }
        cout << endl;
        head = head->down;
    }

}

void delete_matrix(row_node* head) {
    row_node* temp = head;
    cell_node* h = NULL, * ht = h;
    while (temp) {
        ht = temp->cell;
        while (ht) {
            h = ht;
            ht = ht->right;
            delete h;
        }
        temp = temp->down;
        delete head;
        head = temp;
    }
    temp = nullptr;     ht = nullptr;
}

row_node* add_matrices(row_node* left, row_node* right, int num_rows, int num_columns) {
    row_node* head = new row_node, * real_head = head;
    row_node* left_head = left, * right_head = right;
    cell_node* previous = nullptr;
    int row = 0, col = 0;

    while (row < num_rows) {
        cell_node* cell = new cell_node, * cell_head = cell;
        cell_node* lcell_head = left->cell, * rcell_head = right->cell;
        head->cell = cell;
        col = 0;
        previous = nullptr;
        while (col < num_columns) {
            if ((left->cell && left->cell->column == col) || (right->cell && right->cell->column == col)) {

                cell->value = 0;
                if (left->cell && left->cell->column == col)   cell->value += left->cell->value;
                if (right->cell && right->cell->column == col)   cell->value += right->cell->value;



                cell->column = col;   cell->right = nullptr;  cell->left = previous;

                if (col + 1 < num_columns) {

                    if (cell != nullptr) {
                        cell->right = new cell_node;
                        previous = cell;
                        cell = cell->right;
                    }

                    if (left->cell && left->cell->column == col) left->cell = left->cell->right;
                    if (right->cell && right->cell->column == col) right->cell = right->cell->right;
                }
            }
            col++;
        }

        left->cell = lcell_head;
        right->cell = rcell_head;

        cell = cell_head;

        if (row + 1 < num_rows) {
            head->down = new row_node;
            head = head->down;
            if (left->down) left = left->down;
            if (right->down) right = right->down;
        }
        row++;

        if (head->cell && head->cell->value == 0) {
            delete head->cell;
            head->cell = nullptr;
        }
    }

    left = left_head;
    right = right_head;

    return real_head;
}

bool is_symmetric(row_node* head, int num_rows, int num_columns) {
    row_node* real_head = head;
    int row = 0, col = 0, v1 = 0, v2 = 0, c1 = 0, c2 = 0;

    while (row < num_rows) {
        cell_node* cell_head = head->cell;
        col = 0;
        while (col < int(num_columns / 2)) {

            v1 = head->cell->value;
            c1 = head->cell->column;

            cell_node* temp_head = head->cell;
            for (int i = c1; i < num_columns - c1; i++) {
                if (head->cell->right) head->cell = head->cell->right;
            }
            v2 = head->cell->value;
            c2 = head->cell->column;
            head->cell = temp_head;

            if (!(v1 == v2 && c1 + c2 == num_columns - 1))   return 0;

            head->cell = head->cell->right;

            col++;
        }

        head->cell = cell_head;

        head = head->down;

        row++;
    }


    head = real_head;

    return 1;
}

row_node* transpose(row_node* head, int num_rows, int num_columns) {
    row_node* real_head = head;

    return real_head;
}


int main() {
    /////// Timing data ///////
    chrono::time_point<chrono::system_clock> start, end;
    chrono::duration<double> total_time;
    //////////////////////////
    string filename1, filename2;
    int m1_rows, m1_cols, m2_rows, m2_cols;
    cout << "Matrix 1 filename: ";
    cin >> filename1;

    cout << "Matrix 2 filename: ";
    cin >> filename2;

    row_node* m1 = read_matrix(filename1, m1_rows, m1_cols);
    structure_is_okay(m1, m1_rows, m1_cols);

    row_node* m2 = read_matrix(filename2, m2_rows, m2_cols);
    structure_is_okay(m2, m2_rows, m2_cols);

    cout << "m1\n";
    print_matrix(m1, m1_rows, m1_cols);

    cout << "m2\n";
    print_matrix(m2, m2_rows, m2_cols);

    if (m1_rows == m2_rows && m1_cols == m2_cols) {
        start = chrono::system_clock::now();
        row_node* m3 = add_matrices(m1, m2, m1_rows, m1_cols);
        end = chrono::system_clock::now();
        total_time = end - start;
        cout << "Time to add matrices: " << total_time.count() << " seconds\n";

        structure_is_okay(m3, m2_rows, m2_cols);

        cout << "m3\n";
        print_matrix(m3, m2_rows, m2_cols);
        delete_matrix(m3);
    }
    start = chrono::system_clock::now();
    bool symmetric = is_symmetric(m1, m1_rows, m1_cols);
    end = chrono::system_clock::now();
    total_time = end - start;
    cout << "Time to check if m1 is symmetric: " << total_time.count() << " seconds\n";

    if (symmetric)
        cout << "Matrix m1 is symmetric.\n";
    else
        cout << "Matrix m1 is not symmetric.\n";

    start = chrono::system_clock::now();
    symmetric = is_symmetric(m2, m2_rows, m2_cols);
    end = chrono::system_clock::now();
    total_time = end - start;
    cout << "Time to check if m2 is symmetric: " << total_time.count() << " seconds\n";

    if (symmetric)
        cout << "Matrix m2 is symmetric.\n";
    else
        cout << "Matrix m2 is not symmetric.\n";


    delete_matrix(m1);
    delete_matrix(m2);

}