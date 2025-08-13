// test.cpp - The unit test suite
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Customer.h"
#include "Vehicle.h"
#include "Service.h"
#include "Discount.h"

// Define file paths for testing
#define CUSTOMER_FILE "tests/test_customers.txt"
#define VEHICLE_FILE "tests/test_vehicles.txt"
#define SERVICES_FILE "tests/test_services.txt"
#define DISCOUNT_FILE "tests/test_discounts.txt"
#define HISTORY_FILE "tests/test_service_history.txt"

// Global silent mode flag
bool silentMode = false;

// Helper to clear test files
void clearTestFiles() {
    std::ofstream ofs;
    ofs.open(CUSTOMER_FILE, std::ios::trunc); ofs.close();
    ofs.open(VEHICLE_FILE, std::ios::trunc); ofs.close();
    ofs.open(SERVICES_FILE, std::ios::trunc); ofs.close();
    ofs.open(DISCOUNT_FILE, std::ios::trunc); ofs.close();
    ofs.open(HISTORY_FILE, std::ios::trunc); ofs.close();
}

// Helper to set silent mode
void setSilentMode(bool silent) {
    silentMode = silent;
}

// =============================
// 📌 Customer Test Functions
// =============================
void test_nextCustomerId() {
    clearTestFiles();
    if (nextCustomerId() != 1) throw std::runtime_error("Initial customer ID should be 1");

    std::ofstream ofs(CUSTOMER_FILE);
    ofs << "1|John|1234567890|john@example.com\n";
    ofs << "2|Jane|0987654321|jane@example.com\n";
    ofs.flush();
    ofs.close();
    
    if (nextCustomerId() != 3) throw std::runtime_error("Next customer ID should be 3 after two customers");
    if (!silentMode) std::cout << "[PASS] test_nextCustomerId\n";
}

void test_loadCustomers_emptyFile() {
    clearTestFiles();
    auto customers = loadCustomers();
    if (!customers.empty()) throw std::runtime_error("Empty file should return empty customer vector");
    if (!silentMode) std::cout << "[PASS] test_loadCustomers_emptyFile\n";
}

void test_loadCustomers_validData() {
    clearTestFiles();
    std::ofstream ofs(CUSTOMER_FILE);
    ofs << "1|John|1234567890|john@example.com\n";
    ofs << "2|Jane|0987654321|jane@example.com\n";
    ofs.close();
    
    auto customers = loadCustomers();
    if (customers.size() != 2) throw std::runtime_error("Should load two customers");
    if (customers[0].id != 1) throw std::runtime_error("Customer 1 ID should be 1");
    if (customers[0].name != "John") throw std::runtime_error("Customer 1 name should be John");
    if (customers[0].phone != "1234567890") throw std::runtime_error("Customer 1 phone should match");
    if (customers[0].email != "john@example.com") throw std::runtime_error("Customer 1 email should match");
    if (customers[1].id != 2) throw std::runtime_error("Customer 2 ID should be 2");
    if (customers[1].name != "Jane") throw std::runtime_error("Customer 2 name should be Jane");
    if (!silentMode) std::cout << "[PASS] test_loadCustomers_validData\n";
}

void test_loadCustomers_invalidData() {
    clearTestFiles();
    std::ofstream ofs(CUSTOMER_FILE);
    ofs << "1|John|1234567890|john@example.com\n";
    ofs << "2||0987654321|jane@example.com\n"; // Empty name
    ofs << "invalid|data\n"; // Malformed line
    ofs.close();
    
    auto customers = loadCustomers();
    if (customers.size() != 2) throw std::runtime_error("Should load valid and empty-name customers, skip malformed");
    if (customers[0].id != 1) throw std::runtime_error("Valid customer should be loaded");
    if (customers[1].id != 2) throw std::runtime_error("Empty name customer should be loaded");
    if (!customers[1].name.empty()) throw std::runtime_error("Empty name should be preserved");
    if (!silentMode) std::cout << "[PASS] test_loadCustomers_invalidData\n";
}

