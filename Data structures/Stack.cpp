#include <iostream>

using namespace std;

struct Stack {
    int size;
    int * data;
    int capacity;

    Stack() : size(0), data(nullptr), capacity(0) {};

    ~Stack() {
        delete[] data;
    }

    void resize() {
        if (capacity == 0) {
            capacity = 1;
        } else {
            capacity *= 2;
        }
        int * old_data = data;
        data = new int[capacity];

        for (int i = 0; i < size; ++i) {
            data[i] = old_data[i];
        }

        delete[] old_data;
    }

    bool is_empty() const {
        return size == 0;
    }

    void push(int el) {
        if (size == capacity)
            resize();

        data[size] = el;
        ++size;
    }

    int top() const {
        return data[size - 1];
    }

    void pop() {
        --size;
    }

    void display() const {
        cout << '[';
        if (!is_empty()) {
            for (int i = 0; i < size - 1; ++i) {
                cout << data[i] << ", ";
            }
            cout  << data[size - 1];
        }
        cout << ')' << endl;
    }

};