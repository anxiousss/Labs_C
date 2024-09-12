#include "UDT.h"


int main() {
    Udt udt(10);
    udt.push_back(1);
    udt.push_back(0);
    udt.push_back(3);
    udt.push_back(-1);
    udt.display();
    udt.max_el_del();
    udt.display();
    return 0;
}