void test_saveCustomers() {
    clearTestFiles();
    std::vector<Customer> customers = {
        {1, "John", "1234567890", "john@example.com"},
        {2, "Jane", "0987654321", "jane@example.com"}
    };
    saveCustomers(customers);
    
    std::ifstream ifs(CUSTOMER_FILE);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    ifs.close();
    
    if (lines.size() != 2) throw std::runtime_error("Should save two customers");
    if (lines[0] != "1|John|1234567890|john@example.com") throw std::runtime_error("Customer 1 data should match");
    if (lines[1] != "2|Jane|0987654321|jane@example.com") throw std::runtime_error("Customer 2 data should match");
    if (!silentMode) std::cout << "[PASS] test_saveCustomers\n";
}

void test_findCustomerById() {
    clearTestFiles();
    std::vector<Customer> customers = {
        {1, "John", "1234567890", "john@example.com"},
        {2, "Jane", "0987654321", "jane@example.com"}
    };
    saveCustomers(customers);
    
    auto customer = findCustomerById(customers, 1);
    if (customer == nullptr) throw std::runtime_error("Customer with ID 1 should be found");
    if (customer->id != 1) throw std::runtime_error("Found customer ID should be 1");
    if (customer->name != "John") throw std::runtime_error("Found customer name should be John");
    
    customer = findCustomerById(customers, 3);
    if (customer != nullptr) throw std::runtime_error("Non-existent customer ID should return nullptr");
    if (!silentMode) std::cout << "[PASS] test_findCustomerById\n";
}

void test_deleteCustomer() {
    clearTestFiles();
    std::vector<Customer> customers = {
        {1, "John", "1234567890", "john@example.com"},
        {2, "Jane", "0987654321", "jane@example.com"}
    };
    saveCustomers(customers);
    
    customers.erase(std::remove_if(customers.begin(), customers.end(), 
        [](const Customer& c) { return c.id == 1; }), customers.end());
    saveCustomers(customers);
    
    customers = loadCustomers();
    if (customers.size() != 1) throw std::runtime_error("Should have one customer after deletion");
    if (customers[0].id != 2) throw std::runtime_error("Remaining customer ID should be 2");
    if (customers[0].name != "Jane") throw std::runtime_error("Remaining customer name should be Jane");
    if (!silentMode) std::cout << "[PASS] test_deleteCustomer\n";
}

void test_addCustomer_duplicateId() {
    clearTestFiles();
    std::vector<Customer> customers = {
        {1, "John", "1234567890", "john@example.com"}
    };
    saveCustomers(customers);
    
    customers.push_back({1, "Jane", "0987654321", "jane@example.com"});
    saveCustomers(customers);
    
    customers = loadCustomers();
    if (customers.size() != 1) throw std::runtime_error("Duplicate ID should not be added");
    if (customers[0].name != "John") throw std::runtime_error("Original customer should remain");
    if (!silentMode) std::cout << "[PASS] test_addCustomer_duplicateId\n";
}

void test_addCustomer_emptyName() {
    clearTestFiles();
    std::vector<Customer> customers = {
        {1, "", "1234567890", "john@example.com"}
    };
    saveCustomers(customers);
    
    customers = loadCustomers();
    if (customers.size() != 1) throw std::runtime_error("Customer with empty name should be added");
    if (customers[0].id != 1) throw std::runtime_error("Customer ID should be 1");
    if (!customers[0].name.empty()) throw std::runtime_error("Customer name should be empty");
    if (!silentMode) std::cout << "[PASS] test_addCustomer_emptyName\n";
}

// =============================
// 📌 Vehicle Test Functions
// =============================
void test_nextVehicleId() {
    clearTestFiles();
    if (nextVehicleId() != 1) throw std::runtime_error("Initial vehicle ID should be 1");
    
    std::ofstream ofs(VEHICLE_FILE);
    ofs << "1|1|ABC123|Honda|Red\n";
    ofs << "2|2|XYZ789|Toyota|Blue\n";
    ofs.flush();
    ofs.close();
    
    if (nextVehicleId() != 3) throw std::runtime_error("Next vehicle ID should be 3 after two vehicles");
    if (!silentMode) std::cout << "[PASS] test_nextVehicleId\n";
}

