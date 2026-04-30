#include "Assignment.hpp" // Pulls in all libraries, structs, and declarations!

// 1. Define Global Variables (No 'extern' here)
Resident cityA[MAX];
Resident cityB[MAX];
Resident cityC[MAX];

int sizeA = 0;
int sizeB = 0;
int sizeC = 0;

// ============================================================
// LOAD DATA FROM CSV FILE
// ============================================================
void loadData(string filename, Resident arr[], int& size) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header row

    while (getline(file, line) && size < MAX) {
        stringstream ss(line);
        string id, age, mode, dist, carbon, days;

        getline(ss, id,     ',');
        getline(ss, age,    ',');
        getline(ss, mode,   ',');
        getline(ss, dist,   ',');
        getline(ss, carbon, ',');
        getline(ss, days);

        if (id.empty()) continue;

        Resident r;
        r.id             = id;
        r.age            = stoi(age);
        r.transportMode  = mode;
        r.dailyDistance  = stod(dist);
        r.carbonEmission = stod(carbon);
        r.daysPerMonth   = stoi(days);

        arr[size] = r;
        size++;
    }

    file.close();
    cout << "Loaded " << size << " records from " << filename << endl;
}

// ============================================================
// MERGE SORT - DYNAMIC
// ============================================================
void merge(Resident arr[], int left, int mid, int right, int sortBy) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Resident* L = new Resident[n1];
    Resident* R = new Resident[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        bool conditionMet = false;

        if (sortBy == 1) {
            conditionMet = (L[i].age <= R[j].age);
        } 
        else if (sortBy == 2) {
            conditionMet = (L[i].transportMode <= R[j].transportMode);
        } 
        else if (sortBy == 3) {
            conditionMet = (L[i].dailyDistance <= R[j].dailyDistance);
        }

        if (conditionMet) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }

    delete[] L;
    delete[] R;
}

void mergeSort(Resident arr[], int left, int right, int sortBy) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, sortBy);
        mergeSort(arr, mid + 1, right, sortBy);
        merge(arr, left, mid, right, sortBy);
    }
}

// ============================================================
// COPY ARRAY 
// ============================================================
void copyArray(Resident src[], Resident dst[], int size) {
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

// ============================================================
// LINEAR SEARCH - by age
// ============================================================
void searchByAge(Resident arr[], int size, string cityName, int targetAge) {
    int count = 0;
    auto start = chrono::high_resolution_clock::now();

    cout << "\n--- Search by Age = " << targetAge << " in " << cityName << " ---" << endl;
    cout << left << setw(8)  << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
         << setw(12) << "Dist(km)" << setw(8) << "CO2" << setw(6) << "Days" << endl;
    cout << "-------------------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        if (arr[i].age == targetAge) {
            cout << left << setw(8)  << arr[i].id << setw(6) << arr[i].age 
                 << setw(15) << arr[i].transportMode << setw(12) << arr[i].dailyDistance 
                 << setw(8)  << arr[i].carbonEmission << setw(6) << arr[i].daysPerMonth << endl;
            count++;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    long long memoryUsed = size * sizeof(Resident);

    if (count == 0) cout << "No results found." << endl;
    else cout << "Total found: " << count << endl;

    cout << "-------------------------------------------------------" << endl;
    cout << "[Performance] Algorithm: Linear Search O(n)" << endl;
    cout << "[Performance] Time taken: " << duration << " microseconds" << endl;
    cout << "[Performance] Memory usage: " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
}

// ============================================================
// LINEAR SEARCH - by transport mode
// ============================================================
void searchByTransport(Resident arr[], int size, string cityName, string targetMode) {
    int count = 0;
    auto start = chrono::high_resolution_clock::now();

    cout << "\n--- Search by Transport = " << targetMode << " in " << cityName << " ---" << endl;
    cout << left << setw(8)  << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
         << setw(12) << "Dist(km)" << setw(8) << "CO2" << setw(6) << "Days" << endl;
    cout << "-------------------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        if (arr[i].transportMode == targetMode) {
            cout << left << setw(8)  << arr[i].id << setw(6) << arr[i].age 
                 << setw(15) << arr[i].transportMode << setw(12) << arr[i].dailyDistance 
                 << setw(8)  << arr[i].carbonEmission << setw(6) << arr[i].daysPerMonth << endl;
            count++;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    long long memoryUsed = size * sizeof(Resident);

    if (count == 0) cout << "No results found." << endl;
    else cout << "Total found: " << count << endl;

    cout << "-------------------------------------------------------" << endl;
    cout << "[Performance] Algorithm: Linear Search O(n)" << endl;
    cout << "[Performance] Time taken: " << duration << " microseconds" << endl;
    cout << "[Performance] Memory usage: " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
}

// ============================================================
// LINEAR SEARCH - by distance range
// ============================================================
void searchByDistance(Resident arr[], int size, string cityName, double minD, double maxD) {
    int count = 0;
    auto start = chrono::high_resolution_clock::now();

    cout << "\n--- Search by Distance (" << minD << "km - " << maxD << "km) in " << cityName << " ---" << endl;
    cout << left << setw(8)  << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
         << setw(12) << "Dist(km)" << setw(8) << "CO2" << setw(6) << "Days" << endl;
    cout << "-------------------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        if (arr[i].dailyDistance >= minD && arr[i].dailyDistance <= maxD) {
            cout << left << setw(8)  << arr[i].id << setw(6) << arr[i].age 
                 << setw(15) << arr[i].transportMode << setw(12) << arr[i].dailyDistance 
                 << setw(8)  << arr[i].carbonEmission << setw(6) << arr[i].daysPerMonth << endl;
            count++;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    long long memoryUsed = size * sizeof(Resident);

    if (count == 0) cout << "No results found." << endl;
    else cout << "Total found: " << count << endl;

    cout << "-------------------------------------------------------" << endl;
    cout << "[Performance] Algorithm: Linear Search O(n)" << endl;
    cout << "[Performance] Time taken: " << duration << " microseconds" << endl;
    cout << "[Performance] Memory usage: " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
}

// ============================================================
// DISPLAY RESULTS 
// ============================================================
void displaySorted(Resident arr[], int size, string cityName, int sortChoice) {
    Resident temp[MAX];
    copyArray(arr, temp, size);

    auto start = chrono::high_resolution_clock::now();
    mergeSort(temp, 0, size - 1, sortChoice);
    auto end = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    long long memoryUsed = size * sizeof(Resident);

    string sortName = "";
    if (sortChoice == 1) sortName = "Age";
    else if (sortChoice == 2) sortName = "Transport Mode";
    else if (sortChoice == 3) sortName = "Daily Distance";

    cout << "\n--- Sorted by " << sortName << ": " << cityName << " ---" << endl;
    cout << left << setw(8)  << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
         << setw(12) << "Dist(km)" << setw(8) << "CO2" << setw(6) << "Days" << endl;
    cout << "-------------------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        cout << left << setw(8)  << temp[i].id << setw(6) << temp[i].age 
             << setw(15) << temp[i].transportMode << setw(12) << temp[i].dailyDistance 
             << setw(8)  << temp[i].carbonEmission << setw(6) << temp[i].daysPerMonth << endl;
    }

    cout << "-------------------------------------------------------" << endl;
    cout << "[Performance] Algorithm: Merge Sort O(n log n)" << endl;
    cout << "[Performance] Time taken (Sort only): " << duration << " microseconds" << endl;
    cout << "[Performance] Memory usage: " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
}

