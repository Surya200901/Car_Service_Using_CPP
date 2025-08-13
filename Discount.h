// Discount.h
#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <string>
#include <vector>

struct Discount {
    int id;
    std::string name;
    double percent; // e.g., 10 for 10%
    std::string note;
};

std::vector<Discount> loadDiscounts();
void saveDiscounts(const std::vector<Discount>& list);
int nextDiscountId();
void ensureDefaultDiscounts();
void viewDiscounts();
void addDiscountInteractive();
void updateDiscount();
void deleteDiscount();

#endif // DISCOUNT_H