void test_loadVehicles_emptyFile() {
    clearTestFiles();
    auto vehicles = loadVehicles();
    if (!vehicles.empty()) throw std::runtime_error("Empty file should return empty vehicle vector");
    if (!silentMode) std::cout << "[PASS] test_loadVehicles_emptyFile\n";
}

void test_loadVehicles_validData() {
    clearTestFiles();
    std::ofstream ofs(VEHICLE_FILE);
    ofs << "1|1|ABC123|Honda|Red\n";
    ofs << "2|2|XYZ789|Toyota|Blue\n";
    ofs.close();
    
    auto vehicles = loadVehicles();
    if (vehicles.size() != 2) throw std::runtime_error("Should load two vehicles");
    if (vehicles[0].id != 1) throw std::runtime_error("Vehicle 1 ID should be 1");
    if (vehicles[0].customerId != 1) throw std::runtime_error("Vehicle 1 customer ID should be 1");
    if (vehicles[0].regNo != "ABC123") throw std::runtime_error("Vehicle 1 regNo should match");
    if (vehicles[1].id != 2) throw std::runtime_error("Vehicle 2 ID should be 2");
    if (vehicles[1].regNo != "XYZ789") throw std::runtime_error("Vehicle 2 regNo should match");
    if (!silentMode) std::cout << "[PASS] test_loadVehicles_validData\n";
}

void test_saveVehicles() {
    clearTestFiles();
    std::vector<Vehicle> vehicles = {
        {1, 1, "ABC123", "Honda", "Red"},
        {2, 2, "XYZ789", "Toyota", "Blue"}
    };
    saveVehicles(vehicles);
    
    std::ifstream ifs(VEHICLE_FILE);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    ifs.close();
    
    if (lines.size() != 2) throw std::runtime_error("Should save two vehicles");
    if (lines[0] != "1|1|ABC123|Honda|Red") throw std::runtime_error("Vehicle 1 data should match");
    if (lines[1] != "2|2|XYZ789|Toyota|Blue") throw std::runtime_error("Vehicle 2 data should match");
    if (!silentMode) std::cout << "[PASS] test_saveVehicles\n";
}

void test_deleteVehicle() {
    clearTestFiles();
    std::vector<Vehicle> vehicles = {
        {1, 1, "ABC123", "Honda", "Red"},
        {2, 2, "XYZ789", "Toyota", "Blue"}
    };
    saveVehicles(vehicles);
    
    vehicles.erase(std::remove_if(vehicles.begin(), vehicles.end(), 
        [](const Vehicle& v) { return v.id == 1; }), vehicles.end());
    saveVehicles(vehicles);
    
    vehicles = loadVehicles();
    if (vehicles.size() != 1) throw std::runtime_error("Should have one vehicle after deletion");
    if (vehicles[0].id != 2) throw std::runtime_error("Remaining vehicle ID should be 2");
    if (vehicles[0].regNo != "XYZ789") throw std::runtime_error("Remaining vehicle regNo should match");
    if (!silentMode) std::cout << "[PASS] test_deleteVehicle\n";
}

void test_addVehicle_duplicateRegNo() {
    clearTestFiles();
    std::vector<Vehicle> vehicles = {
        {1, 1, "ABC123", "Honda", "Red"},
        {2, 2, "ABC123", "Toyota", "Blue"}
    };
    saveVehicles(vehicles);
    
    vehicles = loadVehicles();
    if (vehicles.size() != 2) throw std::runtime_error("Duplicate registration numbers should be allowed");
    if (!silentMode) std::cout << "[PASS] test_addVehicle_duplicateRegNo\n";
}

