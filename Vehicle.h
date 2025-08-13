// Vehicle.h
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>

struct Vehicle {
    int id;
    int customerId;
    std::string regNo;
    std::string model;
    std::string color;
};

std::vector<Vehicle> loadVehicles();
void saveVehicles(const std::vector<Vehicle>& list);
int nextVehicleId();
void registerVehicleInteractive();
void viewVehicles();
void updateVehicle();
void deleteVehicle();

#endif // VEHICLE_H