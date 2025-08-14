// Discount.cpp (implementation)
#include "Discount.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <map>

#ifdef TEST_MODE
const std::string DISCOUNT_FILE = "tests/test_discounts.txt";
#else
const std::string DISCOUNT_FILE = "discounts.txt";
#endif

/**
 * @brief Determines the next available discount ID by finding the maximum ID in the discount file and incrementing it.
 * @return int The next available discount ID.
 * @note Reads from DISCOUNT_FILE and handles malformed or empty lines gracefully.
 */
int nextDiscountId() {
    std::ifstream ifs(DISCOUNT_FILE);
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
 * @brief Loads all discounts from the discount file into a vector.
 * @return std::vector<Discount> A vector containing all valid discount records.
 * @note Skips empty or malformed lines in the file.
 */
std::vector<Discount> loadDiscounts() {
    std::vector<Discount> list;
    std::ifstream ifs(DISCOUNT_FILE);
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string idStr, name, pctStr, note;
        if (!std::getline(ss, idStr, '|')) continue;
        try {
            Discount d;
            d.id = std::stoi(idStr);
            std::getline(ss, name, '|');
            std::getline(ss, pctStr, '|');
            std::getline(ss, note, '|');
            d.name = name;
            d.percent = std::stod(pctStr);
            d.note = note;
            list.push_back(d);
        } catch (...) {
            continue; // Skip malformed
        }
    }
    return list;
}

/**
 * @brief Saves a list of discounts to the discount file, ensuring unique IDs.
 * @param list A vector of Discount objects to save.
 * @note Overwrites the existing file and keeps only the first occurrence of each discount ID.
 */
void saveDiscounts(const std::vector<Discount>& list) {
    std::map<int, Discount> unique;
    for (const auto& d : list) {
        if (unique.count(d.id) == 0) {
            unique[d.id] = d;
        }
    }
    std::ofstream ofs(DISCOUNT_FILE, std::ios::trunc);
    for (const auto& p : unique) {
        const auto& d = p.second;
        ofs << d.id << '|' << d.name << '|' << d.percent << '|' << d.note << '\n';
    }
}

/**
 * @brief Ensures default discounts are present in the discount file.
 * @note Adds predefined discounts (New Year Offer, Diwali Special, Summer Sale) if the file is empty and saves them.
 */
void ensureDefaultDiscounts() {
    auto list = loadDiscounts();
    if (!list.empty()) return;
    list.push_back({1, "New Year Offer", 10.0, "New Year 10% off"});
    list.push_back({2, "Diwali Special", 15.0, "Festival offer"});
    list.push_back({3, "Summer Sale", 5.0, "Flat 5% summer discount"});
    saveDiscounts(list);
}

/**
 * @brief Displays all discounts in a formatted table.
 * @note Ensures default discounts exist, then shows ID, name, percent, and note with dynamically adjusted column widths.
 *       Prints a message if no discounts are found (though unlikely due to ensureDefaultDiscounts).
 */
void viewDiscounts() {
    ensureDefaultDiscounts();
    auto list = loadDiscounts();
    if (list.empty()) {
        std::cout << "No discounts found.\n";
        return;
    }

    // Calculate max widths
    size_t maxId = 2; // "ID"
    size_t maxName = 4; // "Name"
    size_t maxPercent = 7; // "Percent"
    size_t maxNote = 4; // "Note"
    for (const auto& d : list) {
        maxId = std::max(maxId, std::to_string(d.id).length());
        maxName = std::max(maxName, d.name.length());
        maxPercent = std::max(maxPercent, std::to_string(static_cast<int>(d.percent)).length() + 1); // +1 for '%'
        maxNote = std::max(maxNote, d.note.length());
    }

    // Print header
    std::cout << std::left << std::setw(maxId + 2) << "ID" 
              << std::setw(maxName + 2) << "Name" 
              << std::setw(maxPercent + 2) << "Percent" 
              << std::setw(maxNote + 2) << "Note" << "\n";

    // Print separator
    std::cout << std::string(maxId + maxName + maxPercent + maxNote + 8, '-') << "\n";

    // Print rows
    for (const auto& d : list) {
        std::cout << std::left << std::setw(maxId + 2) << d.id 
                  << std::setw(maxName + 2) << d.name 
                  << std::setw(maxPercent + 2) << (std::to_string(static_cast<int>(d.percent)) + "%") 
                  << std::setw(maxNote + 2) << d.note << "\n";
    }
}

/**
 * @brief Interactively adds a new discount to the discount file.
 * @note Prompts the user for name, percent, and note, assigns a new ID, and saves the updated discount list.
 */
void addDiscountInteractive() {
    auto list = loadDiscounts();
    Discount d;
    d.id = nextDiscountId();
    std::cout << "Enter discount name: "; std::getline(std::cin, d.name);
    std::cout << "Enter percent (e.g., 10 for 10%): "; std::cin >> d.percent; std::cin.ignore();
    std::cout << "Enter note: "; std::getline(std::cin, d.note);
    list.push_back(d);
    saveDiscounts(list);
    std::cout << "Discount added with ID: " << d.id << "\n";
}

/**
 * @brief Updates an existing discount's details interactively.
 * @note Prompts for a discount ID and allows updating name, percent, and note. Empty inputs or zero percent preserve existing values.
 */
void updateDiscount() {
    auto list = loadDiscounts();
    std::cout << "Enter discount ID to update: "; int id; std::cin >> id; std::cin.ignore();
    for (auto &d : list) {
        if (d.id == id) {
            std::cout << "Enter new name (leave blank to keep): ";
            std::string tmp; std::getline(std::cin, tmp); if (!tmp.empty()) d.name = tmp;
            std::cout << "Enter new percent (0 to keep): ";
            double p; std::cin >> p; std::cin.ignore(); if (p>0) d.percent = p;
            std::cout << "Enter new note (leave blank to keep): ";
            std::getline(std::cin, tmp); if (!tmp.empty()) d.note = tmp;
            saveDiscounts(list); std::cout << "Discount updated.\n"; return;
        }
    }
    std::cout << "Discount not found.\n";
}

/**
 * @brief Deletes a discount by ID from the discount file.
 * @note Prompts for a discount ID, removes the discount if found, and saves the updated list.
 */
void deleteDiscount() {
    auto list = loadDiscounts();
    std::cout << "Enter discount ID to delete: "; int id; std::cin >> id; std::cin.ignore();
    auto it = std::remove_if(list.begin(), list.end(), [&](const Discount& d){ return d.id == id; });
    if (it != list.end()) {
        list.erase(it, list.end());
        saveDiscounts(list);
        std::cout << "Discount deleted.\n";
    } else {
        std::cout << "Discount not found.\n";
    }
}