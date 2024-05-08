#include "LinkedList.h"

int main() {
    LinkedList list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.display();
    list.delete_elements(2);
    list.display();
    return 0;
}