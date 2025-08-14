// Service.cpp (implementation)
#include "Service.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

#ifdef TEST_MODE
const std::string SERVICES_FILE = "tests/test_services.txt";
const std::string HISTORY_FILE = "tests/test_service_history.txt";
#else
const std::string SERVICES_FILE = "services.txt";
const std::string HISTORY_FILE = "service_history.txt";
#endif

/**
 * @brief Determines the next available service ID by finding the maximum ID in the services file and incrementing it.
 * @return int The next available service ID.
 * @note Reads from SERVICES_FILE and handles malformed or empty lines gracefully.
 */
int nextServiceId() {
    std::ifstream ifs(SERVICES_FILE);
    int maxId = 0; std::string ln;
    while (std::getline(ifs, ln)) {
        if (ln.empty()) continue;
        std::istringstream ss(ln);
        std::string idStr; std::getline(ss, idStr, '|');
        try {
            int id = std::stoi(idStr);
            if (id > maxId) maxId = id;
        } catch (...) {}
    }
    return maxId + 1;
}

/**
 * @brief Determines the next available service history ID by finding the maximum ID in the history file and incrementing it.
 * @return int The next available service history ID.
 * @note Reads from HISTORY_FILE and handles malformed or empty lines gracefully.
 */
int nextHistoryId() {
    std::ifstream ifs(HISTORY_FILE);
    int maxId = 0; std::string ln;
    while (std::getline(ifs, ln)) {
        if (ln.empty()) continue;
        std::istringstream ss(ln);
        std::string idStr; std::getline(ss, idStr, '|');
        try {
            int id = std::stoi(idStr);
            if (id > maxId) maxId = id;
        } catch (...) {}
    }
    return maxId + 1;
}

/**
 * @brief Loads all services from the services file into a vector.
 * @return std::vector<ServiceItem> A vector containing all valid service records.
 * @note Skips empty or malformed lines in the file.
 */
std::vector<ServiceItem> loadServices() {
    std::vector<ServiceItem> list;
    std::ifstream ifs(SERVICES_FILE);
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string idStr, name, priceStr;
        if (!std::getline(ss, idStr, '|')) continue;
        try {
            ServiceItem s;
            s.id = std::stoi(idStr);
            std::getline(ss, name, '|');
            std::getline(ss, priceStr, '|');
            s.name = name;
            s.price = std::stod(priceStr);
            list.push_back(s);
        } catch (...) {
            continue; // Skip malformed
        }
    }
    return list;
}

/**
 * @brief Saves a list of services to the services file, ensuring unique IDs.
 * @param list A vector of ServiceItem objects to save.
 * @note Overwrites the existing file and keeps only the first occurrence of each service ID.
 */
void saveServices(const std::vector<ServiceItem>& list) {
    std::map<int, ServiceItem> unique;
    for (const auto& s : list) {
        if (unique.count(s.id) == 0) {
            unique[s.id] = s;
        }
    }
    std::ofstream ofs(SERVICES_FILE, std::ios::trunc);
    for (const auto& p : unique) {
        const auto& s = p.second;
        ofs << s.id << '|' << s.name << '|' << s.price << '\n';
    }
}

/**
 * @brief Ensures default services are present in the services file.
 * @note Adds predefined services (e.g., Oil Change, General Service) if the file is empty and saves them.
 */
void ensureDefaultServices() {
    auto list = loadServices();
    if (!list.empty()) return;
    // Add defaults including General Service
    list.push_back({1, "Oil Change", 1200});
    list.push_back({2, "Brake Inspection", 800});
    list.push_back({3, "Wheel Alignment", 600});
    list.push_back({4, "Car Wash", 500});
    list.push_back({5, "Engine Tune-up", 2000});
    list.push_back({6, "General Service", 1500});
    saveServices(list);
}

/**
 * @brief Interactively adds a new service to the services file.
 * @note Prompts for service name and price, validates price input, assigns a new ID, and saves the updated list.
 */
