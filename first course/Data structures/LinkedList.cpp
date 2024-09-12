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
        head = nullptr;
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
        Node* current = head;
        while (current != nullptr) {
            printNode(current);
            current = current->next;
        }
    }

    void deleteLast() {
        if (head == nullptr) {
            return;
        }
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            return;
        }
        Node* current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
    }

    Node* findNode(int val) const {
        Node* current = head;
        while (current != nullptr && current->data != val) {
            current = current->next;
        }
        return current;
    }

    Node* findNext(Node* ptn) const {
        if (ptn == nullptr || ptn->next == nullptr) {
            return nullptr;
        }
        return ptn->next;
    }

    Node* findPrev(Node* ptn) const {
        if (ptn == head || ptn == nullptr) {
            return nullptr;
        }
        Node* current = head;
        while (current != nullptr && current->next != ptn) {
            current = current->next;
        }
        return current;
    }

    void insertNext(Node* current, int val) {
        if (current == nullptr) {
            return;
        }
        Node* newNode = new Node(val);
        newNode->next = current->next;
        current->next = newNode;
    }

    void insertPrev(Node* current, int val) {
        if (current == head || current == nullptr) {
            return;
        }
        Node* prev = findPrev(current);
        if (prev != nullptr) {
            insertNext(prev, val);
        }
    }

    void appendBegin(int val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
    }

    void swap(int x, int y) {
        if (x == y) {
            return;
        }

        Node *prevX = nullptr, *currX = head;
        while (currX != nullptr) {
            if (currX->data == x) {
                break;
            }
            prevX = currX;
            currX = currX->next;
        }

        Node *prevY = nullptr, *currY = head;
        while (currY != nullptr) {
            if (currY->data == y) {
                break;
            }
            prevY = currY;
            currY = currY->next;
        }

        if (currX == nullptr || currY == nullptr) {
            return;
        }

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
