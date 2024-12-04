#include "solution.h"

int main() {
    std::vector<std::shared_ptr<Product>> products = {std::make_shared<Product>(PerishableProduct("Perishable product", 1.0, 10.0, 30, time(nullptr) + 7 * 24 * 60 * 60)), std::make_shared<Product>(ElectronicProduct("Electronic product", 1.0, 10.0, 30, 12, 65))};
    Warehouse warehouse(products);
    std::cout << warehouse.calculate_storage_fee() << std::endl;
    std::cout << warehouse[0]->price;
    return 0;
}
