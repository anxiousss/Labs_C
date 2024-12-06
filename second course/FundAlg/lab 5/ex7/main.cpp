#include "solution.h"

int main()
{
    std::vector<std::shared_ptr<Product>> products;
    products.reserve(100);
    products.push_back(std::shared_ptr<Product>(new PerishableProduct("Perishable product", 1.0, 10.0, 30, time(nullptr) + 7 * 24 * 60 * 60)));
    products.push_back(std::shared_ptr<Product>(new ElectronicProduct("Electronic product", 1.0, 228.0, 30, 12, 65)));
    Warehouse warehouse(products);
    std::cout << warehouse.calculate_storage_fee() << std::endl;
    std::cout << warehouse[1]->price << std::endl;
    return 0;
}
