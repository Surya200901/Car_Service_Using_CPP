// Service.h
#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <vector>

/**
 * @brief Represents a service offered in the car service management system.
 */
struct ServiceItem {
    int id;             /**< Unique identifier for the service. */
    std::string name;   /**< Name of the service. */
    double price;       /**< Price of the service in rupees. */
};

/**
 * @brief Represents a service history entry in the car service management system.
 */
struct ServiceHistory {
    int historyId;                 /**< Unique identifier for the service history entry. */
    int customerId;                /**< ID of the customer associated with the service. */
    int vehicleId;                 /**< ID of the vehicle associated with the service. */
    std::vector<int> serviceIds;   /**< List of service IDs included in this history entry. */
    std::string dateTime;         /**< Date and time of the service in "YYYY-MM-DD HH:MM:SS" format. */
    double subtotal;               /**< Subtotal cost of the services before discount. */
    int discountId;                /**< ID of the applied discount (-1 for none). */
    double discountPercent;        /**< Percentage of the applied discount. */
    double total;                  /**< Total cost after applying the discount. */
    std::string status;           /**< Status of the service ("Pending" or "Completed"). */
};

/**
 * @brief Loads all services from the services file.
 * @return std::vector<ServiceItem> A vector containing all valid service records.
 */
std::vector<ServiceItem> loadServices();

/**
 * @brief Saves a list of services to the services file.
 * @param list A vector of ServiceItem objects to save.
 */
void saveServices(const std::vector<ServiceItem>& list);

/**
 * @brief Determines the next available service ID.
 * @return int The next available service ID.
 */
int nextServiceId();

/**
 * @brief Determines the next available service history ID.
 * @return int The next available service history ID.
 */
int nextHistoryId();

/**
 * @brief Ensures default services are present in the services file.
 */
void ensureDefaultServices();

/**
 * @brief Interactively adds a new service to the services file.
 */
void addServiceInteractive();

/**
 * @brief Displays all services in a list format.
 */
void viewServices();

/**
 * @brief Updates an existing service's details interactively.
 */
void updateService();

/**
 * @brief Deletes a service by ID from the services file.
 */
void deleteService();

/**
 * @brief Loads all service history entries from the history file.
 * @return std::vector<ServiceHistory> A vector containing all valid service history records.
 */
std::vector<ServiceHistory> loadHistory();

/**
 * @brief Saves a list of service history entries to the history file.
 * @param list A vector of ServiceHistory objects to save.
 */
void saveHistory(const std::vector<ServiceHistory>& list);

/**
 * @brief Retrieves the current date and time as a formatted string.
 * @return std::string The current date and time in "YYYY-MM-DD HH:MM:SS" format.
 */
std::string currentDateTime();

/**
 * @brief Adds a new service history entry to the history file.
 * @param h The ServiceHistory object to add.
 */
void addHistoryEntry(const ServiceHistory& h);

/**
 * @brief Displays all service history entries in a formatted table.
 */
void viewServiceHistory();

/**
 * @brief Marks a service history entry as completed by ID.
 */
void markHistoryCompleted();

#endif // SERVICE_H