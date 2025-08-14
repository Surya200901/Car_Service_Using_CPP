// Vehicle.h
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>

/**
 * @brief Represents a vehicle in the car service management system.
 */
struct Vehicle {
    int id;                 /**< Unique identifier for the vehicle. */
    int customerId;         /**< ID of the customer who owns the vehicle. */
    std::string regNo;      /**< Registration number of the vehicle. */
    std::string model;      /**< Model of the vehicle. */
    std::string color;      /**< Color of the vehicle. */
};

/**
 * @brief Loads all vehicles from the vehicle file.
 * @return std::vector<Vehicle> A vector containing all valid vehicle records.
 */
std::vector<Vehicle> loadVehicles();

/**
 * @brief Saves a list of vehicles to the vehicle file.
 * @param list A vector of Vehicle objects to save.
 */
void saveVehicles(const std::vector<Vehicle>& list);

/**
 * @brief Determines the next available vehicle ID.
 * @return int The next available vehicle ID.
 */
int nextVehicleId();

/**
 * @brief Interactively registers a new vehicle to the vehicle file.
 */
void registerVehicleInteractive();

/**
 * @brief Displays all vehicles in a formatted table.
 */
void viewVehicles();

/**
 * @brief Updates an existing vehicle's details interactively.
 */
void updateVehicle();

/**
 * @brief Deletes a vehicle by ID from the vehicle file.
 */
void deleteVehicle();

#endif // VEHICLE_H