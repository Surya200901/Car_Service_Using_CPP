// main.cpp - The main application
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "Customer.h"
#include "Service.h"
#include "Vehicle.h"
#include "Discount.h"
#include <algorithm>

std::vector<ServiceItem> loadServicesLocal() {
    return loadServices();
}

std::vector<Discount> loadDiscountsLocal() {
    return loadDiscounts();
}

ServiceItem* findServiceById(std::vector<ServiceItem>& list, int id) {
    for (auto &s : list) if (s.id == id) return &s;
    return nullptr;
}

Discount* findDiscountById(std::vector<Discount>& list, int id) {
    for (auto &d : list) if (d.id == id) return &d;
    return nullptr;
}

int mainMenu() {
    std::cout << "\n--- Car Service Management ---\n";
    std::cout << "1. Add Customer\n";
    std::cout << "2. View Customers\n";
    std::cout << "3. Search Customer\n";
    std::cout << "4. Register Vehicle\n";
    std::cout << "5. View Vehicles\n";
    std::cout << "6. Book Service\n";
    std::cout << "7. View Service History\n";
    std::cout << "8. Generate Bill (for existing booking)\n";
    std::cout << "9. Discounts (manage)\n";
    std::cout << "10. Update Customer\n";
    std::cout << "11. Delete Customer\n";
    std::cout << "12. Update Vehicle\n";
    std::cout << "13. Delete Vehicle\n";
    std::cout << "14. Services (manage)\n";
    std::cout << "0. Exit (mark customer service completed)\n";
    std::cout << "Enter option: ";
    int opt; std::cin >> opt; std::cin.ignore();
    return opt;
}

void bookServiceFlow() {
    ensureDefaultServices();
    ensureDefaultDiscounts();
    auto services = loadServicesLocal();
    auto discounts = loadDiscountsLocal();
    auto customers = loadCustomers();
    auto vehicles = loadVehicles();

    std::cout << "Enter Customer ID: "; int custId; std::cin >> custId; std::cin.ignore();
    if (findCustomerById(customers, custId) == nullptr) {
        std::cout << "Customer not found.\n"; return;
    }
    std::cout << "Enter Vehicle ID: "; int vehId; std::cin >> vehId; std::cin.ignore();
    bool vehExists = false;
    for (const auto& v : vehicles) {
        if (v.id == vehId && v.customerId == custId) {
            vehExists = true;
            break;
        }
    }
    if (!vehExists) {
        std::cout << "Vehicle not found or not owned by customer.\n"; return;
    }

    std::vector<int> chosen;
    while (true) {
        viewServices();
        std::cout << "Select service number (0 to finish): ";
        int sid; std::cin >> sid; std::cin.ignore();
        if (sid == 0) break;
        if (findServiceById(services, sid) == nullptr) {
            std::cout << "Invalid service id.\n";
            continue;
        }
        chosen.push_back(sid);
    }

    if (chosen.empty()) {
        std::cout << "No services selected. Aborting.\n";
        return;
    }

    double subtotal = 0;
    for (int id : chosen) {
        ServiceItem* s = findServiceById(services, id);
        if (s) subtotal += s->price;
    }

    std::cout << "Subtotal: Rs." << subtotal << "\n";

    std::cout << "Available discounts:\n";
    viewDiscounts();
    std::cout << "Select discount id to apply (0 for none): ";
    int did; std::cin >> did; std::cin.ignore();

    double discPct = 0.0;
    if (did != 0) {
        Discount* d = findDiscountById(discounts, did);
        if (d) discPct = d->percent;
        else {
            std::cout << "Invalid discount id. No discount applied.\n";
            did = -1;
        }
    } else did = -1;

    double discountAmount = subtotal * (discPct / 100.0);
    double total = subtotal - discountAmount;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Discount: " << discPct << "% -> -Rs." << discountAmount << "\n";
    std::cout << "Total: Rs." << total << "\n";

    // Create history entry
    ServiceHistory h;
    h.historyId = nextHistoryId();
    h.customerId = custId; h.vehicleId = vehId; h.serviceIds = chosen;
    h.dateTime = currentDateTime();
    h.subtotal = subtotal;
    h.discountId = did;
    h.discountPercent = discPct;
    h.total = total;
    h.status = "Pending";
    addHistoryEntry(h);

    std::cout << "Booking saved with History ID: " << h.historyId << "\n";
}