// =============================
// 📌 Service Test Functions
// =============================
void test_nextServiceId() {
    clearTestFiles();
    if (nextServiceId() != 1) throw std::runtime_error("Initial service ID should be 1");
    
    std::ofstream ofs(SERVICES_FILE);
    ofs << "1|Oil Change|1200\n";
    ofs << "2|Brake Inspection|800\n";
    ofs.flush();
    ofs.close();
    
    if (nextServiceId() != 3) throw std::runtime_error("Next service ID should be 3 after two services");
    if (!silentMode) std::cout << "[PASS] test_nextServiceId\n";
}

void test_loadServices_emptyFile() {
    clearTestFiles();
    auto services = loadServices();
    if (!services.empty()) throw std::runtime_error("Empty file should return empty service vector");
    if (!silentMode) std::cout << "[PASS] test_loadServices_emptyFile\n";
}

void test_loadServices_validData() {
    clearTestFiles();
    std::ofstream ofs(SERVICES_FILE);
    ofs << "1|Oil Change|1200\n";
    ofs << "2|Brake Inspection|800\n";
    ofs.close();
    
    auto services = loadServices();
    if (services.size() != 2) throw std::runtime_error("Should load two services");
    if (services[0].id != 1) throw std::runtime_error("Service 1 ID should be 1");
    if (services[0].name != "Oil Change") throw std::runtime_error("Service 1 name should match");
    if (services[0].price != 1200) throw std::runtime_error("Service 1 price should match");
    if (services[1].id != 2) throw std::runtime_error("Service 2 ID should be 2");
    if (!silentMode) std::cout << "[PASS] test_loadServices_validData\n";
}

void test_saveServices() {
    clearTestFiles();
    std::vector<ServiceItem> services = {
        {1, "Oil Change", 1200},
        {2, "Brake Inspection", 800}
    };
    saveServices(services);
    
    std::ifstream ifs(SERVICES_FILE);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    ifs.close();
    
    if (lines.size() != 2) throw std::runtime_error("Should save two services");
    if (lines[0] != "1|Oil Change|1200") throw std::runtime_error("Service 1 data should match");
    if (lines[1] != "2|Brake Inspection|800") throw std::runtime_error("Service 2 data should match");
    if (!silentMode) std::cout << "[PASS] test_saveServices\n";
}

void test_ensureDefaultServices() {
    clearTestFiles();
    ensureDefaultServices();
    auto services = loadServices();
    if (services.size() != 6) throw std::runtime_error("Should load 6 default services");
    if (services[0].id != 1) throw std::runtime_error("First default service ID should be 1");
    if (services[0].name != "Oil Change") throw std::runtime_error("First default service name should match");
    if (services[0].price != 1200) throw std::runtime_error("First default service price should match");
    if (services[5].name != "General Service") throw std::runtime_error("Last default service name should match");
    if (services[5].price != 1500) throw std::runtime_error("Last default service price should match");
    if (!silentMode) std::cout << "[PASS] test_ensureDefaultServices\n";
}

void test_deleteService() {
    clearTestFiles();
    std::vector<ServiceItem> services = {
        {1, "Oil Change", 1200},
        {2, "Brake Inspection", 800}
    };
    saveServices(services);
    
    services.erase(std::remove_if(services.begin(), services.end(), 
        [](const ServiceItem& s) { return s.id == 1; }), services.end());
    saveServices(services);
    
    services = loadServices();
    if (services.size() != 1) throw std::runtime_error("Should have one service after deletion");
    if (services[0].id != 2) throw std::runtime_error("Remaining service ID should be 2");
    if (services[0].name != "Brake Inspection") throw std::runtime_error("Remaining service name should match");
    if (!silentMode) std::cout << "[PASS] test_deleteService\n";
}

void test_addService_zeroPrice() {
    clearTestFiles();
    std::vector<ServiceItem> services = {
        {1, "Free Service", 0}
    };
    saveServices(services);
    
    services = loadServices();
    if (services.size() != 1) throw std::runtime_error("Service with zero price should be added");
    if (services[0].price != 0) throw std::runtime_error("Service price should be 0");
    if (!silentMode) std::cout << "[PASS] test_addService_zeroPrice\n";
}

