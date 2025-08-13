// Customer.h
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>

struct Customer {
    int id;
    std::string name;
    std::string phone;
    std::string email;
};

std::vector<Customer> loadCustomers();
void saveCustomers(const std::vector<Customer>& list);
int nextCustomerId();
void addCustomerInteractive();
void viewCustomers();
Customer* findCustomerById(std::vector<Customer>& list, int id);
void searchCustomer();
void updateCustomer();
void deleteCustomer();

#endif // CUSTOMER_H