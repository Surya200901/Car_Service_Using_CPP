// Customer.h
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>

/**
 * @brief Represents a customer in the car service management system.
 */
struct Customer {
    int id;           /**< Unique identifier for the customer. */
    std::string name; /**< Full name of the customer. */
    std::string phone; /**< Phone number of the customer. */
    std::string email; /**< Email address of the customer. */
};

/**
 * @brief Loads all customers from the customer file.
 * @return std::vector<Customer> A vector containing all valid customer records.
 */
std::vector<Customer> loadCustomers();

/**
 * @brief Saves a list of customers to the customer file.
 * @param list A vector of Customer objects to save.
 */
void saveCustomers(const std::vector<Customer>& list);

/**
 * @brief Determines the next available customer ID.
 * @return int The next available customer ID.
 */
int nextCustomerId();

/**
 * @brief Interactively adds a new customer to the customer file.
 */
void addCustomerInteractive();

/**
 * @brief Displays all customers in a formatted table.
 */
void viewCustomers();

/**
 * @brief Finds a customer by ID in a vector of customers.
 * @param list A vector of Customer objects to search.
 * @param id The ID of the customer to find.
 * @return Customer* Pointer to the found customer, or nullptr if not found.
 */
Customer* findCustomerById(std::vector<Customer>& list, int id);

/**
 * @brief Searches for a customer by ID and displays their details.
 */
void searchCustomer();

/**
 * @brief Updates an existing customer's details interactively.
 */
void updateCustomer();

/**
 * @brief Deletes a customer by ID from the customer file.
 */
void deleteCustomer();

#endif // CUSTOMER_H