// =============================
// 📌 Discount Test Functions
// =============================
void test_nextDiscountId() {
    clearTestFiles();
    if (nextDiscountId() != 1) throw std::runtime_error("Initial discount ID should be 1");
    
    std::ofstream ofs(DISCOUNT_FILE);
    ofs << "1|New Year Offer|10|New Year 10% off\n";
    ofs << "2|Diwali Special|15|Festival offer\n";
    ofs.flush();
    ofs.close();
    
    if (nextDiscountId() != 3) throw std::runtime_error("Next discount ID should be 3 after two discounts");
    if (!silentMode) std::cout << "[PASS] test_nextDiscountId\n";
}

void test_loadDiscounts_emptyFile() {
    clearTestFiles();
    auto discounts = loadDiscounts();
    if (!discounts.empty()) throw std::runtime_error("Empty file should return empty discount vector");
    if (!silentMode) std::cout << "[PASS] test_loadDiscounts_emptyFile\n";
}

void test_loadDiscounts_validData() {
    clearTestFiles();
    std::ofstream ofs(DISCOUNT_FILE);
    ofs << "1|New Year Offer|10|New Year 10% off\n";
    ofs << "2|Diwali Special|15|Festival offer\n";
    ofs.close();
    
    auto discounts = loadDiscounts();
    if (discounts.size() != 2) throw std::runtime_error("Should load two discounts");
    if (discounts[0].id != 1) throw std::runtime_error("Discount 1 ID should be 1");
    if (discounts[0].name != "New Year Offer") throw std::runtime_error("Discount 1 name should match");
    if (discounts[0].percent != 10) throw std::runtime_error("Discount 1 percent should match");
    if (discounts[0].note != "New Year 10% off") throw std::runtime_error("Discount 1 note should match");
    if (!silentMode) std::cout << "[PASS] test_loadDiscounts_validData\n";
}

void test_saveDiscounts() {
    clearTestFiles();
    std::vector<Discount> discounts = {
        {1, "New Year Offer", 10, "New Year 10% off"},
        {2, "Diwali Special", 15, "Festival offer"}
    };
    saveDiscounts(discounts);
    
    std::ifstream ifs(DISCOUNT_FILE);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    ifs.close();
    
    if (lines.size() != 2) throw std::runtime_error("Should save two discounts");
    if (lines[0] != "1|New Year Offer|10|New Year 10% off") throw std::runtime_error("Discount 1 data should match");
    if (lines[1] != "2|Diwali Special|15|Festival offer") throw std::runtime_error("Discount 2 data should match");
    if (!silentMode) std::cout << "[PASS] test_saveDiscounts\n";
}

void test_ensureDefaultDiscounts() {
    clearTestFiles();
    ensureDefaultDiscounts();
    auto discounts = loadDiscounts();
    if (discounts.size() != 3) throw std::runtime_error("Should load 3 default discounts");
    if (discounts[0].id != 1) throw std::runtime_error("First default discount ID should be 1");
    if (discounts[0].name != "New Year Offer") throw std::runtime_error("First default discount name should match");
    if (discounts[0].percent != 10) throw std::runtime_error("First default discount percent should match");
    if (!silentMode) std::cout << "[PASS] test_ensureDefaultDiscounts\n";
}

void test_deleteDiscount() {
    clearTestFiles();
    std::vector<Discount> discounts = {
        {1, "New Year Offer", 10, "New Year 10% off"},
        {2, "Diwali Special", 15, "Festival offer"}
    };
    saveDiscounts(discounts);
    
    discounts.erase(std::remove_if(discounts.begin(), discounts.end(), 
        [](const Discount& d) { return d.id == 1; }), discounts.end());
    saveDiscounts(discounts);
    
    discounts = loadDiscounts();
    if (discounts.size() != 1) throw std::runtime_error("Should have one discount after deletion");
    if (discounts[0].id != 2) throw std::runtime_error("Remaining discount ID should be 2");
    if (discounts[0].name != "Diwali Special") throw std::runtime_error("Remaining discount name should match");
    if (!silentMode) std::cout << "[PASS] test_deleteDiscount\n";
}

