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

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }

    void copy(const LinkedList& other) {
        Node** current = &head;
        Node* otherCurrent = other.head;
        while (otherCurrent != nullptr) {
            *current = new Node(otherCurrent->data);
            current = &((*current)->next);
            otherCurrent = otherCurrent->next;
        }
    }

public:
    LinkedList() {
        head = nullptr;
    }

    LinkedList(const LinkedList& other) {
        copy(other);
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

    void appendBegin(int val) {
        Node* newNode = new Node(val);
        newNode -> next = head;
        head = newNode;
    }

    void swap(int x, int y) {
        Node *prevX = nullptr, *currX = head;
        while (currX != nullptr && currX->data != x) {
            prevX = currX;
            currX = currX->next;
        }

        Node *prevY = nullptr, *currY = head;
        while (currY != nullptr && currY->data != y) {
            prevY = currY;
            currY = currY->next;
        }

        if (currX == nullptr || currY == nullptr) return;

        if (prevX != nullptr) {
            prevX->next = currY;
        } else {
            head = currY;
        }


        if (prevY != nullptr) {
            prevY->next = currX;
        } else {
            head = currX;
        }

        Node* temp = currY->next;
        currY->next = currX->next;
        currX->next = temp;

    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            copy(other);
        }
        return *this;
    }

    bool operator==(const LinkedList& other) const {
        Node* current1 = head;
        Node* current2 = other.head;

        while (current1 != nullptr && current2 != nullptr) {
            if (current1->data != current2->data) {
                return false;
            }
            current1 = current1->next;
            current2 = current2->next;
        }

        return current1 == nullptr && current2 == nullptr;
    }


    ~LinkedList() {
        clear();
    }
};