// ============================================================
// CARBON EMISSION ANALYSIS
// ============================================================
void emissionAnalysis(Resident arr[], int size, string cityName) {

    string groupName[5] = {
        "6-17   (Children & Teenagers)",
        "18-25  (University Students)", 
        "26-45  (Working Adults - Early Career)",
        "46-60  (Working Adults - Late Career)",
        "61-100 (Senior Citizens / Retirees)"
    };

    int groupMin[5] = {6,  18, 26, 46, 61};
    int groupMax[5] = {17, 25, 45, 60, 100};

    cout << "\n--- Carbon Emission Analysis: " << cityName << " ---" << endl;

    for (int g = 0; g < 5; g++) {

        string modeList[10];
        int    modeCount[10]  = {0};
        double modeEmit[10]   = {0};
        int    numModes       = 0;
        double groupTotalEmit = 0;

        for (int i = 0; i < size; i++) {
            if (arr[i].age >= groupMin[g] && arr[i].age <= groupMax[g]) {

                double monthly = arr[i].carbonEmission * arr[i].dailyDistance * arr[i].daysPerMonth;
                groupTotalEmit += monthly;

                int found = -1;
                for (int m = 0; m < numModes; m++) {
                    if (modeList[m] == arr[i].transportMode) {
                        found = m;
                        break;
                    }
                }

                if (found == -1 && numModes < 10) {
                    modeList[numModes]  = arr[i].transportMode;
                    modeCount[numModes] = 1;
                    modeEmit[numModes]  = monthly;
                    numModes++;
                } else if (found != -1) {
                    modeCount[found]++;
                    modeEmit[found] += monthly;
                }
            }
        }

        if (numModes == 0) continue;

        cout << "\nAge Group: " << groupName[g] << endl;
        cout << "----------------------------------------------------------------------------" << endl;
        
        cout << left << setw(22) << "Mode of Transport" 
             << setw(10) << "Count" 
             << setw(26) << "Total Emission (kg CO2)" 
             << setw(20) << "Average per Resident" << endl;

        for (int m = 0; m < numModes; m++) {
            double avg = (modeCount[m] > 0) ? modeEmit[m] / modeCount[m] : 0;
            
            cout << left << setw(22) << modeList[m] 
                 << setw(10) << modeCount[m] 
                 << fixed << setprecision(2) << setw(26) << modeEmit[m] 
                 << fixed << setprecision(2) << setw(20) << avg << endl;
        }

        cout << "----------------------------------------------------------------------------" << endl;
        cout << "Total Emission for Age Group: " << fixed << setprecision(2) << groupTotalEmit << " kg CO2\n" << endl;
        
        cout.unsetf(ios_base::fixed); 
    }
}

