#include "system.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

// ---------------- ARRAY ----------------
Residents arr[1000];
int sizeArr = 0;

// ---------------- LOAD CSV ----------------
void loadCSV(string filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Error opening file\n";
        return;
    }

    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;

        getline(ss, arr[sizeArr].id, ',');
        getline(ss, temp, ','); arr[sizeArr].age = stoi(temp);
        getline(ss, arr[sizeArr].mode, ',');
        getline(ss, temp, ','); arr[sizeArr].distance = stod(temp);
        getline(ss, temp, ','); arr[sizeArr].factor = stod(temp);
        getline(ss, temp, ','); arr[sizeArr].days = stoi(temp);

        sizeArr++;
    }

    cout << "Dataset Loaded!\n";
}

// ---------------- DISPLAY ----------------
void displayHeader() {
    cout << "----------------------------------------------------\n";
    cout << "| ID  | Age | Mode      | Distance| Days| CO2      |\n";
    cout << "----------------------------------------------------\n";
}

void printRow(Residents r) {
    double emission = r.distance * r.factor * r.days;

    cout << "| " << setw(4) << r.id
         << "| " << setw(4) << r.age
         << "| " << setw(10) << r.mode
         << "| " << setw(8) << r.distance
         << "| " << setw(4) << r.days
         << "| " << setw(8) << emission << " |\n";
}

// ---------------- Choose DataSet Menu-----------
int chooseDataset() {
    int opt;

    cout << "========================\n";
    cout << "Select Dataset\n";
    cout << "========================\n";
    cout << "1. City A\n";
    cout << "2. City B\n";
    cout << "3. City C\n";
    cout << "4. Exit Program\n";
    cout << "Select: ";
    cin >> opt;

    return opt;
}

void loadSelectedDataset(int opt) {

    sizeArr = 0;

    if (opt == 1) {
        loadCSV("dataset1-cityA.csv");
    }
    else if (opt == 2) {
        loadCSV("dataset2-cityB.csv");
    }
    else if (opt == 3) {
        loadCSV("dataset3-cityC.csv");
    }
    else {
        cout << "Invalid dataset.\n";
    }
}

// Binary Search Age
void binarySearchAge(int target) {
    int left = 0, right = sizeArr - 1;
    bool found = false;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (arr[mid].age == target) {

            displayHeader();

            int i = mid;
            while (i >= 0 && arr[i].age == target) {
                printRow(arr[i]);
                i--;
            }

            i = mid + 1;
            while (i < sizeArr && arr[i].age == target) {
                printRow(arr[i]);
                i++;
            }

            found = true;
            break;
        }

        else if (arr[mid].age < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    if (!found) cout << "No record found.\n";
}

// Binary Search Mode
void binarySearchMode(string target) {
    int left = 0, right = sizeArr - 1;
    bool found = false;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (arr[mid].mode == target) {

            displayHeader();

            int i = mid;
            while (i >= 0 && arr[i].mode == target) {
                printRow(arr[i]);
                i--;
            }

            i = mid + 1;
            while (i < sizeArr && arr[i].mode == target) {
                printRow(arr[i]);
                i++;
            }

            found = true;
            break;
        }

        else if (arr[mid].mode < target)
            left = mid + 1;
        else
            right = mid - 1;
    }

    if (!found) cout << "No record found.\n";
}

void binarySearchDistance(double threshold) {
    int left = 0, right = sizeArr - 1;
    int result = -1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (arr[mid].distance > threshold) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    if (result == -1) {
        cout << "No record found.\n";
        return;
    }

    displayHeader();

    for (int i = result; i < sizeArr; i++) {
        printRow(arr[i]);
    }
}

//-----------------Display Sort list-----
void displaySortedList() {
    displayHeader();
    for (int i = 0; i < sizeArr; i++) {
        printRow(arr[i]);
    }
}

