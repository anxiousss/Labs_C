#include <iostream>


class Node {
public:
    int data;
    Node* next;

    Node(int data) {
        this->data = data;
        this->next = nullptr;
    }
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() {
        head = nullptr;
    }

    bool is_list_empty() const {
        return head == nullptr;
    }

    void append(int val) {
        if (is_list_empty()) {
            head = new Node(val);
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new Node(val);
        }
    }

    void deleteNode(int val) {
        if (head == nullptr) {
            return;
        }

        Node* current = head;
        Node* previous = nullptr;


        while (current != nullptr && current->data != val) {
            previous = current;
            current = current->next;
        }


        if (current == nullptr) {
            return;
        }
        if (previous == nullptr) {
            head = current->next;
        } else {
            previous->next = current->next;
        }

        delete current;
    }


    void printNode(Node* node) const {
        if (node == nullptr) return;
        std::cout << node << ' ' << node->data << ' ' << node->next << std::endl;
    }

    void display() const {
        Node *current = head;
        while (current != nullptr) {
            printNode(current);
            current = current -> next;
        }
    }

    void deleteLast() {
        Node* current = head;
        while (current->next->next != nullptr) {
            current = current -> next;
        }
        delete current -> next ->next;
        current -> next = nullptr;
    }

    Node* findNode(int val) const {
        Node* current = head;
        while (current -> data != val && current -> next != nullptr) {
            current = current -> next;
        }
        if (current -> data == val) return current;
        throw std::exception();
    }

    Node* findNext(Node* ptn) const {
        Node* current = head;
        while (current -> next != ptn && current -> next != nullptr) {
            current = current -> next;
        }
        if (current -> next == ptn) return current;
        throw std::exception();
    }

    Node* findPrev(Node* ptn) const {
        Node* current = head;
        while (current -> next != ptn && current -> next != nullptr) {
            current = current -> next;
        }
        if (current -> next == ptn) return current;
        throw std::exception();
    }

    void insertNext(Node* current, int val) {
        Node* next_address = current -> next;
        current->next = new Node(val);
        current->next->next = next_address;
    }

    void insertPrev(Node* current, int val) {
        Node* prev = findPrev(current);
        insertNext(prev, val);
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};