void generateBillForHistory() {
    auto histories = loadHistory();

    if (histories.empty()) {
        std::cout << "No history entries.\n"; return;
    }
    std::cout << "Enter History ID to generate bill: ";
    int hid; std::cin >> hid; std::cin.ignore();
    for (auto &h : histories) {
        if (h.historyId == hid) {
            // print bill
            std::cout << "\n--- BILL ---\n";
            std::cout << "History ID: " << h.historyId << "\n";
            std::cout << "Customer ID: " << h.customerId << "\n";
            std::cout << "Vehicle ID: " << h.vehicleId << "\n";
            std::cout << "Date: " << h.dateTime << "\n";
            auto services = loadServicesLocal();
            std::cout << "Services:\n";
            for (int sid : h.serviceIds) {
                ServiceItem* s = findServiceById(services, sid);
                if (s) std::cout << " - " << s->name << " : Rs." << s->price << "\n";
            }
            std::cout << "Subtotal: Rs." << h.subtotal << "\n";
            std::cout << "Discount: " << h.discountPercent << "%\n";
            std::cout << "Total: Rs." << h.total << "\n";
            std::cout << "Status: " << h.status << "\n";
            return;
        }
    }
    std::cout << "History ID not found.\n";
}

// New function to delete all vehicles for a customer
void deleteVehiclesForCustomer(int customerId) {
    auto list = loadVehicles();
    auto it = std::remove_if(list.begin(), list.end(), [customerId](const Vehicle& v){ return v.customerId == customerId; });
    if (it != list.end()) {
        list.erase(it, list.end());
        saveVehicles(list);
        std::cout << "All vehicles for customer " << customerId << " deleted.\n";
    } else {
        std::cout << "No vehicles found for customer " << customerId << ".\n";
    }
}

int main() {
    // Ensure default data
    ensureDefaultServices();
    ensureDefaultDiscounts();

    while (true) {
        int opt = mainMenu();
        switch (opt) {
            case 1: addCustomerInteractive(); break;
            case 2: viewCustomers(); break;
            case 3: searchCustomer(); break;
            case 4: registerVehicleInteractive(); break;
            case 5: viewVehicles(); break;
            case 6: bookServiceFlow(); break;
            case 7: viewServiceHistory(); break;
            case 8: generateBillForHistory(); break;
            case 9: {
                std::cout << "\n--- Discounts Menu ---\n";
                std::cout << "1. View Discounts\n2. Add Discount\n3. Update Discount\n4. Delete Discount\n0. Back\nEnter: ";
                int dopt; std::cin >> dopt; std::cin.ignore();
                if (dopt==1) viewDiscounts();
                else if (dopt==2) addDiscountInteractive();
                else if (dopt==3) updateDiscount();
                else if (dopt==4) deleteDiscount();
                break;
            }
            case 10: updateCustomer(); break;
            case 11: deleteCustomer(); break;
            case 12: updateVehicle(); break;
            case 13: deleteVehicle(); break;
            case 14: {
                std::cout << "\n--- Services Menu ---\n";
                std::cout << "1. View Services\n2. Add Service\n3. Update Service\n4. Delete Service\n0. Back\nEnter: ";
                int sopt; std::cin >> sopt; std::cin.ignore();
                if (sopt==1) viewServices();
                else if (sopt==2) addServiceInteractive();
                else if (sopt==3) updateService();
                else if (sopt==4) deleteService();
                break;
            }
            case 0: {
                std::cout << "Before exit, enter customer ID to mark their service(s) as completed (or 0 to skip): ";
                int cid; std::cin >> cid; std::cin.ignore();
                if (cid > 0) {
                    auto list = loadHistory();
                    bool found = false;
                    for (auto &h: list) {
                        if (h.customerId == cid && h.status == "Pending") {
                            h.status = "Completed";
                            found = true;
                        }
                    }
                    if (found) {
                        saveHistory(list);
                        std::cout << "Marked all pending services for customer " << cid << " as Completed.\n";
                        // Delete all vehicles for this customer
                        deleteVehiclesForCustomer(cid);
                        // Delete the customer
                        auto customers = loadCustomers();
                        auto it = std::remove_if(customers.begin(), customers.end(), [cid](const Customer& c){ return c.id == cid; });
                        if (it != customers.end()) {
                            customers.erase(it, customers.end());
                            saveCustomers(customers);
                            std::cout << "Customer " << cid << " deleted.\n";
                        } else {
                            std::cout << "Customer " << cid << " not found.\n";
                        }
                    } else {
                        std::cout << "No pending services found for customer " << cid << ".\n";
                    }
                }
                std::cout << "Exiting...\n";
                return 0;
            }
            default: std::cout << "Invalid option.\n";
        }
    }

    return 0;
}