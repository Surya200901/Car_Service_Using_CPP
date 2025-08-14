// Discount.h
#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>
#include <vector>

/**
 * @brief Represents a discount in the car service management system.
 */
struct Discount {
    int id;             /**< Unique identifier for the discount. */
    std::string name;   /**< Name of the discount. */
    double percent;     /**< Discount percentage (e.g., 10 for 10%). */
    std::string note;   /**< Additional notes or description for the discount. */
};

/**
 * @brief Loads all discounts from the discount file.
 * @return std::vector<Discount> A vector containing all valid discount records.
 */
std::vector<Discount> loadDiscounts();

/**
 * @brief Saves a list of discounts to the discount file.
 * @param list A vector of Discount objects to save.
 */
void saveDiscounts(const std::vector<Discount>& list);

/**
 * @brief Determines the next available discount ID.
 * @return int The next available discount ID.
 */
int nextDiscountId();

/**
 * @brief Ensures default discounts are present in the discount file.
 */
void ensureDefaultDiscounts();

/**
 * @brief Displays all discounts in a formatted table.
 */
void viewDiscounts();

/**
 * @brief Interactively adds a new discount to the discount file.
 */
void addDiscountInteractive();

/**
 * @brief Updates an existing discount's details interactively.
 */
void updateDiscount();

/**
 * @brief Deletes a discount by ID from the discount file.
 */
void deleteDiscount();

#endif // DISCOUNT_H