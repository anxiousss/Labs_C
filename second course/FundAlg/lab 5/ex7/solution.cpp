#include "solution.h"


Product::Product(const std::string &name, double weight, double price, unsigned int storage_period): name{name}, weight(weight), price(price), storage_period(storage_period){
    ++counter;
    id = counter;
}

Product::Product(const Product& product) {
    name = product.name;
    ++counter;
    id = counter;
    weight = product.weight;
    price = product.price;
    storage_period = product.storage_period;
}

Product& Product::operator=(const Product &product) {
    name = product.name;
    ++counter;
    id = counter;
    weight = product.weight;
    price = product.price;
    storage_period = product.storage_period;
    return *this;
}

double Product::calculate_storage_fee() const {
    return price * weight;
}

void Product::display_info() const {
    std::cout << "ID: "<< id << " NAME: " << name << " WEIGHT: " << weight << " PRICE: " << price << " PERIOD: " << storage_period;
}

Product::~Product() = default;


PerishableProduct::PerishableProduct(const std::string &name, double weight, double price,
                                     unsigned int storage_period, const time_t& expriation_date): Product(name, weight, price, storage_period), expriation_date(expriation_date) {}

PerishableProduct::PerishableProduct(const PerishableProduct &perishable_product): Product(perishable_product), expriation_date(perishable_product.expriation_date) {}

PerishableProduct &PerishableProduct::operator=(const PerishableProduct &perishable_product) {
    Product::operator=(perishable_product);
    expriation_date = perishable_product.expriation_date;
    return *this;
}

double PerishableProduct::calculate_storage_fee() const {
    double multiplier = difftime(expriation_date, time(nullptr)) / 60 / 60 / 24;
    if (multiplier < 0) multiplier = 0;
    return Product::calculate_storage_fee() * (1 + (multiplier - storage_period) * DAY_MULTIPLITER);
}

void PerishableProduct::display_info() const {
    Product::display_info();
    std::cout << " EXPRIATION DATE: " << expriation_date;
}

ElectronicProduct::ElectronicProduct(const std::string &name, double weight, double price,
                                     unsigned int storage_period,  unsigned int warranty_period,
                                     unsigned int power_rating): Product(name, weight, price, storage_period), warranty_period(warranty_period), power_rating(power_rating) {}

ElectronicProduct::ElectronicProduct(const ElectronicProduct &electronic_product): Product(electronic_product), warranty_period(electronic_product.warranty_period), power_rating(electronic_product.power_rating) {}

ElectronicProduct &ElectronicProduct::operator=(const ElectronicProduct &electronic_product) {
    Product::operator=(electronic_product);
    warranty_period = electronic_product.warranty_period;
    power_rating = electronic_product.power_rating;
    return *this;
}

void ElectronicProduct::display_info() const {
    Product::display_info();
    std::cout << " WARRANTY PERIOD: " << warranty_period << " POWER RATING: " << power_rating;
}


BuildingMaterial::BuildingMaterial(const std::string &name, const double weight, const double price,
                                   const unsigned int storage_period, const bool flammability): Product(name, weight, price, storage_period), flammability(flammability) {}

BuildingMaterial::BuildingMaterial(const BuildingMaterial &building_material): Product(building_material), flammability(building_material.flammability) {}

BuildingMaterial &BuildingMaterial::operator=(const BuildingMaterial &building_material) {
    Product::operator=(building_material);
    flammability = building_material.flammability;
    return *this;
}

double BuildingMaterial::calculate_storage_fee() const {
    return Product::calculate_storage_fee() * (flammability ? FLAMMABILITY_MULTIPLIER: 1);
}

void BuildingMaterial::display_info() const {
    Product::display_info();
    std::cout << " FLAMMABILTY: " << (flammability ? "YES": "NO");
}


Warehouse::Warehouse(const std::vector<Product*>& products_vector) {
    products = products_vector;
}

void Warehouse::push_back(Product &product) {
    products.push_back(&product);
}

void Warehouse::remove(unsigned int id) {
    products.erase(products.begin() + id);
}

double Warehouse::calculate_storage_fee() {
    double total = 0;
    for (const auto & product : products) {
        total += product->calculate_storage_fee();
    }
    return total;
}

std::vector<PerishableProduct> Warehouse::getExpiringProducts( unsigned int days) {
    std::vector<PerishableProduct> expiring_products;
    for (const auto& product: products) {
        if (auto* perishableProduct = dynamic_cast<PerishableProduct*>(product)) {
            double diff = difftime(time(nullptr), perishableProduct->expriation_date) / 60 / 60 / 24;
            if (diff < days) {
                expiring_products.push_back(*perishableProduct);
            }
        }
    }
    return expiring_products;
}

Warehouse& Warehouse::operator+=(Product &product) {
    Warehouse::push_back(product);
    return *this;
}

Warehouse& Warehouse::operator-=(unsigned int id) {
    Warehouse::remove(id);
    return *this;
}

Product* Warehouse::operator[](unsigned int id) {
    for (const auto& product: products) {
        if (product->id == id) {
            return product;
        }
    }
    return nullptr;
}

void Warehouse::find_perishables() {
    for (const auto& product: products) {
        if (auto* perishableProduct = dynamic_cast<PerishableProduct*>(product)) {
            perishableProduct->display_info();
            std::cout << std::endl;
        }
    }
}

void Warehouse::find_electronic() {
    for (const auto& product: products) {
        if (auto* perishableProduct = dynamic_cast<ElectronicProduct*>(product)) {
            perishableProduct->display_info();
            std::cout << std::endl;
        }
    }
}

void Warehouse::find_building() {
    for (const auto& product: products) {
        if (auto* perishableProduct = dynamic_cast<BuildingMaterial*>(product)) {
            perishableProduct->display_info();
            std::cout << std::endl;
        }
    }
}

void Warehouse::display_inventory() {
    std::cout << "PERISHABLES" << std::endl << std::endl;
    find_perishables();
    std::cout << "ELECTRONIC" << std::endl << std::endl;
    find_electronic();
    std::cout << "BUILDING" << std::endl << std::endl;
    find_building();
}

std::ostream& operator<<(std::ostream& ostream, Warehouse warehouse) {
    warehouse.display_inventory();
    return ostream;
}
