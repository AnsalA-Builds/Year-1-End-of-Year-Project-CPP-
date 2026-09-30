#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits> // Include limits header for numeric_limits

using namespace std;

// Define a class to represent a Vehicle
class Vehicle {
public:
    string make;
    string model;
    int year;
    double price;
    
    // Default constructor
    Vehicle() {}
    
    // Parameterized constructor
    Vehicle(const string& make, const string& model, int year, double price) 
        : make(make), model(model), year(year), price(price) {}
    
    // Function to display vehicle information
    void display() const {
        cout << "Make: " << make << ", Model: " << model << ", Year: " << year << ", Price: $" << price << endl;
    }
};

// Define a class to represent the Car Dealership
class CarDealership {
private:
    vector<Vehicle> inventory; // Vector to hold vehicle inventory
    string adminUsername; // Admin username
    string adminPassword; // Admin password
    bool loggedIn; // Flag to indicate if a user is logged in

    // Function to read inventory data from a file
    void readInventoryFromFile() {
        ifstream infile("inventory.txt");
        if (infile.is_open()) {
            // Read inventory data from file and populate the inventory vector
            Vehicle vehicle;
            while (infile >> vehicle.make >> vehicle.model >> vehicle.year >> vehicle.price) {
                inventory.push_back(vehicle);
            }
            infile.close();
        } else {
            cerr << "Error: Unable to open inventory file." << endl;
        }
    }

    // Function to write inventory data to a file
    void writeInventoryToFile() const {
        ofstream outfile("inventory.txt");
        if (outfile.is_open()) {
            // Write inventory data to file
            for (const auto& vehicle : inventory) {
                outfile << vehicle.make << " " << vehicle.model << " " << vehicle.year << " " << vehicle.price << endl;
            }
            outfile.close();
        } else {
            cerr << "Error: Unable to open inventory file for writing." << endl;
        }
    }

public:
    // Constructor
    CarDealership() : adminUsername("Admin"), adminPassword("Lexus"), loggedIn(false) {}

    // Function to handle user sign up
    void signUp() {
        string username, password;
        cout << "Please Enter username: ";
        cin >> username;
        cout << "Please Enter password: ";
        cin >> password;

        // Store user data into an external file
        ofstream outfile("userdata.txt", ios::app);
        if (outfile.is_open()) {
            outfile << username << " " << password << endl;
            cout << "User has signed up successfully! \n" << endl;
            outfile.close();
        } else {
            cerr << "Error: Unable to open file." << endl;
        }
    }

    // Function to handle user login
    bool login() {
        string username, password, u, p;
        cout << "Enter username: \n";
        cin >> username;
        cout << "Enter password: \n";
        cin >> password;

        // Check user credentials against stored data
        ifstream infile("userdata.txt");
        if (infile.is_open()) {
            while (infile >> u >> p) {
                if (u == username && p == password) {
                    cout << "Login successful!\n" << endl;
                    infile.close();
                    loggedIn = true;
                    return true;
                }
            }
            infile.close();
            cout << "Invalid username or password. \n" << endl;
            return false;
        } else {
            cerr << "Error: Unable to open file. \n" << endl;
            return false;
        }
    }

    // Function to add a new vehicle to the inventory
    void addVehicle() {
        Vehicle newVehicle;
        cout << "Enter make: ";
        cin >> newVehicle.make;
        cout << "Enter model: ";
        cin >> newVehicle.model;
        cout << "Enter year: ";
        cin >> newVehicle.year;
        cout << "Enter price: ";
        cin >> newVehicle.price;

        inventory.push_back(newVehicle);
        cout << "Vehicle added successfully! \n" << endl;

        // Write updated inventory to file
        writeInventoryToFile();
    }

    // Function to display the inventory
    void displayInventory() const {
        if (inventory.empty()) {
            cout << "Inventory is empty. \n" << endl;
        } else {
            cout << "Inventory:" << endl;
            for (const auto& vehicle : inventory) {
                vehicle.display();
            }
        }
    }

    // Function to inquire about a specific vehicle
    void inquireVehicle() const {
        string make, model;
        cout << "Enter make: \n";
        cin >> make;
        cout << "Enter model: \n";
        cin >> model;

        bool found = false;
        for (const auto& vehicle : inventory) {
            if (vehicle.make == make && vehicle.model == model) {
                cout << "Vehicle found: \n" << endl;
                vehicle.display();
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Vehicle not found. \n" << endl;
        }
    }

    // Function for admin terminal
    void adminTerminal() {
        string username, password;
        cout << "Enter Admin username: \n";
        cin >> username;
        cout << "Enter Admin password: \n";
        cin >> password;

        if (username == adminUsername && password == adminPassword) {
            int choice;
            bool validChoice;
            do {
                cout << "Welcome to the Admin Terminal" << endl;
                cout << "1. Add Vehicle" << endl;
                cout << "2. Update Inventory" << endl;
                cout << "3. Exit Admin Terminal" << endl;
                cout << "Enter your choice: \n";
                cin >> choice;

                // Check if the input is valid
                validChoice = true;
                switch (choice) {
                    case 1:
                        addVehicle();
                        break;
                    case 2:
                        displayInventory();
                        break;
                    case 3:
                        return; // Exit admin terminal
                    default:
                        cout << "Invalid choice. Please try again.\n" << endl;
                        validChoice = false;
                        // Clear input buffer
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (!validChoice); // Loop until valid choice
        } else {
            cout << "Invalid admin credentials. Access denied. \n" << endl;
        }
    }

    // Function to start the car dealership system
    void start() {
        int choice;
        bool validChoice;
        do {
            cout << "Welcome to the online Car Dealership! " << endl;
            cout << "Select 1 in order to Sign Up" << endl;
            cout << "Select 2 in order to Login" << endl;
            cout << "Select 3 in order to Exit" << endl;
            cout << "Enter your choice: \n";
            cin >> choice;

            // Check if the input is valid
            validChoice = true;
            switch(choice) {
                case 1:
                    signUp();
                    break;
                case 2:
                    loggedIn = login();
                    break;
                case 3:
                    cout << "Exiting program. Goodbye! \n" << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again. \n" << endl;
                    validChoice = false;
                    // Clear input buffer
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!loggedIn || !validChoice); // Loop until logged in and valid choice

        // Read inventory data from file at program startup
        readInventoryFromFile();

        if (loggedIn) {
            while (true) {
                cout << "Welcome to the Car Management System" << endl;
                cout << "1. Browse Inventory" << endl;
                cout << "2. Inquire about a Vehicle" << endl;
                cout << "3. Schedule Maintenance" << endl;
                cout << "4. Admin Terminal" << endl;
                cout << "5. Logout" << endl;
                cout << "Enter your choice: \n";
                cin >> choice;

                // Check if the input is valid
                validChoice = true;
                switch(choice) {
                    case 1:
                        displayInventory();
                        break;
                    case 2:
                        inquireVehicle();
                        break;
                    case 3:
                        cout << "Maintenance scheduling feature is under development. \n" << endl;
                        break;
                    case 4:
                        adminTerminal();
                        break;
                    case 5:
                        loggedIn = false;
                        break;
                    default:
                        cout << "Invalid choice. Please try again. \n" << endl;
                        validChoice = false;
                        // Clear input buffer
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                if (!loggedIn || !validChoice) {
                    cout << "Press Enter to continue... \n";
                    cin.get();
                }
                
                if (!loggedIn) {
                    break; // Exit the loop if user logs out or invalid choice
                }
            }
        }
    }
};

int main() {
    CarDealership dealership;
    dealership.start();
    return 0;
}
