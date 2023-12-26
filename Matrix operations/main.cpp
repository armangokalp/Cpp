#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>


struct unknowns {

    int row;
    int column;
    int value;

    unknowns() {}

    unknowns(int _row, int _column, int _value)
        :row(_row), column(_column), value(_value)
    {
    }

};


std::vector<std::vector<unknowns> > solve_equation(std::vector<std::vector<int> > _lhs, std::vector<std::vector<int> > _rhs, std::vector<std::vector<int> > _res) {
    std::vector<int> num(_rhs.size()), nofU_inEachRow;

    std::vector<std::vector<unknowns> > _unknows;
    std::vector<unknowns> urow;

    int col, row, nofU = 0, m = 0, tempM = 1, tempC = 0;    bool even;

    //sum of unknonws of the each row stored in num[]
    for (int i = 0; i < _lhs.size(); i++) {
        num[i] = 0;
        for (int k = 0; k < _lhs[i].size(); k++) {
            if (_lhs[i][k] != -1) {
                num[i] += _lhs[i][k] * _rhs[k][0];
            }
            if (_lhs[i][k] == -1) {
                col = k;
                row = i;
                urow.push_back(unknowns(row, col, 0));
                nofU++;
            }
        }
        num[i] = _res[i][0] - num[i];
        nofU_inEachRow.push_back(nofU);   nofU = 0;
        _unknows.push_back(urow);   urow.clear();
    }

    //_unknows.push_back(urow);
    for (int n = 0; n < _unknows.size(); n++) {
        for (int i = 0; i < _unknows[n].size(); i++) {
            if (nofU_inEachRow[n] % 2 == 0) even = true;
            else even = false;

            for (int q = 1; q <= 9; q++) {
                for (int a = 9; a >= 1; a--) {

                    for (int k = 0; k < nofU_inEachRow[n]; k++) {
                        if (k % 2 == 0) {
                            tempM += _rhs[_unknows[n][k].column][0] * q;
                        }
                        else {
                            tempM += _rhs[_unknows[n][k].column][0] * a;
                        }

                    }
                    if (tempM - 1 == num[n]) {

                        for (int t = 0; t < _unknows[n].size(); t++) {

                            if (_unknows[n][t].value < tempC || tempC == 0) {
                                tempC = _unknows[n][t].value;
                                if (even) {
                                    if (t % 2 == 0) {
                                        _unknows[n][t].value = a;

                                    }
                                    else {
                                        _unknows[n][t].value = q;
                                    }
                                }
                                else {
                                    if (t % 2 == 0) {
                                        _unknows[n][t].value = q;

                                    }
                                    else {
                                        _unknows[n][t].value = a;
                                    }
                                }
                            }
                        
                        }
                    }
                    tempM = 1;

                }
            }


        }

    }


    return _unknows;
}


//reads the file, stores it, performs error checks, and returns the file as 2D matrix
std::vector<std::vector<int> > file_reader(std::vector<std::vector<int> > _matrix, std::vector<int> _cols, std::string _inputName, bool& _error, int funcNumber) {
    if (_error) return _matrix;;
    int row, column;
    std::ifstream input;
    input.open(_inputName);
    if (!input) {
        _error = true;
        return _matrix;
    }
    std::string str;
    getline(input, str);

    row = stoi(str.substr(0, str.find(" ")));
    column = stoi(str.substr(str.find(" "), str.length()));

    //puts file into 2D vector
    int temp, tempCol, tempRow = 0;
    while (getline(input, str)) {
        tempCol = 0;
        std::istringstream s(str);
        while (s >> temp) {
            _cols.push_back(temp);
            tempCol++;
        }
        _matrix.push_back(_cols);
        _cols.clear();
        s.clear();
        tempRow++;
    }

    str.clear();    input.close();


    //Check for LHS, RHS, and RES
    if (row != tempRow || column != tempCol) {
        _error = true;
        return _matrix;
    }

    switch (funcNumber) {
    case 0:
        //Check for LHS
        for (int i = 0; i < row; i++) {
            for (int k = 0; k < column; k++) {
                if (_matrix[k][i] > 9 || _matrix[i][k] < -1 || _matrix[i][k] == 0) {
                    _error = true;
                    return _matrix;
                }
            }
        }
        break;
    case 1:
        //Check for RHS
        if (column != 1) {
            _error = true;
            return _matrix;
        }
        for (int i = 0; i < row; i++) {
            if (_matrix[i][0] < 1 || _matrix[i][0] > 9) {
                _error = true;
                return _matrix;
            }
        }
        break;
    case 2:
        //Check for RES
        if (column != 1) {
            _error = true;
            return _matrix;
        }
        for (int i = 0; i < row; i++) {
            if (_matrix[i][0] < 1) {
                _error = true;
                return _matrix;
            }
        }
        break;
    }

    return _matrix;
}


std::string print_output(std::vector<std::vector<int> > _lhs, std::vector<std::vector<int> > _rhs, std::vector<std::vector<int> > _res, int svalue, std::vector<std::vector<unknowns> > (*solveE)(std::vector<std::vector<int> >, std::vector<std::vector<int> >, std::vector<std::vector<int> >)) {

    std::string result = "";
    int val = 0, count = 1, len = std::to_string(svalue).length();

    std::vector<std::vector<unknowns> > u = solve_equation(_lhs, _rhs, _res);

    //prepares the result to be printed
    for (int k = 0; k < u.size(); k++) {
        for (int j = 0; j < u[k].size(); j++) {
                result += std::to_string(u[k][j].row) + " " + std::to_string(u[k][j].column) + " " + std::to_string(u[k][j].value) + "\n";
        }
    }

    return result;
}



int main() {
    std::ifstream input;  std::ofstream output;  std::string inputName;
    bool error = false;

    std::vector<std::vector<int> > matrix, LHS, RHS, RES;
    std::vector<int > cols;

    std::cout << "Enter LHS matrix filename: ";   std::cin >> inputName;

    LHS = file_reader(matrix, cols, inputName, error, 0);

    std::cout << "Enter RHS matrix filename: ";   std::cin >> inputName;

    RHS = file_reader(matrix, cols, inputName, error, 1);

    std::cout << "Enter RES filename: ";          std::cin >> inputName;

    RES = file_reader(matrix, cols, inputName, error, 2);

    std::vector<std::vector<unknowns> > (*solveE)(std::vector<std::vector<int> >, std::vector<std::vector<int> >, std::vector<std::vector<int> >) = solve_equation;

    std::cout << "Enter output filename: ";       std::cin >> inputName;

    output.open(inputName);

    if (error)  output << "Error";
    else
    {
        output << print_output(LHS, RHS, RES, 481199797, solveE);
    }

    output.close();

}