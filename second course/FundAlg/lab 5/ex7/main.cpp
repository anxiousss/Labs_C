#include "solution.h"

int main() {

    PerishableProduct perishableProduct("Perishable product", 1.0, 10.0, 30, time(nullptr) + 7 * 24 * 60 * 60);
    ElectronicProduct electronicProduct("Electronic product", 1.0, 10.0, 30, 12, 65);
    std::vector<Product*> products = {&perishableProduct, &electronicProduct};
    Warehouse warehouse(products);
    std::cout << warehouse;
    return 0;
}