void test_addDiscount_zeroPercent() {
    clearTestFiles();
    std::vector<Discount> discounts = {
        {1, "Zero Discount", 0, "No discount"}
    };
    saveDiscounts(discounts);
    
    discounts = loadDiscounts();
    if (discounts.size() != 1) throw std::runtime_error("Discount with zero percent should be added");
    if (discounts[0].percent != 0) throw std::runtime_error("Discount percent should be 0");
    if (!silentMode) std::cout << "[PASS] test_addDiscount_zeroPercent\n";
}

// =============================
// 📌 Service History Test Functions
// =============================
void test_nextHistoryId() {
    clearTestFiles();
    if (nextHistoryId() != 1) throw std::runtime_error("Initial history ID should be 1");
    
    std::ofstream ofs(HISTORY_FILE);
    ofs << "1|1|1|1,2|2023-10-10 10:00:00|2000|-1|0|2000|Pending\n";
    ofs << "2|2|2|3|2023-10-11 11:00:00|600|-1|0|600|Completed\n";
    ofs.flush();
    ofs.close();
    
    if (nextHistoryId() != 3) throw std::runtime_error("Next history ID should be 3 after two entries");
    if (!silentMode) std::cout << "[PASS] test_nextHistoryId\n";
}

void test_loadHistory_emptyFile() {
    clearTestFiles();
    auto history = loadHistory();
    if (!history.empty()) throw std::runtime_error("Empty file should return empty history vector");
    if (!silentMode) std::cout << "[PASS] test_loadHistory_emptyFile\n";
}

void test_loadHistory_validData() {
    clearTestFiles();
    std::ofstream ofs(HISTORY_FILE);
    ofs << "1|1|1|1,2|2023-10-10 10:00:00|2000|-1|0|2000|Pending\n";
    ofs.close();
    
    auto history = loadHistory();
    if (history.size() != 1) throw std::runtime_error("Should load one history entry");
    if (history[0].historyId != 1) throw std::runtime_error("History ID should be 1");
    if (history[0].customerId != 1) throw std::runtime_error("Customer ID should be 1");
    if (history[0].vehicleId != 1) throw std::runtime_error("Vehicle ID should be 1");
    if (history[0].serviceIds.size() != 2) throw std::runtime_error("Should have two service IDs");
    if (history[0].serviceIds[0] != 1) throw std::runtime_error("First service ID should be 1");
    if (history[0].serviceIds[1] != 2) throw std::runtime_error("Second service ID should be 2");
    if (history[0].subtotal != 2000) throw std::runtime_error("Subtotal should match");
    if (history[0].status != "Pending") throw std::runtime_error("Status should match");
    if (!silentMode) std::cout << "[PASS] test_loadHistory_validData\n";
}

void test_saveHistory() {
    clearTestFiles();
    std::vector<ServiceHistory> history = {
        {1, 1, 1, {1, 2}, "2023-10-10 10:00:00", 2000, -1, 0, 2000, "Pending"},
        {2, 2, 2, {3}, "2023-10-11 11:00:00", 600, -1, 0, 600, "Completed"}
    };
    saveHistory(history);
    
    std::ifstream ifs(HISTORY_FILE);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    ifs.close();
    
    if (lines.size() != 2) throw std::runtime_error("Should save two history entries");
    if (lines[0] != "1|1|1|1,2|2023-10-10 10:00:00|2000|-1|0|2000|Pending") throw std::runtime_error("History 1 data should match");
    if (lines[1] != "2|2|2|3|2023-10-11 11:00:00|600|-1|0|600|Completed") throw std::runtime_error("History 2 data should match");
    if (!silentMode) std::cout << "[PASS] test_saveHistory\n";
}

