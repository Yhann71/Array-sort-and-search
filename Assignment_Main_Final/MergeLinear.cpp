#include "MergeLinear.hpp" // Pulls in all libraries, structs, and declarations!

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
            conditionMet = (L[i].dailyDistance <= R[j].dailyDistance);
        } 
        else if (sortBy == 3) {
            conditionMet = (L[i].carbonEmission <= R[j].carbonEmission);
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
// LINEAR SEARCH - by age (Updated for Range)
// ============================================================
void searchByAge(Resident arr[], int size, string cityName, int minAge, int maxAge) {
    int count = 0;
    auto start = chrono::high_resolution_clock::now();

    if (minAge == maxAge) {
        cout << "\n--- Search by Age = " << minAge << " in " << cityName << " ---" << endl;
    } else {
        cout << "\n--- Search by Age (" << minAge << " - " << maxAge << ") in " << cityName << " ---" << endl;
    }
    
    cout << left << setw(8)  << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
         << setw(12) << "Dist(km)" << setw(8) << "CO2" << setw(6) << "Days" << endl;
    cout << "-------------------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        if (arr[i].age >= minAge && arr[i].age <= maxAge) {
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
    cout << left << setw(18) << "Algorithm" << ": Linear Search" << endl;
    
    cout << left << setw(18) << "Search by" << ": Age ";
    if (minAge == maxAge) cout << "(" << minAge << ")" << endl;
    else cout << "(" << minAge << "-" << maxAge << ")" << endl;

    cout << left << setw(18) << "Execution Time" << ": " << fixed << setprecision(4) << (duration / 1000.0) << " ms" << endl;
    cout.unsetf(ios_base::fixed);
    cout << left << setw(18) << "Memory usage" << ": " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
    cout << left << setw(18) << "Time Complexity" << ": O(n)" << endl;
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
    cout << left << setw(18) << "Algorithm" << ": Linear Search" << endl;
    cout << left << setw(18) << "Search by" << ": Transport Mode" << endl;
    cout << left << setw(18) << "Execution Time" << ": " << fixed << setprecision(4) << (duration / 1000.0) << " ms" << endl;
    cout.unsetf(ios_base::fixed);
    cout << left << setw(18) << "Memory usage" << ": " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
    cout << left << setw(18) << "Time Complexity" << ": O(n)" << endl;
}

// ============================================================
// LINEAR SEARCH - by distance (Handles Range & Thresholds)
// ============================================================
void searchByDistance(Resident arr[], int size, string cityName, double minD, double maxD) {
    int count = 0;
    auto start = chrono::high_resolution_clock::now();

    cout << "\n--- Search by Distance (" << minD << "km - ";
    if (maxD >= 99999.0) cout << "MAX";
    else cout << maxD;
    cout << "km) in " << cityName << " ---" << endl;
    
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
    cout << left << setw(18) << "Algorithm" << ": Linear Search" << endl;
    cout << left << setw(18) << "Search by" << ": Daily Distance" << endl;
    cout << left << setw(18) << "Execution Time" << ": " << fixed << setprecision(4) << (duration / 1000.0) << " ms" << endl;
    cout.unsetf(ios_base::fixed);
    cout << left << setw(18) << "Memory usage" << ": " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
    cout << left << setw(18) << "Time Complexity" << ": O(n)" << endl;
}

// ============================================================
// LINEAR SEARCH - by carbon emission
// ============================================================
void searchByCarbonEmission(Resident arr[], int size, string cityName, double targetCarbon) {
    int count = 0;
    auto start = chrono::high_resolution_clock::now();

    cout << "\n--- Search by Carbon Emission = " << targetCarbon << " in " << cityName << " ---" << endl;
    cout << left << setw(8)  << "ID" << setw(6) << "Age" << setw(15) << "Transport" 
         << setw(12) << "Dist(km)" << setw(8) << "CO2" << setw(6) << "Days" << endl;
    cout << "-------------------------------------------------------" << endl;

    for (int i = 0; i < size; i++) {
        if (arr[i].carbonEmission == targetCarbon) {
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
    cout << left << setw(18) << "Algorithm" << ": Linear Search" << endl;
    cout << left << setw(18) << "Search by" << ": Carbon Emission" << endl;
    cout << left << setw(18) << "Execution Time" << ": " << fixed << setprecision(4) << (duration / 1000.0) << " ms" << endl;
    cout.unsetf(ios_base::fixed);
    cout << left << setw(18) << "Memory usage" << ": " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
    cout << left << setw(18) << "Time Complexity" << ": O(n)" << endl;
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
    else if (sortChoice == 2) sortName = "Daily Distance";
    else if (sortChoice == 3) sortName = "Carbon Emission";

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
    cout << left << setw(18) << "Algorithm" << ": Merge Sort" << endl;
    cout << left << setw(18) << "Sort by" << ": " << sortName << endl;
    cout << left << setw(18) << "Execution Time" << ": " << fixed << setprecision(4) << (duration / 1000.0) << " ms" << endl;
    cout.unsetf(ios_base::fixed);
    cout << left << setw(18) << "Memory usage" << ": " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
    cout << left << setw(18) << "Time Complexity" << ": O(n log n)" << endl;
}

// ============================================================
// AGE GROUP CATEGORY DISPLAY
// ============================================================
void emissionAnalysis(Resident arr[], int size, string cityName, int ageChoice) {
    auto start = chrono::high_resolution_clock::now(); 

    string groupName[5] = {
        "6-17: Children & Teenagers",
        "18-25: University Students / Young Adults",
        "26-45: Working Adults (Early Career)",
        "46-60: Working Adults (Late Career)",
        "61-100: Senior Citizens / Retirees"
    };

    int groupMin[5] = {6,  18, 26, 46, 61};
    int groupMax[5] = {17, 25, 45, 60, 100};

    double totalDatasetEmission = 0;
    for (int i = 0; i < size; i++) {
        totalDatasetEmission += (arr[i].carbonEmission * arr[i].dailyDistance * arr[i].daysPerMonth);
    }

    int startG = (ageChoice == 6) ? 0 : ageChoice - 1;
    int endG   = (ageChoice == 6) ? 4 : ageChoice - 1;

    for (int g = startG; g <= endG; g++) {

        string modeList[10];
        int    modeCount[10]  = {0};
        double modeEmit[10]   = {0};
        int    numModes       = 0;
        double groupTotalEmit = 0;
        int    groupTotalResidents = 0;

        for (int i = 0; i < size; i++) {
            if (arr[i].age >= groupMin[g] && arr[i].age <= groupMax[g]) {

                double monthly = arr[i].carbonEmission * arr[i].dailyDistance * arr[i].daysPerMonth;
                groupTotalEmit += monthly;
                groupTotalResidents++;

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

        if (numModes == 0) {
            cout << "\nAge Group: " << groupName[g] << endl;
            cout << "No records found in this age group." << endl;
            continue;
        }

        string mostPreferred = "N/A";
        int maxCount = -1;
        for (int m = 0; m < numModes; m++) {
            if (modeCount[m] > maxCount) {
                maxCount = modeCount[m];
                mostPreferred = modeList[m];
            }
        }

        double groupAvgEmission = (groupTotalResidents > 0) ? (groupTotalEmit / groupTotalResidents) : 0;

        cout << "\n----------------------------------------------------------------------------" << endl;
        cout << "Age Group: " << groupName[g] << endl;
        cout << "----------------------------------------------------------------------------" << endl;
        
        cout << left << setw(20) << "Mode of Transport" 
             << setw(16) << "Resident Count" 
             << setw(26) << "Total Emission (kg CO2)" 
             << "Average per Resident (kg CO2)" << endl;

        for (int m = 0; m < numModes; m++) {
            double avg = (modeCount[m] > 0) ? modeEmit[m] / modeCount[m] : 0;
            
            cout << left << setw(20) << modeList[m] 
                 << setw(16) << modeCount[m] 
                 << fixed << setprecision(1) << setw(26) << modeEmit[m] 
                 << fixed << setprecision(1) << avg << endl;
        }

        cout << "----------------------------------------------------------------------------" << endl;
        cout << "Most Preferred Transport: " << mostPreferred << endl;
        cout << "Total Residents in Group: " << groupTotalResidents << endl;
        cout << "Total Emission in Group: " << fixed << setprecision(1) << groupTotalEmit << " kg CO2" << endl;
        cout << "Average Emission per Resident: " << fixed << setprecision(1) << groupAvgEmission << " kg CO2" << endl;
        cout << "----------------------------------------------------------------------------" << endl;
        cout << "Total Carbon Emission (" << cityName << "): " << fixed << setprecision(1) << totalDatasetEmission << " kg CO2" << endl;
        cout << endl;
        cout.unsetf(ios_base::fixed); 
    }

    auto end = chrono::high_resolution_clock::now(); 
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    long long memoryUsed = size * sizeof(Resident);

    cout << "-------------------------------------------------------" << endl;
    cout << left << setw(18) << "Algorithm" << ": Carbon Emission Analysis" << endl;
    cout << left << setw(18) << "Execution Time" << ": " << fixed << setprecision(4) << (duration / 1000.0) << " ms" << endl;
    cout.unsetf(ios_base::fixed);
    cout << left << setw(18) << "Memory usage" << ": " << memoryUsed << " bytes (Size: " << sizeof(Resident) << " bytes/element)" << endl;
    cout << left << setw(18) << "Time Complexity" << ": O(n)" << endl;
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
int linearMerge() {
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
        cout << "  3. Display Age Group Category" << endl;
        cout << "  4. Exit" << endl;
        cout << "=====================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int searchChoice;
            cout << "\n=====================================" << endl;
            cout << "       SELECT SEARCH CRITERIA        " << endl;
            cout << "=====================================" << endl;
            cout << "  1. Age" << endl;
            cout << "  2. Mode of Transport" << endl;
            cout << "  3. Daily Distance" << endl;
            cout << "  4. Carbon Emission" << endl;
            cout << "  5. Back" << endl;
            cout << "=====================================" << endl;
            cout << "Enter search choice (1-5): ";
            cin >> searchChoice;

            if (searchChoice == 5) {
                continue; 
            }

            if (searchChoice >= 1 && searchChoice <= 4) {
                string ageStr, distStr, modeStr;
                int minAge = 0, maxAge = 0;
                double minD = 0, maxD = 0;
                double carbon = 0;
                bool validSearch = true;

                if (searchChoice == 1) {
                    cout << "Enter age to search (e.g., 26 or 26-45): ";
                    cin >> ageStr;
                    try {
                        size_t dashPos = ageStr.find('-');
                        if (dashPos != string::npos) {
                            minAge = stoi(ageStr.substr(0, dashPos));
                            maxAge = stoi(ageStr.substr(dashPos + 1));
                        } else {
                            minAge = maxAge = stoi(ageStr);
                        }
                    } catch (...) {
                        cout << "Invalid age format!" << endl;
                        validSearch = false;
                    }
                }
                else if (searchChoice == 2) {
                    cout << "Enter transport mode (e.g., Car, Bus, Bicycle, Walking, School Bus, Carpool): ";
                    cin >> ws; // clear whitespace buffer
                    getline(cin, modeStr); // Allows for spaces in input like "School Bus"
                }
                else if (searchChoice == 3) {
                    cout << "Enter daily distance (e.g., 15, >15, <20, 10-20): ";
                    cin >> distStr;
                    try {
                        if (distStr[0] == '>') {
                            minD = stod(distStr.substr(1));
                            maxD = 99999.0;
                        } else if (distStr[0] == '<') {
                            minD = 0.0;
                            maxD = stod(distStr.substr(1));
                        } else {
                            size_t dashPos = distStr.find('-');
                            if (dashPos != string::npos) {
                                minD = stod(distStr.substr(0, dashPos));
                                maxD = stod(distStr.substr(dashPos + 1));
                            } else {
                                minD = maxD = stod(distStr);
                            }
                        }
                    } catch (...) {
                        cout << "Invalid distance format!" << endl;
                        validSearch = false;
                    }
                } 
                else if (searchChoice == 4) {
                    cout << "Enter Carbon Emission factor to search (e.g., 0.21): ";
                    cin >> carbon;
                }

                if (validSearch) {
                    int city = selectCity();

                    if (searchChoice == 1) {
                        if      (city == 1) searchByAge(cityA, sizeA, "City A", minAge, maxAge);
                        else if (city == 2) searchByAge(cityB, sizeB, "City B", minAge, maxAge);
                        else if (city == 3) searchByAge(cityC, sizeC, "City C", minAge, maxAge);
                    } 
                    else if (searchChoice == 2) {
                        if      (city == 1) searchByTransport(cityA, sizeA, "City A", modeStr);
                        else if (city == 2) searchByTransport(cityB, sizeB, "City B", modeStr);
                        else if (city == 3) searchByTransport(cityC, sizeC, "City C", modeStr);
                    }
                    else if (searchChoice == 3) {
                        if      (city == 1) searchByDistance(cityA, sizeA, "City A", minD, maxD);
                        else if (city == 2) searchByDistance(cityB, sizeB, "City B", minD, maxD);
                        else if (city == 3) searchByDistance(cityC, sizeC, "City C", minD, maxD);
                    }
                    else if (searchChoice == 4) {
                        if      (city == 1) searchByCarbonEmission(cityA, sizeA, "City A", carbon);
                        else if (city == 2) searchByCarbonEmission(cityB, sizeB, "City B", carbon);
                        else if (city == 3) searchByCarbonEmission(cityC, sizeC, "City C", carbon);
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
            cout << "  1. Age" << endl;
            cout << "  2. Daily Distance" << endl;
            cout << "  3. Carbon Emission" << endl;
            cout << "  4. Back" << endl;
            cout << "=====================================" << endl;
            cout << "Enter sort choice (1-4): ";
            cin >> sortChoice;

            if (sortChoice == 4) {
                continue; 
            }

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
            int ageChoice;
            cout << "\nSelect Age Group:" << endl;
            cout << "  1. 6-17   : Children & Teenagers" << endl;
            cout << "  2. 18-25  : University Students / Young Adults" << endl;
            cout << "  3. 26-45  : Working Adults (Early Career)" << endl;
            cout << "  4. 46-60  : Working Adults (Late Career)" << endl;
            cout << "  5. 61-100 : Senior Citizens / Retirees" << endl;
            cout << "  6. All Age Group" << endl;
            cout << "  7. Back" << endl;
            cout << "Enter choice (1-7): ";
            cin >> ageChoice;

            if (ageChoice == 7) {
                continue;
            }

            if (ageChoice >= 1 && ageChoice <= 6) {
                int city = selectCity();

                if      (city == 1) emissionAnalysis(cityA, sizeA, "City A", ageChoice);
                else if (city == 2) emissionAnalysis(cityB, sizeB, "City B", ageChoice);
                else if (city == 3) emissionAnalysis(cityC, sizeC, "City C", ageChoice);
            } else {
                cout << "Invalid choice! Returning to main menu." << endl;
            }
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
