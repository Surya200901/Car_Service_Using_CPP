// Vehicle.cpp (implementation)
#include "Vehicle.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <map>

#ifdef TEST_MODE
const std::string VEHICLE_FILE = "tests/test_vehicles.txt";
#else
const std::string VEHICLE_FILE = "vehicles.txt";
#endif

/**
 * @brief Determines the next available vehicle ID by finding the maximum ID in the vehicle file and incrementing it.
 * @return int The next available vehicle ID.
 * @note Reads from VEHICLE_FILE and handles malformed or empty lines gracefully.
 */
int nextVehicleId() {
    std::ifstream ifs(VEHICLE_FILE);
    int maxId = 0;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string idStr;
        std::getline(ss, idStr, '|');
        try {
            int id = std::stoi(idStr);
            if (id > maxId) maxId = id;
        } catch (...) {}
    }
    return maxId + 1;
}

/**
 * @brief Loads all vehicles from the vehicle file into a vector.
 * @return std::vector<Vehicle> A vector containing all valid vehicle records.
 * @note Skips empty or malformed lines in the file.
 */
std::vector<Vehicle> loadVehicles() {
    std::vector<Vehicle> list;
    std::ifstream ifs(VEHICLE_FILE);
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string idStr, cid, regNo, model, color;
        if (!std::getline(ss, idStr, '|')) continue;
        try {
            Vehicle v;
            v.id = std::stoi(idStr);
            std::getline(ss, cid, '|'); v.customerId = std::stoi(cid);
            std::getline(ss, regNo, '|');
            std::getline(ss, model, '|');
            std::getline(ss, color, '|');
            v.regNo = regNo;
            v.model = model;
            v.color = color;
            list.push_back(v);
        } catch (...) {
            continue; // Skip malformed
        }
    }
    return list;
}

/**
 * @brief Saves a list of vehicles to the vehicle file, ensuring unique IDs.
 * @param list A vector of Vehicle objects to save.
 * @note Overwrites the existing file and keeps only the first occurrence of each vehicle ID.
 */
void saveVehicles(const std::vector<Vehicle>& list) {
    std::map<int, Vehicle> unique;
    for (const auto& v : list) {
        if (unique.count(v.id) == 0) {
            unique[v.id] = v;
        }
    }
    std::ofstream ofs(VEHICLE_FILE, std::ios::trunc);
    for (const auto& p : unique) {
        const auto& v = p.second;
        ofs << v.id << '|' << v.customerId << '|' << v.regNo << '|' << v.model << '|' << v.color << '\n';
    }
}

/**
 * @brief Interactively registers a new vehicle to the vehicle file.
 * @note Prompts for customer ID, registration number, model, and color, assigns a new ID, and saves the updated list.
 */
void registerVehicleInteractive() {
    std::vector<Vehicle> list = loadVehicles();
    Vehicle v;
    v.id = nextVehicleId();
    std::cout << "Enter customer ID: "; std::cin >> v.customerId; std::cin.ignore();
    std::cout << "Enter registration number: "; std::getline(std::cin, v.regNo);
    std::cout << "Enter model: "; std::getline(std::cin, v.model);
    std::cout << "Enter color: "; std::getline(std::cin, v.color);
    list.push_back(v);
    saveVehicles(list);
    std::cout << "Vehicle registered with ID: " << v.id << "\n";
}

/**
 * @brief Displays all vehicles in a formatted table.
 * @note Shows ID, customer ID, registration number, model, and color with dynamically adjusted column widths.
 *       Prints a message if no vehicles are found.
 */
void viewVehicles() {
    auto list = loadVehicles();
    if (list.empty()) {
        std::cout << "No vehicles found.\n";
        return;
    }

    // Calculate max widths
    size_t maxId = 2; // "ID"
    size_t maxCustId = 6; // "CustID"
    size_t maxRegNo = 5; // "RegNo"
    size_t maxModel = 5; // "Model"
    size_t maxColor = 5; // "Color"
    for (const auto& v : list) {
        maxId = std::max(maxId, std::to_string(v.id).length());
        maxCustId = std::max(maxCustId, std::to_string(v.customerId).length());
        maxRegNo = std::max(maxRegNo, v.regNo.length());
        maxModel = std::max(maxModel, v.model.length());
        maxColor = std::max(maxColor, v.color.length());
    }

    // Print header
    std::cout << std::left << std::setw(maxId + 2) << "ID" 
              << std::setw(maxCustId + 2) << "CustID" 
              << std::setw(maxRegNo + 2) << "RegNo" 
              << std::setw(maxModel + 2) << "Model" 
              << std::setw(maxColor + 2) << "Color" << "\n";

    // Print separator
    std::cout << std::string(maxId + maxCustId + maxRegNo + maxModel + maxColor + 10, '-') << "\n";

    // Print rows
    for (const auto& v : list) {
        std::cout << std::left << std::setw(maxId + 2) << v.id 
                  << std::setw(maxCustId + 2) << v.customerId 
                  << std::setw(maxRegNo + 2) << v.regNo 
                  << std::setw(maxModel + 2) << v.model 
                  << std::setw(maxColor + 2) << v.color << "\n";
    }
}

/**
 * @brief Updates an existing vehicle's details interactively.
 * @note Prompts for a vehicle ID and allows updating registration number, model, and color.
 *       Empty inputs preserve existing values.
 */
void updateVehicle() {
    auto list = loadVehicles();
    std::cout << "Enter vehicle ID to update: "; int id; std::cin >> id; std::cin.ignore();
    for (auto &v : list) {
        if (v.id == id) {
            std::cout << "Enter new reg no (leave blank to keep): ";
            std::string tmp; std::getline(std::cin, tmp); if (!tmp.empty()) v.regNo = tmp;
            std::cout << "Enter new model (leave blank to keep): ";
            std::getline(std::cin, tmp); if (!tmp.empty()) v.model = tmp;
            std::cout << "Enter new color (leave blank to keep): ";
            std::getline(std::cin, tmp); if (!tmp.empty()) v.color = tmp;
            saveVehicles(list);
            std::cout << "Vehicle updated.\n";
            return;
        }
    }
    std::cout << "Vehicle not found.\n";
}

/**
 * @brief Deletes a vehicle by ID from the vehicle file.
 * @note Prompts for a vehicle ID, removes the vehicle if found, and saves the updated list.
 */
void deleteVehicle() {
    auto list = loadVehicles();
    std::cout << "Enter vehicle ID to delete: ";
    int id; std::cin >> id; std::cin.ignore();
    auto it = std::remove_if(list.begin(), list.end(), [&](const Vehicle& v){ return v.id == id; });
    if (it != list.end()) {
        list.erase(it, list.end());
        saveVehicles(list);
        std::cout << "Vehicle deleted.\n";
    } else {
        std::cout << "Vehicle not found.\n";
    }
}