void sortByCarbon() {
    for (int i = 1; i < sizeArr; i++) {
        Residents key = arr[i];
        int j = i - 1;

        double keyEmission = key.distance * key.factor * key.days;

        while (j >= 0) {
            double currEmission = arr[j].distance * arr[j].factor * arr[j].days;

            if (currEmission <= keyEmission) break;

            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void sortByDistance() {
    for (int i = 1; i < sizeArr; i++) {
        Residents key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j].distance > key.distance) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int sortingMenu() {
    int opt;

    cout << "\n===================\n";
    cout << "Sorting Algorithms\n";
    cout << "===================\n";
    cout << "1. Insertion Sort\n";
    cout << "Select: ";
    cin >> opt;

    if (opt != 1) {
        cout << "Invalid choice.\n";
        return -1;
    }

    return opt;
}

// ---------------- INSERTION SORT ----------------
void insertionSort() {
    for (int i = 1; i < sizeArr; i++) {
        Residents key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j].age > key.age) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ---------------- SEARCH ----------------
void searchMenu() {
    int opt;
    string mode;
    int age;
    double distance;

    cout << "\nSearch By\n";
    cout << "1. Age\n";
    cout << "2. Mode of Transport\n";
    cout << "3. Daily Distance Threshold\n";
    cout << "Select: ";
    cin >> opt;

    bool found = false;

    // ---------------- INPUT ----------------

    if (opt == 1) {
        cout << "Enter Age: ";
        cin >> age;
        insertionSort();
        binarySearchAge(age);
    }
    else if (opt == 2) {
        cout << "Enter Mode (Car/Bus/Bicycle/Walking/Carpool/School Bus) : ";
        cin.ignore();
        getline(cin, mode);

        for (int i = 1; i < sizeArr; i++) {
            Residents key = arr[i];
            int j = i - 1;

            while (j >= 0 && arr[j].mode > key.mode) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        insertionSort();
        binarySearchMode(mode);
    }
    else if (opt == 3) {
        cout << "Enter Distance Threshold (> km): ";
        cin >> distance;

        sortByDistance();
        binarySearchDistance(distance);        
    }
    else {
        cout << "Invalid option.\n";
        return;
    }
}

// ---------------- AGE GROUP MENU ----------------
int getAgeGroup(int& minAge, int& maxAge) {
    int opt;

    cout << "\nAge Group\n";
    cout << "1. 6-17 (Children & Teenagers)\n";
    cout << "2. 18-25 (University Students)\n";
    cout << "3. 26-45 (Working Adults - Early Career)\n";
    cout << "4. 46-60 (Working Adults - Late Career)\n";
    cout << "5. 61-100 (Senior Citizens / Retirees)\n";
    cout << "Select: ";
    cin >> opt;

    switch (opt) {
        case 1: minAge = 6; maxAge = 17; break;
        case 2: minAge = 18; maxAge = 25; break;
        case 3: minAge = 26; maxAge = 45; break;
        case 4: minAge = 46; maxAge = 60; break;
        case 5: minAge = 61; maxAge = 100; break;
        default: return -1;
    }

    return opt;
}

// ---------------- ANALYSIS ----------------
void analysis() {

    // Sorting Menu
    cout << "\n===================\n";
    cout << "Sorting Algorithms\n";
    cout << "===================\n";
    cout << "1. Insertion Sort\n";
    cout << "Select: ";

    int opt;
    cin >> opt;

    if (opt != 1) {
        cout << "Invalid choice.\n";
        return;
    }

    // Measure time
    auto start = high_resolution_clock::now();

    insertionSort();

    auto end = high_resolution_clock::now();
    double timeTaken = duration<double, milli>(end - start).count();

    // Age Group
    int minAge, maxAge;
    int group = getAgeGroup(minAge, maxAge);
    if (group == -1) return;

    cout << "\nAge Group: " << minAge << "-" << maxAge << endl;

    // ---------------- ANALYSIS ----------------
    int count = 0;
    double totalEmission = 0;

    int transportCount[6] = {0};
    double transportEmission[6] = {0};

    string transportName[6] = {
        "Car","Bus","Bicycle","Walking","Carpool","School Bus"
    };

    for (int i = 0; i < sizeArr; i++) {
        if (arr[i].age >= minAge && arr[i].age <= maxAge) {

            double emission = arr[i].distance * arr[i].factor * arr[i].days;

            totalEmission += emission;
            count++;

            int index = 0;

            if (arr[i].mode == "Car") index = 0;
            else if (arr[i].mode == "Bus") index = 1;
            else if (arr[i].mode == "Bicycle") index = 2;
            else if (arr[i].mode == "Walking") index = 3;
            else if (arr[i].mode == "Carpool") index = 4;
            else index = 5;

            transportCount[index]++;
            transportEmission[index] += emission;
        }
    }

    // ---------------- OUTPUT ----------------
    cout << "\n-----------------------------------------------------------------------\n";
    cout << left << setw(20) << "Mode of Transport"
        << setw(10) << "Count"
        << setw(28) << "Total Emission (kg CO2)"
        << "Average" << endl;
    cout << "-----------------------------------------------------------------------\n";

    for (int i = 0; i < 6; i++) {
        if (transportCount[i] > 0) {

            cout << left << setw(20) << transportName[i]
                << setw(10) << transportCount[i]
                << setw(28) << fixed << setprecision(2) << transportEmission[i]
                << fixed << setprecision(2)
                << (transportEmission[i] / transportCount[i])
                << endl;
        }
    }

    cout << "---------------------------------------------\n";
    cout << "Total Emission for Age Group: " << totalEmission << " kg CO2\n";

    // ---------------- MEMORY ----------------
    int bytesPerElement = sizeof(Residents);
    cout << "\n--- Memory Usage ---\n";
    cout << "Elements (n): " << count << endl;
    cout << "Memory Usage: " << count << " x " << bytesPerElement
         << " bytes = " << count * bytesPerElement << " bytes\n";

    // ---------------- TIME ----------------
    cout << "\n--- Time Complexity (Insertion Sort) ---\n";
    cout << "Time Complexity: O(n^2)\n";
    cout << "Time Taken: " << timeTaken << " ms\n";

    // ---------------- EXTRA MENU ----------------
    int choice;
    cout << "\n====================\n";
    cout << "1. Display Sorted by Age\n";
    cout << "2. Display Sorted by Carbon Emission\n";
    cout << "3. Display Sorted by Distance\n";

    cout << "Select: ";
    cin >> choice;

    if (choice == 1) {
        insertionSort(); 
        displaySortedList();
    }
    else if (choice == 2) {
        sortByCarbon(); 
        displaySortedList();
    }
    else if (choice == 3) {
        sortByDistance(); 
        displaySortedList();
    }
    else if (choice == 4) {
        cout << "Exiting display menu...\n";
    }
    else {
        cout << "Invalid choice\n";
    }
}


// ---------------- MAIN ----------------
int binaryInsertion() {
    cout << "\n==========================================\n";
    cout << "Welcome to the Carbon Emission System\n";
    cout << "==========================================\n";

    int datasetChoice;

    while (true) { 

        datasetChoice = chooseDataset();

        if (datasetChoice == 4) {
            cout << "Exiting program\n";
            break;
        }

        loadSelectedDataset(datasetChoice);

        int choice;

        do {
            cout << "1. Search Data\n";
            cout << "2. Carbon Emission Analysis\n";
            cout << "3. Back to Dataset Menu\n";
            cout << "Select: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    searchMenu();
                    break;

                case 2:
                    analysis();
                    break;

                case 3:
                    cout << "Returning to dataset menu...\n";
                    break;

                default:
                    cout << "Invalid choice\n";
            }

        } while (choice != 3);
    }

    return 0;
}
