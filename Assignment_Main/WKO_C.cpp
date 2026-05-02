#include "WKO_C.hpp"

Node::Node(string RID, int age, string transport,
           int distance, double carbon, int avgDay) {
    ResidentID           = RID;
    Age                  = age;
    ModeOfTransport      = transport;
    DailyDistance        = distance;
    CarbonEmissionFactor = carbon;
    AverageDayPerMonth   = avgDay;
    next                 = nullptr;
}

SinglyLinkedList::SinglyLinkedList() {
    head   = nullptr;
    sorted = nullptr;
    size   = 0;
}

SinglyLinkedList::~SinglyLinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current    = current->next;
        delete temp;
    }
}

    void SinglyLinkedList::append(string RID, int age, string transport, int distance, double carbon, int avgDay) {
        Node* newNode = new Node(RID, age, transport, distance, carbon, avgDay);

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    bool SinglyLinkedList::readFromCSV() {
        const string& filename = "dataset3-cityC.csv";
        ifstream file (filename);
        if (!file.is_open()) {
            cerr << "Error! Cannot open file: " << filename << endl;
            return false;
        }

        string line;
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string id, mode, age, distance, carbon, avgDay;
            
            getline(ss, id, ',');
            getline(ss, age, ',');
            getline(ss, mode, ',');
            getline(ss, distance, ',');
            getline(ss, carbon, ',');
            getline(ss, avgDay, ',');

            if (id.empty() || mode.empty() || age.empty() || distance.empty() || carbon.empty() || avgDay.empty()) {
                continue;
            }

            append (id, 
                    stoi(age),
                    mode,
                    stoi(distance),
                    stod(carbon),
                    stoi(avgDay));
        }

        file.close();
        cout << "\nData is successfully loaded from: " << filename << endl;
        cout << "The data size is: " << size << endl;

        return true;
    }

    void SinglyLinkedList::displayList() {
        if (isEmpty()) {
            cout << "List is empty" << endl;
            return;
        }

        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "ResidentID\tAge\tModeofTransport\t\tDailyDistance(KM)\tCarbonEmissionFactor\tAverageDayPerMonth\n";
        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;

        Node* current = head;
        while (current != nullptr) {
            cout << current->ResidentID <<"\t\t"
                 << current->Age <<"\t"
                 << current->ModeOfTransport <<"\t\t\t"
                 << current->DailyDistance <<"\t\t\t"
                 << current->CarbonEmissionFactor <<"\t\t\t"
                 << current->AverageDayPerMonth << "\n";
            current = current->next;
        }
        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Total records: " << size << "\n";
    }

    bool SinglyLinkedList::isEmpty() { 
        return head == nullptr;
    }

    //==================================Searching Algorithm==========================================================================================
    
    //Linear Search
    void SinglyLinkedList::linearSearch(string targetID) {
        Node* current = head;
        bool found = false;
        cout << "\nLinear Search Result:" << endl;

        while (current != nullptr && !found) {
            if (current->ResidentID == targetID) {
                found = true;
            } else {
                current = current->next;
            }
        }

        if (found) {
            cout << "Resident " << targetID << " found!" << endl;
            displayData(current);
        } else {
            cout << "Resident " << targetID << " not found!" << endl;
        }
    }

    void SinglyLinkedList::displayData(Node* node) {
        if (node == nullptr) {
            return;
        }
        cout << "  ResidentID             : " << node->ResidentID           << endl;
        cout << "  Age                    : " << node->Age                  << endl;
        cout << "  Mode Of Transport      : " << node->ModeOfTransport      << endl;
        cout << "  Daily Distance         : " << node->DailyDistance << " km" << endl;
        cout << "  Carbon Emission Factor : " << fixed << node->CarbonEmissionFactor  << endl;
        cout << "  Average Day/Month      : " << node->AverageDayPerMonth << " days" << endl;
    };

    //Binary Search
    Node* SinglyLinkedList::getMiddle(Node* start, Node* last) {
        if (start == nullptr) {
            return NULL;
        }

        if (start == last) {
            return start;
        }

        Node* slow = start;
        Node* fast = start->next;

        while (fast != last && fast->next != last) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }

    void SinglyLinkedList::binarySearch(string targetID) {
        Node* start = head;
        Node* last = nullptr;

        bool found = false;
        cout << "\nBinary Search Result:" << endl;

        while (start != last) {
            Node* mid = getMiddle(start, last);

            if (mid->ResidentID == targetID) {
                found = true;
                cout << "Resident " << targetID << " found!" << endl;

                displayData(mid);
                break;
            } else if (mid->ResidentID < targetID) {
                start = mid->next;
            } else {
                last = mid;
            }
        
        }

        if (!found) {
            cout << "Resident " << targetID << " not found!" << endl;
        }
    }

    //==================================Sorting Algorithm==========================================================================================

    bool SinglyLinkedList::compareNodes(Node* a, Node* b, string key) {
        if (key == "Age") {
            return a->Age < b->Age;
        } else if (key == "DailyDistance") {
            return a->DailyDistance < b->DailyDistance;
        } else if (key == "CarbonEmission") {
            double emissionA = a->DailyDistance * a->CarbonEmissionFactor * a->AverageDayPerMonth;
            double emissionB = b->DailyDistance * b->CarbonEmissionFactor * b->AverageDayPerMonth;
            return emissionA < emissionB;
        } else if (key == "ModeOfTransport") {
            return a->ModeOfTransport < b->ModeOfTransport;
        } else {
            return a->ResidentID < b->ResidentID;
        }
    }

    // ---------------------------------------------------------------
    //  INSERTION SORT — by sort key
    //  Same pointer re-linking logic as original insertionSort()
    //  but uses compareNodes() instead of comparing ResidentID
    //
    //  Time Complexity  : O(n²) worst/average | O(n) best
    //  Space Complexity : O(1)
    // ---------------------------------------------------------------
    void SinglyLinkedList::sortedInsert(Node* newnode, string sortKey) {
        if (sorted == nullptr || !compareNodes(sorted, newnode, sortKey)) {
            newnode->next = sorted;
            sorted = newnode;
        } else {
            Node* current = sorted;
            while (current->next != nullptr && compareNodes(current->next, newnode, sortKey)) {
                current = current->next;
            }
            newnode->next = current->next;
            current->next = newnode;
        }
    }

    void SinglyLinkedList::insertionSort(string sortKey) {
        sorted = nullptr;
        Node* current = head;
        
        while (current != nullptr) {
            Node* next = current->next;
            sortedInsert(current, sortKey);
            current = next;
        }

        head = sorted;
    }

    // ---------------------------------------------------------------
    //  MERGE SORT — by sort key
    //  Same divide and conquer logic as original mergeSort()
    //  but uses compareNodes() instead of comparing ResidentID
    //
    //  Time Complexity  : O(n log n) always
    //  Space Complexity : O(log n) — recursive call stack
    // ---------------------------------------------------------------
    Node* SinglyLinkedList::merge(Node* left, Node* right, string sortKey) {
        if (left == nullptr) {
            return right;
        }

        if (right == nullptr) {
            return left;
        }

        Node* result = nullptr;

        if (compareNodes(left, right, sortKey)) {
            result = left;
            result->next = merge(left->next, right, sortKey);
        } else {
            result = right;
            result->next = merge(left, right->next, sortKey);
        }
        return result;
    }

    Node* SinglyLinkedList::sort(Node* node, string sortKey) { 
        if (node == nullptr || node->next == nullptr) {
            return node;
        }
    
        Node* mid = getMiddle(node, nullptr);

        Node* left = node;
        Node* right = mid->next;
        mid->next = nullptr;

        left = sort(left, sortKey);
        right = sort(right, sortKey);

        return merge(left, right, sortKey);
    }

    void SinglyLinkedList::mergeSort(string sortKey) {
        head = sort(head, sortKey);
        cout << "\nMerge Sort Result:" << endl;
        cout << "List has been successfully sorted by " << sortKey << endl;
    }

    // ---------------------------------------------------------------
    //  DISPLAY SORTED TABLE
    //  Prints the list after sorting with the sort key highlighted
    // ---------------------------------------------------------------
    void SinglyLinkedList::displaySortedData(string sortKey) {
        cout << left
            << setw(14) << "ResidentID"
            << setw(8) << "Age"
            << setw(20) << "Mode of Transport"
            << setw(18) << "Daily Distance (km)"
            << setw(24) << "Carbon Emission Factor"
            << setw(18) << "Average Day/Month"
            << "\n";
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        
        Node* current = head;
        while (current != nullptr) {
            double emission = current->DailyDistance
                            * current->CarbonEmissionFactor
                            * current->AverageDayPerMonth;
            
            cout << left
                << setw(14) << current->ResidentID
                << setw(8) << current->Age
                << setw(20) << current->ModeOfTransport
                << setw(18) << current->DailyDistance
                << setw(24) << fixed << setprecision(1) << current->CarbonEmissionFactor
                << setw(18) << current->AverageDayPerMonth
                << "\n";
            current = current->next;
        }
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        cout << "Total records: " << size << endl;
    }

    // ---------------------------------------------------------------
    //  RELOAD FROM CSV
    //  Resets the list and reloads fresh unsorted data.
    //  Needed between sorts so each algorithm starts from
    //  the same unsorted state.
    // ---------------------------------------------------------------
    void SinglyLinkedList::reload() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        sorted = nullptr;
        size = 0;

        readFromCSV();
    }
    
    // ---------------------------------------------------------------
    //  SORTING EXPERIMENTS — Section 6
    //  Runs both sorting algorithms on all 3 sort keys,
    //  measures execution time, and prints a comparison table.
    // ---------------------------------------------------------------
    void SinglyLinkedList::sortingExperiment() {
        const int keyNum = 3;
        const string sortKeys[keyNum] = {"Age", "DailyDistance", "CarbonEmission"};
        const string keyLabels[keyNum] = {"Age", "Daily Distance", "Carbon Emission"};

        double insertionDuration[keyNum] = {0.0};
        double mergeDuration[keyNum] = {0.0};

        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Sorting Experiment" << endl;
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;

        int algoChoice = 0;
        while (true) {
            cout << "\nSelect Sorting Algorithm: " << endl;
            cout << "  1. Insertion Sort" << endl;
            cout << "  2. Merge Sort" << endl;
            cout << "  3. Back" << endl;
            cout << "Enter choice (1-3): ";
            cin >> algoChoice;

            if (algoChoice == 3) {
                return;
            }

            if (algoChoice == 1 || algoChoice == 2) {
                break;
            }

            cout << "Invalid choice! Please enter (1-3)." << endl;
        }

        int keyChoice = 0;
        while (true) {
            cout << "\nSelect Sort Key: " << endl;
            cout << "  1. Age" << endl;
            cout << "  2. Daily Distance" << endl;
            cout << "  3. Carbon Emission" << endl;
            cout << "  4. Back" << endl;
            cout << "Enter choice (1-4): ";
            cin >> keyChoice;

            if (keyChoice == 4) {
                sortingExperiment();
                return;
            }

            if (keyChoice >= 1 && keyChoice <= 3) {
                break;
            }
            cout << "Invalid choice! Please enter (1-4)." << endl;
        }

        string selectedKey   = sortKeys[keyChoice - 1];
        string selectedLabel = keyLabels[keyChoice - 1];
        string algoName      = (algoChoice == 1) ? "Insertion Sort" : "Merge Sort";

        reload();
        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << algoName << " sorted by " << selectedLabel << endl;
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;

        double duration = 0.0;

        if (algoChoice == 1 ) {
            auto start = chrono::high_resolution_clock::now();
            insertionSort(selectedKey);
            auto end = chrono::high_resolution_clock::now();
            duration = chrono::duration<double, milli>(end - start).count();
        } else {
            auto start = chrono::high_resolution_clock::now();
            mergeSort(selectedKey);
            auto end = chrono::high_resolution_clock::now();
            duration = chrono::duration<double, milli>(end - start).count();
        }

        displaySortedData(selectedKey);

        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;   
        cout << "Algorithm     : " << algoName      << endl;
        cout << "Sort by       : " << selectedLabel << endl;
        cout << "Execution Time: " << fixed << setprecision(4) << duration << " ms" << endl;
        
        int nodeSize = sizeof(Node);
        long long totalMemory = static_cast<long long>(nodeSize) * size;

        cout << "Memory per Node  : " << nodeSize << " bytes" << endl;
        cout << "Total Data Memory: " << totalMemory << " bytes (" << fixed << setprecision(2) << totalMemory / 1024.0 << " KB)" << endl;

        if (algoChoice == 1) {
            cout << "Time Complexity  : O(n^2) worst case | O(n) best case" << endl;
            cout << "Space Complexity : O(1)" << endl;
        } else {
            cout << "Time Complexity  : O(n log n)" << endl;
            cout << "Space Complexity : O(log n)" << endl;
        }
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;
    }

    // ---------------------------------------------------------------
    //  Linear Search — by search key
    //
    //  Time Complexity  : O(n)
    //  Space Complexity : O(n)
    // ---------------------------------------------------------------
    void SinglyLinkedList::linearSearch(string searchKey, string strTarget, int minInt, int maxInt) {
        Node* current = head;
        int count = 0;
        cout << left
            << setw(14) << "ResidentID"
            << setw(8) << "Age"
            << setw(20) << "Mode of Transport"
            << setw(18) << "Daily Distance (km)"
            << setw(24) << "Carbon Emission Factor"
            << setw(18) << "Average Day/Month"
            << "\n";
        cout << "----------------------------------------------------------------------------------------------------" << endl;

        while (current != nullptr) {
            bool isMatch = false;

            if (searchKey == "Age") {
                isMatch = (current->Age >= minInt && current->Age <= maxInt);
            } else if (searchKey == "ModeOfTransport") {
                isMatch = (current->ModeOfTransport == strTarget);
            } else if (searchKey == "DailyDistance") {
                isMatch = (current->DailyDistance > minInt);
            }
            
            if (isMatch) {
                cout << left
                    << setw(14) << current->ResidentID
                    << setw(8) << current->Age
                    << setw(20) << current->ModeOfTransport
                    << setw(18) << current->DailyDistance
                    << setw(24) << fixed << setprecision(1) << current->CarbonEmissionFactor
                    << setw(18) << current->AverageDayPerMonth
                    << "\n";
                count++;
            }
            current = current->next;
        }
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        cout << "Total matches found: " << count << endl;
    }

    // ---------------------------------------------------------------
    //  Binary Search — by search key
    //  
    //  Time Complexity  : O(n)
    //  Space Complexity : O(1)
    // ---------------------------------------------------------------
    void SinglyLinkedList::binarySearch(string searchKey, string strTarget, int minInt, int maxInt) {
        Node* start = head;
        Node* last = nullptr;
        Node* firstMatch = nullptr;

        while (start != last) {
            Node* mid = getMiddle(start, last);

            if (mid == nullptr) {
                break;
            } 
            bool goLeft = false;            
            
            // Determine if we need to search the left half
            if (searchKey == "Age" && mid->Age >= minInt) {
                goLeft = true;
            } else if (searchKey == "ModeOfTransport" && mid->ModeOfTransport >= strTarget) {
                goLeft = true;
            } else if (searchKey == "DailyDistance" && mid->DailyDistance > minInt) {
                goLeft = true;
            }
            
            if (goLeft) {
                if (searchKey == "ModeOfTransport" && mid->ModeOfTransport != strTarget) {

                } else {
                    firstMatch = mid;
                }
                last = mid; // Keep looking left for earlier occurrences
            } else {
                start = mid->next; // Look right
            }       
        }

        int count = 0;
        cout << left
            << setw(14) << "ResidentID"
            << setw(8) << "Age"
            << setw(20) << "Mode of Transport"
            << setw(18) << "Daily Distance (km)"
            << setw(24) << "Carbon Emission Factor"
            << setw(18) << "Average Day/Month"
            << "\n";
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        Node* current = firstMatch;

        while (current != nullptr) {
            if (searchKey == "Age" && current->Age > maxInt) {
                break;
            }
            if (searchKey == "ModeOfTransport" && current->ModeOfTransport != strTarget) {
                break;
            }
            if (searchKey == "DailyDistance" && current->DailyDistance <= minInt) {
                current = current->next;
                continue;
            }

            cout << left
                << setw(14) << current->ResidentID
                << setw(8) << current->Age
                << setw(20) << current->ModeOfTransport
                << setw(18) << current->DailyDistance
                << setw(24) << fixed << setprecision(1) << current->CarbonEmissionFactor
                << setw(18) << current->AverageDayPerMonth
                << "\n";     
                count++;
                current = current->next;       
        }
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        cout << "Total matches found: " << count << endl;
    }

    // ---------------------------------------------------------------
    //  Searching EXPERIMENTS — Section 7
    //  Runs both searching algorithms on all 3 sort keys,
    //  measures execution time, and prints a comparison table.
    // ---------------------------------------------------------------
    void SinglyLinkedList::searchingExperiment() {
        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Searching Experiment" << endl;
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;

        int algoChoice = 0;
        while (true) {
            cout << "\nSelect Searching Algorithm: " << endl;
            cout << "  1. Linear Search" << endl;
            cout << "  2. Binary Search" << endl;
            cout << "  3. Back" << endl;
            cout << "Enter choice (1-3): ";
            cin >> algoChoice;

            if (algoChoice == 3) {
                return;
            }

            if (algoChoice == 1 || algoChoice == 2) {
                break;
            }
            cout << "Invalid choice! Please enter (1-3)." << endl;
        }

        int keyChoice = 0;
        string searchKey = "";
        
        while (true) {
            cout << "\nSelect Search Criteria: " << endl;
            cout << "  1. Age Group (Range)" << endl;
            cout << "  2. Mode of Transport (Exact Match)" << endl;
            cout << "  3. Daily Distance Threshold (Greater Than)" << endl;
            cout << "  4. Back" << endl;
            cout << "Enter choice (1-4): ";
            cin >> keyChoice;

            if (keyChoice == 4) {
                return;
            }
            if (keyChoice >= 1 && keyChoice <= 3) {
                break;
            }
            cout << "Invalid choice! Please enter (1-4)." << endl;
        }

        int minInt = 0, maxInt = 0; 
        string strTarget = "";

        if (keyChoice == 1) {
            searchKey = "Age";
            cout << "Enter Minimum Age: "; cin >> minInt;
            cout << "Enter Maximum Age: "; cin >> maxInt;
        } else if (keyChoice == 2) {
            searchKey = "ModeOfTransport";
            cout << "Enter Mode of Transport (Car, Bus, Bicycle, Walking, School Bus, Carpool): "; 
            cin.ignore();
            getline(cin, strTarget);
        } else if (keyChoice == 3) {
            searchKey = "DailyDistance";
            cout << "Enter Minimum Daily Distance (km): "; cin >> minInt;
        }

        reload();
        double duration = 0.0;

        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Search Results" << endl;
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;

        if (algoChoice == 1) {
            auto start = chrono::high_resolution_clock::now();
            linearSearch(searchKey, strTarget, minInt, maxInt);
            auto end = chrono::high_resolution_clock::now();
            duration = chrono::duration<double, milli>(end - start).count();
        } else {
            // Sort data first using the existing mergeSort 
            cout << "[System] Sorting data by " << searchKey << " for Binary Search...\n\n";
            mergeSort(searchKey); 

            auto start = chrono::high_resolution_clock::now();
            binarySearch(searchKey, strTarget, minInt, maxInt);
            auto end = chrono::high_resolution_clock::now();
            duration = chrono::duration<double, milli>(end - start).count();
        }

        cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Algorithm     : " << (algoChoice == 1 ? "Linear Search" : "Binary Search") << endl;
        cout << "Execution Time: " << fixed << setprecision(4) << duration << " ms" << endl;
        
        int nodeSize = sizeof(Node);
        long long totalMemory = static_cast<long long>(nodeSize) * size;
        
        cout << "Memory per Node  : " << nodeSize << " bytes" << endl;
        cout << "Total Data Memory: " << totalMemory << " bytes (" << fixed << setprecision(2) << totalMemory / 1024.0 << " KB)" << endl;

        if (algoChoice == 1) {
            cout << "Time Complexity  : O(n) average | O(1) best case" << endl;
            cout << "Space Complexity : O(1)" << endl;
        } else {
            cout << "Time Complexity  : O(n log n)" << endl; 
            cout << "Space Complexity : O(1)" << endl;
        }
        cout << "------------------------------------------------------------------------------------------------------------------" << endl;
    }

    //Section 4: Age Group Categorization
    void SinglyLinkedList::ageGroupCategory() {
        const int groupNum = 5;
        const string groupName[groupNum] = {
            "6-17: Children & Teenagers",
            "18-25: University Students / Young Adults",
            "26-45: Working Adults (Early Career)", 
            "46-60: Working Adults (Late Career)", 
            "61-100: Senior Citizens / Retirees" 
        };

        const int groupMin[groupNum] = {6, 18, 26, 46, 61};
        const int groupMax[groupNum] = {17, 25, 45, 60, 100};

        const int transportNum = 6;
        const string transportName[transportNum] = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};

        int groupCount[groupNum] = {0};
        double groupTotalEmission[groupNum] = {0.0};
        int transportCount[groupNum][transportNum] = {{0}};
        double transportEmission[groupNum][transportNum] = {{0.0}};

        Node* current = head;
        while (current != nullptr) {
            double emission = current->DailyDistance
                            * current->CarbonEmissionFactor
                            * current->AverageDayPerMonth;
            
            int age = -1;
            for (int i = 0; i < groupNum; i++) {
                if (current->Age >= groupMin[i] && current->Age <= groupMax[i]) {
                    age = i;
                    break;
                }
            }

            if (age == -1) {
                current = current->next;
                continue;
            }

            groupCount[age]++;
            groupTotalEmission[age] += emission;

            int transport = -1;
            for (int i = 0; i < transportNum; i++) {
                if (current->ModeOfTransport == transportName[i]) {
                    transport = i;
                    break;
                }
            }

            if (transport != -1) {
                transportCount[age][transport]++;
                transportEmission[age][transport] += emission;
            }

            current = current->next;
        }

        int choice = 0;
        while (true) {
            cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "Age Group Categorization" << endl;
            cout << "------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "\nSelect Age Group: " << endl;
            cout << "  1. 6-17   : Children & Teenagers" << endl;
            cout << "  2. 18-25  : University Students / Young Adults" << endl;
            cout << "  3. 26-45  : Working Adults (Early Career)" << endl;
            cout << "  4. 46-60  : Working Adults (Late Career)" << endl;
            cout << "  5. 61-100 : Senior Citizens / Retirees" << endl;
            cout << "  6. All Age Group" << endl;
            cout << "  7. Back" << endl;
            cout << "Enter choice (1-7): ";
            cin >> choice;

            if (choice == 7) {
                return;
            }

            if (choice < 1 || choice > 7) {
                cout << "Invalid choice! Enter 1 to 7: " << endl;
                continue;
            }

            int startIdx = (choice == 6) ? 0        : choice - 1;
            int endIdx   = (choice == 6) ? groupNum  : choice;

            for (int i = startIdx; i < endIdx; i++) {
                cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
                cout << "Age Group: " << groupName[i] << endl;
                cout << "------------------------------------------------------------------------------------------------------------------" << endl;

                cout << left
                    << setw(20) << "Mode of Transport"
                    << setw(16)  << "Resident Count"
                    << setw(26) << "Total Emission (kg CO2)"
                    << setw(26) << "Average per Resident (kg CO2)"
                    << "\n";

                int mostCount = -1;
                string mostPreferred = "N/A";

                for (int x = 0; x < transportNum; x++) {
                    double avgResident = (transportCount[i][x] > 0) ? transportEmission[i][x] / transportCount[i][x] : 0.0;

                    cout << left
                        << setw(20) << transportName[x]
                        << setw(16)  << transportCount[i][x]
                        << setw(26) << fixed << setprecision(1) << transportEmission[i][x]
                        << setw(26) << avgResident
                        << "\n";

                    if (transportCount[i][x] > mostCount) {
                        mostCount = transportCount[i][x];
                        mostPreferred = transportName[x];
                    }
                }

                cout << "\n" << "------------------------------------------------------------------------------------------------------------------" << endl;
                double avgEmission = (groupCount[i] > 0) ? groupTotalEmission[i] / groupCount[i] : 0.0;

                cout << "Most Preferred Transport: " << mostPreferred << endl;
                cout << "Total Residents in Group: " << groupCount[i] << endl;
                cout << "Total Emission in Group: " << fixed << setprecision(1) << groupTotalEmission[i] << " kg CO2" << endl;
                cout << "Average Emission per Resident: " << fixed << setprecision(1) << avgEmission << " kg CO2" << endl;

                cout << "------------------------------------------------------------------------------------------------------------------" << endl;
                }

                double emissionSetTotal = 0.0;
                for (int i = 0; i < groupNum; i++) {
                    emissionSetTotal += groupTotalEmission[i];
                }
                cout << "Total Carbon Emission (Dataset3-CityC): " << emissionSetTotal <<fixed << setprecision(1) << " kg CO2" << endl;
        }
    } 