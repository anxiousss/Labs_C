
#ifndef EX5_7_SOLUTION_H
#define EX5_7_SOLUTION_H

#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>

#define DAY_MULTIPLITER 0.3
#define FLAMMABILITY_MULTIPLIER 1.3

class Product {
private:
    inline static unsigned int counter = 0;
public:
    std::string name;
    unsigned int id;
    double weight;
    double price;
    unsigned int storage_period;


    Product(const std::string& name,double weight, double price, unsigned int storage_period);

    Product(const Product& product);

    virtual double calculate_storage_fee() const;

    virtual void display_info() const;

    Product& operator=(const Product& product);

    virtual ~Product();

};

class PerishableProduct: public virtual Product {
public:
    time_t expriation_date;

    PerishableProduct(const std::string& name, double weight, double price, unsigned int storage_period, const time_t& expriation_date);

    PerishableProduct(const PerishableProduct& perishable_product);

    PerishableProduct& operator=(const PerishableProduct& perishable_product);

    void display_info() const final;

    double calculate_storage_fee() const final;
};

class ElectronicProduct: public virtual Product {
public:
    unsigned int warranty_period;
    unsigned int power_rating;

    ElectronicProduct(const std::string& name, double weight, double price, unsigned int storage_period, unsigned int warranty_period, unsigned int power_rating);

    ElectronicProduct(const ElectronicProduct& electronic_product);

    ElectronicProduct& operator=(const ElectronicProduct& electronic_product);

    void display_info() const final;
};

class BuildingMaterial: public virtual Product {
public:
    bool flammability;

    BuildingMaterial(const std::string& name, double weight, double price, unsigned int storage_period, bool flammability);

    BuildingMaterial(const BuildingMaterial& building_material);

    BuildingMaterial& operator=(const BuildingMaterial& building_material);

    void display_info() const final;

    double calculate_storage_fee() const final;
};


class Warehouse {
public:
    std::vector<Product*> products;

    explicit Warehouse(const    std::vector<Product*>& products_vector);

    void push_back(Product& product);

    void remove(unsigned int id);

    void find_perishables();

    void find_electronic();

    void find_building();

    double calculate_storage_fee();

    std::vector<PerishableProduct> getExpiringProducts(unsigned int days);

    void display_inventory();

    Warehouse& operator+=(Product& product);

    Warehouse& operator-=(  unsigned int id);

    Product* operator[](unsigned int id);

};

std::ostream& operator<<(std::ostream& ostream, Warehouse warehouse);


#endif //EX5_7_SOLUTION_H
