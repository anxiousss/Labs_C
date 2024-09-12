#include <iostream>

#include "LinkedList.h"

Node::Node(int data) {
    this->data = data;
    this->next = nullptr;
}

void LinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

void LinkedList::copy(const LinkedList &other) {
    Node** current = &head;
    Node* otherCurrent = other.head;
    while (otherCurrent != nullptr) {
        *current = new Node(otherCurrent->data);
        current = &((*current)->next);
        otherCurrent = otherCurrent->next;
    }
}

LinkedList::LinkedList() {
    this->head = nullptr;
}

LinkedList::LinkedList(const LinkedList &other) {
    head = nullptr;
    copy(other);
}

bool LinkedList::is_empty() const {
    return head == nullptr;
}

void LinkedList::append(int data) {
    if (this->is_empty()) {
        head = new Node(data);
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new Node(data);
    }
}

void LinkedList::delete_node(int data) {
    if (head == nullptr) {
        return;
    }

    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr && current->data != data) {
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

void LinkedList::display() const{
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data << ' ';
        current = current->next;
    }
    std::cout << std::endl;
}

int LinkedList::len() const {
    Node* current = head;
    int len = 0;
    while (current != nullptr) {
        ++len;
        current = current->next;
    }
    return len;
}

void LinkedList::delete_elements(int k) {
    if (this->len() < k) {
        return;
    }

    Node* current = head;
    Node* prev = nullptr;
    int n_nodes = this->len() - k;
    for (int i = 0; i < n_nodes && current != nullptr; ++i) {
        prev = current;
        current = current->next;
    }

    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    if (prev != nullptr) {
        prev->next = nullptr;
    }
}

LinkedList::~LinkedList() {
    this->clear();
}