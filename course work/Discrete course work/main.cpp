#include <iostream>
#include <vector>
#include <queue>

int n;
std::vector<std::vector<int>> bfs_tree_creating(std::vector<std::vector<int>> matrix, int start){
    std::vector<bool> d(n, false);
    std::queue<int> q;
    q.push(start);
    d[start] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < n; ++i) {
            if (matrix[u][i] == 1 && !d[i]) {
                q.push(i);
                matrix[u][i] = 2;
                d[i] = true;
            }
        }
    }

    std::vector<std::vector<int>> bfs_tree(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] == 2) {
                bfs_tree[i][j] = 1;
                bfs_tree[j][i] = 1;
            }
        }
    }
    return bfs_tree;
}

std::vector<int> d_vertex(std::vector<std::vector<int>> matrix, int start) {
    std::queue<int> q;
    q.push(start);
    std::vector<int> d(n, -1);
    d[start] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < n; ++i) {
            if (matrix[u][i] == 1 && d[i] == -1) {
                q.push(i);
                d[i] = d[u] + 1;
            }
        }
    }
    return d;
}

std::vector<int> find_components(std::vector<std::vector<int>> matrix) {
    int color = 0;
    std::vector<int> d(n, 0);
    for (int i = 0; i < n; ++i) {
        if (!d[i]) {
            std::queue<int> q;
            ++color;
            q.push(i);
            d[i] = color;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int j = 0; j < n; ++j) {
                    if (matrix[u][j] == 1 && !d[j]) {
                        q.push(j);
                        d[j] = color;
                    }
                }
            }
        }
    }
    return d;
}


int main() {
    std::cin >> n;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    // ввод матрицы смежности
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    // поиск путей из вершины
    std::vector<int> vertex = d_vertex(matrix, 0);
    for (int dv: vertex) {
        std::cout << dv << ' ';
    }
    std::cout << std::endl;

    // поиск компонент связности
    std::vector<int> components = find_components(matrix);
    for (int c: components) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;

    // создание bfs дерева
    std::vector<std::vector<int>> tree = bfs_tree_creating(matrix, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << tree[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    return 0;
}
