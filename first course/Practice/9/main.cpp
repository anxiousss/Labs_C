#include <iostream>

struct Node {
    int key;
    int value;
};

void shellSort(Node arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            Node temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap].key > temp.key; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

int main() {
    Node arr[] = {{10, 20}, {5, 10}, {15, 30}, {2, 5}, {12, 13}};
    int n = sizeof(arr) / sizeof(arr[0]);

    shellSort(arr, n);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i].key << " " << arr[i].value << std::endl;
    }

    return 0;
}