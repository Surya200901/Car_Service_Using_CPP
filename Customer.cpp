// Customer.cpp (implementation)
#include "Customer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <map>

#ifdef TEST_MODE
const std::string CUSTOMER_FILE = "tests/test_customers.txt";
#else
const std::string CUSTOMER_FILE = "customers.txt";
#endif

int nextCustomerId() {
    std::ifstream ifs(CUSTOMER_FILE);
    int maxId = 0;
    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream ss(line);
        std::string idStr;
        if (std::getline(ss, idStr, '|')) {
            try {
                int id = std::stoi(idStr);
                if (id > maxId) maxId = id;
            } catch (...) {}
        }
    }
    return maxId + 1;
}

std::vector<Customer> loadCustomers() {
    std::vector<Customer> list;
    std::ifstream ifs(CUSTOMER_FILE);
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string idStr, name, phone, email;
        if (!std::getline(ss, idStr, '|')) continue;
        try {
            Customer c;
            c.id = std::stoi(idStr);
            std::getline(ss, name, '|');
            std::getline(ss, phone, '|');
            std::getline(ss, email, '|');
            c.name = name;
            c.phone = phone;
            c.email = email;
            list.push_back(c);
        } catch (...) {
            continue; // Skip malformed
        }
    }
    return list;
}

void saveCustomers(const std::vector<Customer>& list) {
    std::map<int, Customer> unique;
    for (const auto& c : list) {
        if (unique.count(c.id) == 0) {
            unique[c.id] = c;  // Keep first occurrence
        }
    }
    std::ofstream ofs(CUSTOMER_FILE, std::ios::trunc);
    for (const auto& p : unique) {
        const auto& c = p.second;
        ofs << c.id << '|' << c.name << '|' << c.phone << '|' << c.email << '\n';
    }
}

void addCustomerInteractive() {
    std::vector<Customer> list = loadCustomers();
    Customer c;
    c.id = nextCustomerId();
    std::cout << "Enter name: "; std::getline(std::cin, c.name);
    std::cout << "Enter phone: "; std::getline(std::cin, c.phone);
    std::cout << "Enter email: "; std::getline(std::cin, c.email);
    list.push_back(c);
    saveCustomers(list);
    std::cout << "Customer added with ID: " << c.id << "\n";
}

void viewCustomers() {
    auto list = loadCustomers();
    if (list.empty()) {
        std::cout << "No customers found.\n";
        return;
    }

    // Calculate max widths
    size_t maxId = 2; // "ID"
    size_t maxName = 4; // "Name"
    size_t maxPhone = 5; // "Phone"
    size_t maxEmail = 5; // "Email"
    for (const auto& c : list) {
        maxId = std::max(maxId, std::to_string(c.id).length());
        maxName = std::max(maxName, c.name.length());
        maxPhone = std::max(maxPhone, c.phone.length());
        maxEmail = std::max(maxEmail, c.email.length());
    }

    // Print header
    std::cout << std::left << std::setw(maxId + 2) << "ID" 
              << std::setw(maxName + 2) << "Name" 
              << std::setw(maxPhone + 2) << "Phone" 
              << std::setw(maxEmail + 2) << "Email" << "\n";

    // Print separator
    std::cout << std::string(maxId + maxName + maxPhone + maxEmail + 8, '-') << "\n";

    // Print rows
    for (const auto& c : list) {
        std::cout << std::left << std::setw(maxId + 2) << c.id 
                  << std::setw(maxName + 2) << c.name 
                  << std::setw(maxPhone + 2) << c.phone 
                  << std::setw(maxEmail + 2) << c.email << "\n";
    }
}

Customer* findCustomerById(std::vector<Customer>& list, int id) {
    for (auto &c : list) if (c.id == id) return &c;
    return nullptr;
}

void searchCustomer() {
    auto list = loadCustomers();
    std::cout << "Enter customer ID to search: ";
    int id; std::cin >> id; std::cin.ignore();
    for (auto &c : list) {
        if (c.id == id) {
            std::cout << "Found: ID="<<c.id<<", Name="<<c.name<<", Phone="<<c.phone<<", Email="<<c.email<<"\n";
            return;
        }
    }
    std::cout << "Customer not found.\n";
}

void updateCustomer() {
    auto list = loadCustomers();
    std::cout << "Enter customer ID to update: ";
    int id; std::cin >> id; std::cin.ignore();
    for (auto &c : list) {
        if (c.id == id) {
            std::cout << "Enter new name (leave blank to keep): ";
            std::string tmp; std::getline(std::cin, tmp); if (!tmp.empty()) c.name = tmp;
            std::cout << "Enter new phone (leave blank to keep): ";
            std::getline(std::cin, tmp); if (!tmp.empty()) c.phone = tmp;
            std::cout << "Enter new email (leave blank to keep): ";
            std::getline(std::cin, tmp); if (!tmp.empty()) c.email = tmp;
            saveCustomers(list);
            std::cout << "Customer updated.\n";
            return;
        }
    }
    std::cout << "Customer not found.\n";
}

void deleteCustomer() {
    auto list = loadCustomers();
    std::cout << "Enter customer ID to delete: ";
    int id; std::cin >> id; std::cin.ignore();
    auto it = std::remove_if(list.begin(), list.end(), [&](const Customer& c){ return c.id == id; });
    if (it != list.end()) {
        list.erase(it, list.end());
        saveCustomers(list);
        std::cout << "Customer deleted.\n";
    } else {
        std::cout << "Customer not found.\n";
    }
}