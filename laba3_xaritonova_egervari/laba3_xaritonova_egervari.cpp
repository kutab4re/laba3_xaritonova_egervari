#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

// function to subtract the minimum element from each row of the matrix
void subtract_min_from_rows(vector<vector<int>>& cost) {
    for (auto& row : cost) {
        // looking for minimum element in the row
        int row_min = *min_element(row.begin(), row.end());
        // Subtract the minimum element from each element of the row
        for (auto& val : row) val -= row_min;
    }
}

// function to subtract the minimum element from each column of the matrix
void subtract_min_from_cols(vector<vector<int>>& cost) {
    for (size_t col = 0; col < cost[0].size(); ++col) {
        // looking for minimum element in the column
        int col_min = INF;
        for (size_t row = 0; row < cost.size(); ++row) {
            col_min = min(col_min, cost[row][col]);
        }
        // subtract the minimum element from each element of the column
        for (size_t row = 0; row < cost.size(); ++row) {
            cost[row][col] -= col_min;
        }
    }
}

// function to try to find an augmenting path in the graph
bool try_kuhn(int v, const vector<vector<int>>& cost, vector<int>& used, vector<int>& matching) {
    if (used[v]) return false; // vertex already visited
    used[v] = true; // visited summits label
    for (size_t j = 0; j < cost[v].size(); ++j) {
        // if there is a free edge and an augmenting path is found
        if (cost[v][j] == 0 && (matching[j] == -1 || !used[matching[j]] && try_kuhn(matching[j], cost, used, matching))) {
            matching[j] = v; // update the pair for vertex j
            return true;
        }
    }
    return false; // augmenting path not found
}


// main function to solve
void hungarian_algorithm(const vector<vector<int>>& original_cost) {
    size_t n = original_cost.size();
    size_t m = original_cost[0].size();
    vector<vector<int>> cost = original_cost;
    // row and column conversion
    subtract_min_from_rows(cost);
    subtract_min_from_cols(cost);
    // create an array to store pairs
    vector<int> matching(m, -1);
    // try to find an augmenting path for each vertex
    for (size_t row = 0; row < n; ++row) {
        vector<int> used(n, 0);
        try_kuhn(row, cost, used, matching);
    }
    // restore the answer
    int total_cost = 0;
    cout << "Distribution:\n";
    for (size_t j = 0; j < m; ++j) {
        if (matching[j] != -1) {
            cout << "Vertex " << matching[j] + 1 << " relates to " << j + 1 << endl;
            // increase the total cost of assignment
            total_cost += original_cost[matching[j]][j];
        }
    }
    cout << "Total cost: " << total_cost << endl;
}

int main() {
    int n, m;
    cout << "Enter matrix size NxK: ";
    cin >> n >> m;
    vector<vector<int>> cost(n, vector<int>(m));

    cout << "Enter the matrix:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> cost[i][j];
        }
    }
    // call the function
    hungarian_algorithm(cost);
    return 0;
}
