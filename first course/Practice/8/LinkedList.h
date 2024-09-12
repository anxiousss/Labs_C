class Node {
public:
    int data;
    Node* next;

    Node(int data);
};

class LinkedList{
private:
    Node* head;

    void clear();

    void copy(const LinkedList& other);

public:
    LinkedList();

    LinkedList(const LinkedList& other);

    bool is_empty() const;

    void append(int data);

    void delete_node(int data);

    void display() const;

    int len() const ;

    void delete_elements(int k);

    ~LinkedList();
};





#ifndef UNTITLED_LINKEDLIST_H
#define UNTITLED_LINKEDLIST_H

#endif //UNTITLED_LINKEDLIST_H
