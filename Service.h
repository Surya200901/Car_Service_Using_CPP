// Service.h
#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <vector>

struct ServiceItem {
    int id;
    std::string name;
    double price;
};

struct ServiceHistory {
    int historyId;
    int customerId;
    int vehicleId;
    std::vector<int> serviceIds;
    std::string dateTime;
    double subtotal;
    int discountId; // -1 none
    double discountPercent;
    double total;
    std::string status; // "Pending" or "Completed"
};

std::vector<ServiceItem> loadServices();
void saveServices(const std::vector<ServiceItem>& list);
int nextServiceId();
int nextHistoryId();
void ensureDefaultServices();
void addServiceInteractive();
void viewServices();
void updateService();
void deleteService();
std::vector<ServiceHistory> loadHistory();
void saveHistory(const std::vector<ServiceHistory>& list);
std::string currentDateTime();
void addHistoryEntry(const ServiceHistory& h);
void viewServiceHistory();
void markHistoryCompleted();

#endif // SERVICE_H