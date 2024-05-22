#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <codecvt>
#include <string>

using namespace std;

std::vector<std::vector<int>> bfs_tree_creating(std::vector<std::vector<int>> matrix, int n, int start) {
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

std::vector<int> d_vertex(std::vector<std::vector<int>> matrix, int n, int start) {
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

std::vector<int> find_components(std::vector<std::vector<int>> matrix, int n) {
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


int main(int argc, char* argv[]) {
    int n;
    std::ifstream in(argv[1]);

    in >> n;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    // ввод матрицы смежности
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            in >> matrix[i][j];
        }
    }

    std::wofstream out(argv[1]);

    out.imbue(locale(locale(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>));

    std::wstring out_text = L"<Text>\nПути из вершины:";

    out << to_wstring(n) << L'\n';
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            out << to_wstring(matrix[i][j]) << L' ';
        }
        out << L'\n';
    }
    out << out_text << L'\n';

    // поиск путей из вершины
    std::vector<int> vertex = d_vertex(matrix, n, 0);
    for (int dv : vertex) {
        out << to_wstring(dv) << L' ';
    }
    out << L'\n';


    out << L"Компоненты связности:" << L"\n";
    // поиск компонент связности
    std::vector<int> components = find_components(matrix, n);
    for (int c : components) {
        out << to_wstring(c) << L' ';
    }

    out << L"\nBFS дерево:" << L"\n";

    // создание bfs дерева
    std::vector<std::vector<int>> tree = bfs_tree_creating(matrix, n, 0);


    wstring rib_colors = L"<Rib_Colors>\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {

            if (tree[i][j] == 1) {
                rib_colors  += to_wstring(i) + L' ' + to_wstring(j) + L' ' + L"красный\n" ;
              
            }
            out << to_wstring(tree[i][j]) << L' ';
        }
        out << L'\n';
    }
    out << rib_colors;
    return 0;
}
