# CAR_SERVICE_NEW

A C++ console application for managing a car service center, supporting customer, vehicle, service, discount, and service history management.

---

## Features

- **Customer Management**
  - Add, view, search, update, and delete customers.
- **Vehicle Management**
  - Register, view, update, and delete vehicles linked to customers.
- **Service Management**
  - Add, view, update, and delete available car services.
  - Default services are auto-populated if none exist.
- **Discount Management**
  - Add, view, update, and delete discounts.
  - Default discounts are auto-populated if none exist.
- **Service Booking**
  - Book services for a customer's vehicle.
  - Apply discounts to bookings.
  - Generate bills for completed or pending bookings.
- **Service History**
  - View all service bookings and their statuses.
  - Mark services as completed.
- **Data Persistence**
  - All data is stored in plain text files for easy inspection and backup.

---

## File Structure

- `main.cpp` - Main application logic and menu system.
- `Customer.h` / `Customer.cpp` - Customer data structures and functions.
- `Vehicle.h` / `Vehicle.cpp` - Vehicle data structures and functions.
- `Service.h` / `Service.cpp` - Service and service history data structures and functions.
- `Discount.h` / `Discount.cpp` - Discount data structures and functions.
- `customers.txt`, `vehicles.txt`, `services.txt`, `discounts.txt`, `service_history.txt` - Data storage files.
- `tests/` - Unit tests and test data files.
- `.vscode/` - VSCode configuration for building and debugging.

---

## How to Build

1. **Requirements**
   - C++ compiler (e.g., g++)
   - [Optional] Visual Studio Code with C/C++ extensions

2. **Build Command**
   - On Windows (MinGW):
     ```sh
     g++ *.cpp -o main
     ```
   - Or use the build tasks in [`.vscode/launch.json`](.vscode/launch.json).

---

## How to Run

- Run the executable:
  ```sh
  ./main.exe
  ```
- Follow the on-screen menu to manage customers, vehicles, services, discounts, and bookings.

---

## Data Files

- **customers.txt**: Stores customer records.
- **vehicles.txt**: Stores vehicle records.
- **services.txt**: Stores available services.
- **discounts.txt**: Stores available discounts.
- **service_history.txt**: Stores all service bookings and their statuses.

Each file uses `|` as a field separator.

---

## Unit Testing

- All core logic is covered by unit tests in [tests/test.cpp](tests/test.cpp).
- To run tests:
  ```sh
  g++ -std=c++17 -DTEST_MODE -I. -o test tests/test.cpp Customer.cpp Discount.cpp Service.cpp Vehicle.cpp
  ./test.exe
  ```

---

## Extending the Project

- Add new fields to the data structures in the header files.
- Update the corresponding load/save logic in the `.cpp` files.
- Add new menu options in `main.cpp` as needed.

---

## Notes

- The application is designed for simplicity and clarity, using only standard C++ and file I/O.
- No external libraries are required.
- Data files are human-readable and can be edited manually if needed.