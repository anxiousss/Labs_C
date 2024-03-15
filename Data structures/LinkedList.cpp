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
        if (head == nullptr) {
            head = new Node(val);
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = new Node(val);
        }
    }

    void display() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    void deleteNode(int val) {
        Node* current = head;
        while (current->next->data != val) {
            current = current -> next;
        }
        Node* next_address = current->next->next;
        delete current->next;
        current = next_address;
    }

    void deleteLast() {
        Node* current = head;
        while (current->next->next != nullptr) {
            current = current -> next;
        }
        delete current -> next ->next;
        current -> next = nullptr;
    }


    bool findNode(int val) const {
        Node* current = head;
        while (current -> next != nullptr) {
            if (current->data == val) return true;
            current = current -> next;
        }
        return false;
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
