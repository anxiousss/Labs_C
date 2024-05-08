struct Udt {
    int depth;
    int len;
    int* mas;

    Udt(int len);

    Udt(const Udt& other);

    void resize(int size);

    bool is_empty() const;

    void push_front(int value);

    void push_back(int value);

    void pop_front();

    void pop_back();

    void display() const;

    int size() const;

    void insert_sort();

    void delete_value(int value);

    void max_el_del();

    Udt& operator=(const Udt& other);

    ~Udt();
};


#ifndef UNTITLED_UDT_H
#define UNTITLED_UDT_H

#endif //UNTITLED_UDT_H