void addServiceInteractive() {
    auto list = loadServices();
    ServiceItem s;
    s.id = nextServiceId();
    std::cout << "Enter service name: ";
    std::getline(std::cin, s.name);
    
    while (true) {
        std::cout << "Enter price: ";
        std::string input;
        std::getline(std::cin, input); // Read input as string
        std::istringstream iss(input);
        double price;
        if (iss >> price && iss.eof() && price >= 0) { // Ensure valid number and no trailing characters
            s.price = price;
            break; // Valid input, exit loop
        } else {
            std::cout << "Invalid price. Please enter a valid number (>= 0).\n";
            std::cin.clear(); // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }
    
    list.push_back(s);
    saveServices(list);
    std::cout << "Service added with ID: " << s.id << "\n";
}

/**
 * @brief Displays all services in a simple list format.
 * @note Ensures default services exist, then shows ID, name, and price for each service.
 */
void viewServices() {
    ensureDefaultServices();
    auto list = loadServices();
    std::cout << "--- Available Services ---\n";
    for (auto &s : list) {
        std::cout << s.id << ". " << s.name << " - Rs." << (int)s.price << '\n';
    }
}

/**
 * @brief Updates an existing service's details interactively.
 * @note Prompts for a service ID and allows updating name and price. Empty name or zero price inputs preserve existing values.
 */
void updateService() {
    auto list = loadServices();
    std::cout << "Enter service ID to update: ";
    int id;
    while (!(std::cin >> id)) {
        std::cout << "Invalid ID. Please enter a valid integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(); // Clear newline after valid ID input

    for (auto &s : list) {
        if (s.id == id) {
            std::cout << "Enter new name (leave blank to keep): ";
            std::string tmp;
            std::getline(std::cin, tmp);
            if (!tmp.empty()) s.name = tmp;

            std::cout << "Enter new price (0 to keep): ";
            while (true) {
                std::string input;
                std::getline(std::cin, input); // Read input as string
                std::istringstream iss(input);
                double p;
                if (input.empty() || (iss >> p && iss.eof() && p >= 0)) { // Allow empty input or valid number
                    if (!input.empty()) s.price = p; // Update price only if input is not empty and valid
                    saveServices(list);
                    std::cout << "Service updated.\n";
                    return;
                } else {
                    std::cout << "Invalid price. Please enter a valid number (>= 0) or 0 to keep: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }
    }
    std::cout << "Service not found.\n";
}

/**
 * @brief Deletes a service by ID from the services file.
 * @note Prompts for a service ID, removes the service if found, and saves the updated list.
 */
void deleteService() {
    auto list = loadServices();
    std::cout << "Enter service ID to delete: ";
    int id; std::cin >> id; std::cin.ignore();
    auto it = std::remove_if(list.begin(), list.end(), [&](const ServiceItem& s){ return s.id == id; });
    if (it != list.end()) {
        list.erase(it, list.end());
        saveServices(list);
        std::cout << "Service deleted.\n";
    } else {
        std::cout << "Service not found.\n";
    }
}

/**
 * @brief Loads all service history entries from the history file into a vector.
 * @return std::vector<ServiceHistory> A vector containing all valid service history records.
 * @note Skips empty or malformed lines and parses comma-separated service IDs.
 */
std::vector<ServiceHistory> loadHistory() {
    std::vector<ServiceHistory> list;
    std::ifstream ifs(HISTORY_FILE);
    std::string ln;
    while (std::getline(ifs, ln)) {
        if (ln.empty()) continue;
        std::istringstream ss(ln);
        std::string sid, sids, subtotalStr, discIdStr, discPctStr, totalStr;
        try {
            std::getline(ss, sid, '|'); int historyId = std::stoi(sid);
            std::getline(ss, sid, '|'); int customerId = std::stoi(sid);
            std::getline(ss, sid, '|'); int vehicleId = std::stoi(sid);
            std::getline(ss, sids, '|');
            std::vector<int> serviceIds;
            std::istringstream sss(sids);
            std::string token;
            while (std::getline(sss, token, ',')) {
                if (!token.empty()) serviceIds.push_back(std::stoi(token));
            }
            std::string dateTime;
            std::getline(ss, dateTime, '|');
            std::getline(ss, subtotalStr, '|'); double subtotal = std::stod(subtotalStr);
            std::getline(ss, discIdStr, '|'); int discountId = std::stoi(discIdStr);
            std::getline(ss, discPctStr, '|'); double discountPercent = std::stod(discPctStr);
            std::getline(ss, totalStr, '|'); double total = std::stod(totalStr);
            std::string status;
            std::getline(ss, status, '|');
            list.push_back({historyId, customerId, vehicleId, serviceIds, dateTime, subtotal, discountId, discountPercent, total, status});
        } catch (...) {
            continue; // Skip malformed
        }
    }
    return list;
}

/**
 * @brief Saves a list of service history entries to the history file.
 * @param list A vector of ServiceHistory objects to save.
 * @note Overwrites the existing file, storing service IDs as a comma-separated list.
 */
void saveHistory(const std::vector<ServiceHistory>& list) {
    std::ofstream ofs(HISTORY_FILE, std::ios::trunc);
    for (auto &h : list) {
        ofs << h.historyId << '|' << h.customerId << '|' << h.vehicleId << '|';
        for (size_t i=0;i<h.serviceIds.size();++i) {
            if (i) ofs << ',';
            ofs << h.serviceIds[i];
        }
        ofs << '|' << h.dateTime << '|' << h.subtotal << '|' << h.discountId << '|' << h.discountPercent << '|' << h.total << '|' << h.status << '\n';
    }
}

/**
 * @brief Retrieves the current date and time as a formatted string.
 * @return std::string The current date and time in the format "YYYY-MM-DD HH:MM:SS".
 */
std::string currentDateTime() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return buf;
}

/**
 * @brief Adds a new service history entry to the history file.
 * @param h The ServiceHistory object to add.
 * @note Loads the existing history, appends the new entry, and saves the updated list.
 */
void addHistoryEntry(const ServiceHistory& h) {
    auto list = loadHistory();
    list.push_back(h);
    saveHistory(list);
}

/**
 * @brief Displays all service history entries in a formatted table.
 * @note Shows history ID, customer ID, vehicle ID, services, date, costs, and status with dynamically adjusted column widths.
 */
void viewServiceHistory() {
    auto list = loadHistory();
    if (list.empty()) {
        std::cout << "No service history found.\n";
        return;
    }

    // Calculate max widths
    size_t maxHistoryId = 9; // "HistoryID"
    size_t maxCustId = 6; // "CustID"
    size_t maxVehId = 5; // "VehID"
    size_t maxServices = 8; // "Services"
    size_t maxDateTime = 8; // "DateTime"
    size_t maxSubtotal = 8; // "Subtotal"
    size_t maxDiscount = 9; // "Discount%"
    size_t maxTotal = 5; // "Total"
    size_t maxStatus = 6; // "Status"
    for (const auto& h : list) {
        maxHistoryId = std::max(maxHistoryId, std::to_string(h.historyId).length());
        maxCustId = std::max(maxCustId, std::to_string(h.customerId).length());
        maxVehId = std::max(maxVehId, std::to_string(h.vehicleId).length());
        std::string servicesStr;
        for (size_t i = 0; i < h.serviceIds.size(); ++i) {
            if (i > 0) servicesStr += ",";
            servicesStr += std::to_string(h.serviceIds[i]);
        }
        maxServices = std::max(maxServices, servicesStr.length());
        maxDateTime = std::max(maxDateTime, h.dateTime.length());
        maxSubtotal = std::max(maxSubtotal, std::to_string(static_cast<int>(h.subtotal)).length());
        maxDiscount = std::max(maxDiscount, std::to_string(static_cast<int>(h.discountPercent)).length());
        maxTotal = std::max(maxTotal, std::to_string(static_cast<int>(h.total)).length());
        maxStatus = std::max(maxStatus, h.status.length());
    }

    // Print header
    std::cout << std::left << std::setw(maxHistoryId + 2) << "HistoryID" 
              << std::setw(maxCustId + 2) << "CustID" 
              << std::setw(maxVehId + 2) << "VehID" 
              << std::setw(maxServices + 2) << "Services" 
              << std::setw(maxDateTime + 2) << "DateTime" 
              << std::setw(maxSubtotal + 2) << "Subtotal" 
              << std::setw(maxDiscount + 2) << "Discount%" 
              << std::setw(maxTotal + 2) << "Total" 
              << std::setw(maxStatus + 2) << "Status" << "\n";

    // Print separator
    size_t totalWidth = maxHistoryId + maxCustId + maxVehId + maxServices + maxDateTime + maxSubtotal + maxDiscount + maxTotal + maxStatus + 18;
    std::cout << std::string(totalWidth, '-') << "\n";

    // Print rows
    for (const auto& h : list) {
        std::string servicesStr;
        for (size_t i = 0; i < h.serviceIds.size(); ++i) {
            if (i > 0) servicesStr += ",";
            servicesStr += std::to_string(h.serviceIds[i]);
        }
        std::cout << std::left << std::setw(maxHistoryId + 2) << h.historyId 
                  << std::setw(maxCustId + 2) << h.customerId 
                  << std::setw(maxVehId + 2) << h.vehicleId 
                  << std::setw(maxServices + 2) << servicesStr 
                  << std::setw(maxDateTime + 2) << h.dateTime 
                  << std::setw(maxSubtotal + 2) << h.subtotal 
                  << std::setw(maxDiscount + 2) << h.discountPercent 
                  << std::setw(maxTotal + 2) << h.total 
                  << std::setw(maxStatus + 2) << h.status << "\n";
    }
}

/**
 * @brief Marks a service history entry as completed by ID.
 * @note Prompts for a history ID, updates the status to "Completed" if found, and saves the updated list.
 */
void markHistoryCompleted() {
    auto list = loadHistory();
    std::cout << "Enter history ID to mark completed: ";
    int id; std::cin >> id; std::cin.ignore();
    for (auto &h : list) {
        if (h.historyId == id) {
            h.status = "Completed";
            saveHistory(list);
            std::cout << "Marked completed.\n";
            return;
        }
    }
    std::cout << "History entry not found.\n";
}