// ============================================================
// USER CAN SELECT DATABASE HERE
// ============================================================
int selectCity() {
    int c;
    cout << "\n  1. City A" << endl;
    cout << "  2. City B" << endl;
    cout << "  3. City C" << endl;
    cout << "Choose city (1-3): ";
    cin >> c;
    return c;
}

// ============================================================
// MAIN MENU
// ============================================================
int main() {
    loadData("dataset1-cityA.csv", cityA, sizeA);
    loadData("dataset2-cityB.csv", cityB, sizeB);
    loadData("dataset3-cityC.csv", cityC, sizeC);

    int choice = 0;

    do {
        cout << "\n=====================================" << endl;
        cout << "   TRANSPORT DATABASE - ARRAY        " << endl;
        cout << "=====================================" << endl;
        cout << "  1. Search Database (Linear Search)" << endl;
        cout << "  2. Sort Database (Merge Sort)" << endl;
        cout << "  3. Carbon Emission Analysis" << endl;
        cout << "  4. Exit" << endl;
        cout << "=====================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int searchChoice;
            cout << "\n=====================================" << endl;
            cout << "       SELECT SEARCH CRITERIA        " << endl;
            cout << "=====================================" << endl;
            cout << "  1. Search by Age" << endl;
            cout << "  2. Search by Transport Mode" << endl;
            cout << "  3. Search by Distance Range" << endl;
            cout << "=====================================" << endl;
            cout << "Enter search choice (1-3): ";
            cin >> searchChoice;

            if (searchChoice >= 1 && searchChoice <= 3) {
                int age = 0;
                string mode = "";
                double minD = 0, maxD = 0;
                bool validSearch = true;

                if (searchChoice == 1) {
                    cout << "Enter age to search: ";
                    cin >> age;
                } 
                else if (searchChoice == 2) {
                    cout << "Enter transport mode (Car/Bus/Bicycle/Walking): ";
                    cin >> mode;
                } 
                else if (searchChoice == 3) {
                    int range;
                    cout << "\n  1. Short  (0-10 km)\n  2. Medium (11-20 km)\n  3. Long   (21+ km)\nChoose range: ";
                    cin >> range;
                    
                    if      (range == 1) { minD = 0;  maxD = 10; }
                    else if (range == 2) { minD = 11; maxD = 20; }
                    else if (range == 3) { minD = 21; maxD = 9999; }
                    else {
                        cout << "Invalid range selection!" << endl;
                        validSearch = false; 
                    }
                }

                if (validSearch) {
                    int city = selectCity();

                    if (searchChoice == 1) {
                        if      (city == 1) searchByAge(cityA, sizeA, "City A", age);
                        else if (city == 2) searchByAge(cityB, sizeB, "City B", age);
                        else if (city == 3) searchByAge(cityC, sizeC, "City C", age);
                    } 
                    else if (searchChoice == 2) {
                        if      (city == 1) searchByTransport(cityA, sizeA, "City A", mode);
                        else if (city == 2) searchByTransport(cityB, sizeB, "City B", mode);
                        else if (city == 3) searchByTransport(cityC, sizeC, "City C", mode);
                    } 
                    else if (searchChoice == 3) {
                        if      (city == 1) searchByDistance(cityA, sizeA, "City A", minD, maxD);
                        else if (city == 2) searchByDistance(cityB, sizeB, "City B", minD, maxD);
                        else if (city == 3) searchByDistance(cityC, sizeC, "City C", minD, maxD);
                    }
                }

            } else {
                cout << "Invalid search choice! Returning to main menu." << endl;
            }
        }
        else if (choice == 2) {
            int sortChoice;
            cout << "\n=====================================" << endl;
            cout << "       SELECT SORTING CRITERIA       " << endl;
            cout << "=====================================" << endl;
            cout << "  1. Sort by Age" << endl;
            cout << "  2. Sort by Transport Mode (A-Z)" << endl;
            cout << "  3. Sort by Daily Distance" << endl;
            cout << "=====================================" << endl;
            cout << "Enter sort choice (1-3): ";
            cin >> sortChoice;

            if (sortChoice >= 1 && sortChoice <= 3) {
                int city = selectCity();

                if      (city == 1) displaySorted(cityA, sizeA, "City A", sortChoice);
                else if (city == 2) displaySorted(cityB, sizeB, "City B", sortChoice);
                else if (city == 3) displaySorted(cityC, sizeC, "City C", sortChoice);
            } else {
                cout << "Invalid sorting choice! Returning to main menu." << endl;
            }
        }
        else if (choice == 3) {
            int city = selectCity();

            if      (city == 1) emissionAnalysis(cityA, sizeA, "City A");
            else if (city == 2) emissionAnalysis(cityB, sizeB, "City B");
            else if (city == 3) emissionAnalysis(cityC, sizeC, "City C");
        }
        else if (choice == 4) {
            cout << "Exiting system. Goodbye!" << endl;
        }
        else {
            cout << "Invalid choice." << endl;
        }

    } while (choice != 4);

    return 0;
}
