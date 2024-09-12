#include <iostream>

using namespace std;

struct Node {
    int val;
    Node * prev;
    Node * next;

    Node(int new_value, Node* p1 = nullptr, Node* p2 = nullptr) : val(new_value), prev(p1), next(p2) {};
};

struct Queue {
    int size;
    Node* first;
    Node* last;

    Queue(): size(0), first(nullptr), last(nullptr) {};

    ~Queue() {
        Node* current = first;
        while (current != nullptr) {
            current = current->next;
            delete[] current -> prev;
        }
    }

    bool is_empty() const {
        return size == 0;
    }

    void push_back(int el) {
        if (is_empty())
            first = last = new Node(el);
        else {
            last = new Node(el, last, nullptr);
            last -> prev -> next = last;
        }
        ++size;
    }

    void push_front(int el) {
        if (is_empty())
            first = last = new Node(el);
        else {
            first = new Node(el, nullptr, first);
            first -> next -> prev = first;
        }
        ++size;
    }

    int pop_back() {
        if (is_empty()) {
            return 0;
        }
        Node* old = last;
        last -> prev -> next = nullptr;
        last = last -> prev;
        int x = old -> val;
        delete old;
        --size;
        return x;
    }

    int pop_front() {
        if (is_empty()) {
            return 0;
        }
        Node* old = first;
        first -> next -> prev = nullptr;
        first = first -> next;
        int x = old -> val;
        delete old;
        --size;
        return x;
    }

    int back() const {
        return last -> val;
    }

    int front() const {
        return first -> val;
    }

    void display() const {
        cout << '[';
        Node* current = first;
        while (current != nullptr) {
            cout << current->val;
            if (current->next != nullptr) {
                cout << ", ";
            }
            current = current->next;
        }
        cout << ')' << endl;
    }

    void copyQueue(const Queue& queue1, Queue queue2) {
        Node* current1 = queue1.first;
        Node* current2 = queue2.first;
        queue2.size = queue1.size;
        while (current1 != nullptr) {
            current2 -> val = current1 -> val;
            current1 = current1 -> next;
            current2 = current1 -> next;
        }
    }
};