void test_addHistoryEntry() {
    clearTestFiles();
    ServiceHistory h = {1, 1, 1, {1, 2}, "2023-10-10 10:00:00", 2000, -1, 0, 2000, "Pending"};
    addHistoryEntry(h);
    
    auto history = loadHistory();
    if (history.size() != 1) throw std::runtime_error("Should add one history entry");
    if (history[0].historyId != 1) throw std::runtime_error("History ID should be 1");
    if (history[0].serviceIds.size() != 2) throw std::runtime_error("Should have two service IDs");
    if (history[0].subtotal != 2000) throw std::runtime_error("Subtotal should match");
    if (!silentMode) std::cout << "[PASS] test_addHistoryEntry\n";
}

void test_addHistoryEntry_emptyServiceIds() {
    clearTestFiles();
    ServiceHistory h = {1, 1, 1, {}, "2023-10-10 10:00:00", 0, -1, 0, 0, "Pending"};
    addHistoryEntry(h);
    
    auto history = loadHistory();
    if (history.size() != 1) throw std::runtime_error("History entry with empty service IDs should be added");
    if (!history[0].serviceIds.empty()) throw std::runtime_error("Service IDs should be empty");
    if (!silentMode) std::cout << "[PASS] test_addHistoryEntry_emptyServiceIds\n";
}

// =============================
// 📌 Unified Test Runner Macro
// =============================
#define RUN_TEST(testFunc) \
    do { \
        try { \
            setSilentMode(true); \
            testFunc(); \
            std::cout << "[PASS] " << #testFunc << std::endl; \
        } catch (const std::exception& e) { \
            std::cout << "[FAIL] " << #testFunc << " - Exception: " << e.what() << std::endl; \
        } catch (...) { \
            std::cout << "[FAIL] " << #testFunc << " - Unknown error" << std::endl; \
        } \
    } while(0)

// =============================
// 📌 Main Function
// =============================
int main() {
    std::cout << "===== Auto Service Management Unit Test Suite =====" << std::endl;

    // Customer Tests
    RUN_TEST(test_nextCustomerId);
    RUN_TEST(test_loadCustomers_emptyFile);
    RUN_TEST(test_loadCustomers_validData);
    RUN_TEST(test_loadCustomers_invalidData);
    RUN_TEST(test_saveCustomers);
    RUN_TEST(test_findCustomerById);
    RUN_TEST(test_deleteCustomer);
    RUN_TEST(test_addCustomer_duplicateId);
    RUN_TEST(test_addCustomer_emptyName);

    // Vehicle Tests
    RUN_TEST(test_nextVehicleId);
    RUN_TEST(test_loadVehicles_emptyFile);
    RUN_TEST(test_loadVehicles_validData);
    RUN_TEST(test_saveVehicles);
    RUN_TEST(test_deleteVehicle);
    RUN_TEST(test_addVehicle_duplicateRegNo);

    // Service Tests
    RUN_TEST(test_nextServiceId);
    RUN_TEST(test_loadServices_emptyFile);
    RUN_TEST(test_loadServices_validData);
    RUN_TEST(test_saveServices);
    RUN_TEST(test_ensureDefaultServices);
    RUN_TEST(test_deleteService);
    RUN_TEST(test_addService_zeroPrice);

    // Discount Tests
    RUN_TEST(test_nextDiscountId);
    RUN_TEST(test_loadDiscounts_emptyFile);
    RUN_TEST(test_loadDiscounts_validData);
    RUN_TEST(test_saveDiscounts);
    RUN_TEST(test_ensureDefaultDiscounts);
    RUN_TEST(test_deleteDiscount);
    RUN_TEST(test_addDiscount_zeroPercent);

    // Service History Tests
    RUN_TEST(test_nextHistoryId);
    RUN_TEST(test_loadHistory_emptyFile);
    RUN_TEST(test_loadHistory_validData);
    RUN_TEST(test_saveHistory);
    RUN_TEST(test_addHistoryEntry);
    RUN_TEST(test_addHistoryEntry_emptyServiceIds);

    std::cout << "=========== Test Suite Completed ===========" << std::endl;

    // Clean up test files
    clearTestFiles();
    return 0;
}