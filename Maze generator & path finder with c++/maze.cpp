// Made with Visual Studio 2019 v16.11.25
// by Arman Gökalp - 29398

#include "Header.h"


void maze_generator(int M, int N, int K)
{
    // Generate K different mazes
    for (int k = 1; k <= K; k++) {

        vector<vector<bool>> visited(N, vector<bool>(M, false)), vertical(N, vector<bool>(M, true)), horizontal(N, vector<bool>(M, true));

        stack<array<int, 2>> cell;

        std::mt19937 mt{};
        mt.seed(rand());

        int x = mt() % M, y = mt() % N, walls = M * N - 1;;

        // Starting the maze from a random position
        visited[y][x] = true;
        cell.push({ x, y });

        // Beginning the maze generation
        while (walls > 0)
        {
            // Choose a random cell from the stack
            array<int, 2> current_cell = cell.top();
            x = current_cell[0], y = current_cell[1];

            // Looking for unvisited neighbours
            vector<array<int, 2>> unvisited_neighbors;

            if (x > 0 && !visited[y][x - 1]) unvisited_neighbors.push_back({ x - 1, y });
            if (x < M - 1 && !visited[y][x + 1]) unvisited_neighbors.push_back({ x + 1, y });
            if (y > 0 && !visited[y - 1][x]) unvisited_neighbors.push_back({ x, y - 1 });
            if (y < N - 1 && !visited[y + 1][x]) unvisited_neighbors.push_back({ x, y + 1 });

            // Knock down a wall if the cell is unvisited. If visited, backtrack.
            if (!unvisited_neighbors.empty()) 
            {
                int i = mt() % unvisited_neighbors.size();
                int nx = unvisited_neighbors[i][0], ny = unvisited_neighbors[i][1];
                if (nx == x - 1) vertical[y][x - 1] = false;
                else if (nx == x + 1) vertical[y][x] = false;
                else if (ny == y - 1) horizontal[y - 1][x] = false;
                else horizontal[y][x] = false;
                visited[ny][nx] = true;
                cell.push({ nx, ny });
                walls--;
            }
            else cell.pop();

        }

        // Print out the maze
        string filename = "maze_" + to_string(k) + ".txt";
        ofstream out(filename);

        out << M << " " << N << endl;

        for (int y = 0; y < N; y++) {
            for (int x = 0; x < M; x++) {
                out << "x=" << y << " y=" << x << " l=" << ((x > 0 && vertical[y][x - 1]) || y == 0 ? "1" : "0") << " r=" << ((x < M - 1 && vertical[y][x]) || y == N-1 ? "1" : "0") << " u=" << ((y > 0 && horizontal[y - 1][x]) || x == M-1 ? "1" : "0") << " d=" << ((y < N - 1 && horizontal[y][x]) || x == 0 ? "1" : "0") << endl;
            }
        }

        out.close();

    }
}


void find_path(int mazeID, int entryX, int entryY, int exitX, int exitY)
{
    string file_name = "maze_" + to_string(mazeID) + ".txt";
    ifstream input(file_name);

    int M, N;
    input >> M >> N;

    vector<vector<bool>> walls(N, vector<bool>(M * 2, true));
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < M * 2; x++) {
            int wall;
            input >> wall;
            walls[y][x] = (wall == 1);
        }
    }

    // Entry coordinates are set
    stack<array<int, 2>> path_stack;
    path_stack.push({ entryX, entryY });
    visited[entryY][entryX] = true;

    // Looking for the exit
    while (!path_stack.empty()) {
        // Current cell
        array<int, 2> current_cell = path_stack.top();
        int x = current_cell[0], y = current_cell[1];

        // Write the coordiantes if the exit is reached
        if (x == exitX && y == exitY) {

            string path_file_name = "maze_" + to_string(mazeID) + "_path_" + to_string(exitY) + "_" + to_string(exitX) + "_" + to_string(entryY) + "_" + to_string(entryX) + ".txt";
            ofstream out(path_file_name);

            while (!path_stack.empty()) {
                array<int, 2> cell = path_stack.top();
                out << cell[0] << " " << cell[1] << endl;
                path_stack.pop();
            }

            out.close();
            return;
        }

        // Find neighbor cells
        bool found = false;
        for (int dir = 1; dir <= 4; dir++) {
            int nx = x, ny = y;
            switch (dir) {
            case 1: nx--; break;
            case 2: nx++; break;
            case 3: ny--; break;
            case 4: ny++; break;
            }

            // Check if the cell is unvisited
            if (nx >= 0 && nx < M && ny >= 0 && ny < N && !walls[ny][x * 2 + (dir - 1) % 2] && !visited[ny][nx]) {
                path_stack.push({ nx, ny });
                visited[ny][nx] = true;
                found = true;
                break;
            }
        }

        // Backtrack
        if (!found) {
            path_stack.pop();
        }
    }
}



int main()
{
    //  Maze generation.
    int maze_quantity;
    cout << "Enter the number of mazes: "; cin >> maze_quantity;

    int row_number, col_number;
    cout << "Enter the number of rows and columns (M and N): "; cin >> row_number >> col_number;

    maze_generator(row_number, col_number, maze_quantity);

    cout << "All mazes are generated. \n";

    // Finding a path for given entry and exit points.
    int maze_id;
    cout << "Enter a maze ID between 1 to " << maze_quantity << " inclusive to find a path: "; cin >> maze_id;
         
    int entry_x, entry_y;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): "; cin >> entry_x >> entry_y;

    int exit_x, exit_y;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): "; cin >> exit_x >> exit_y;

    find_path(maze_id, exit_y, exit_x, entry_y, entry_x);
}
