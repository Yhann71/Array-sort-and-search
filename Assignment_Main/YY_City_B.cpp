#include "YY_City_B.hpp"
#include "Utilities.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <map>

// ============================================================
//  NodeB  –  Public Constructor
// ============================================================
NodeB::NodeB(string RID, int age, string transport,
           int distance, double carbon, int avgDay)
    : ResidentID(RID), Age(age), ModeOfTransport(transport),
      DailyDistance(distance), CarbonEmissionFactor(carbon),
      AverageDayPerMonth(avgDay), next(nullptr) {}

// ============================================================
//  SinglyLinkedListB  –  Public Constructor & Destructor
// ============================================================
SinglyLinkedListB::SinglyLinkedListB()
    : head(nullptr), sorted(nullptr), size(0) {}

SinglyLinkedListB::~SinglyLinkedListB() {
    clear();
}

// ============================================================
//  Private Helper – append a NodeB to the tail
// ============================================================
void SinglyLinkedListB::append(string RID, int age, string transport,
                               int distance, double carbon, int avgDay) {
    NodeB* newNodeB = new NodeB(RID, age, transport, distance, carbon, avgDay);
    if (!head) {
        head = newNodeB;
    } else {
        NodeB* curr = head;
        while (curr->next) curr = curr->next;
        curr->next = newNodeB;
    }
    size++;
}

// ============================================================
//  File Operations
// ============================================================
bool SinglyLinkedListB::readFromCSV() {
    const string filename = "dataset2-cityB.csv";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "[Error] Cannot open " << filename << "\n";
        return false;
    }

    string line;
    getline(file, line);                // skip header row

    while (getline(file, line)) {
        stringstream ss(line);
        string id, age, mode, dist, factor, avgDay;
        getline(ss, id,     ',');
        getline(ss, age,    ',');
        getline(ss, mode,   ',');
        getline(ss, dist,   ',');
        getline(ss, factor, ',');
        getline(ss, avgDay, ',');
        if (id.empty()) continue;
        append(id, stoi(age), mode, stoi(dist), stod(factor), stoi(avgDay));
    }
    file.close();
    cout << "[Info] Loaded " << size << " records from dataset2-cityB.csv\n";
    return true;
}

void SinglyLinkedListB::reload() {
    clear();
    readFromCSV();
}

void SinglyLinkedListB::clear() {
    NodeB* curr = head;
    while (curr) {
        NodeB* temp = curr;
        curr = curr->next;
        delete temp;
    }
    head   = nullptr;
    sorted = nullptr;
    size   = 0;
}

// ============================================================
//  Getter
// ============================================================
int SinglyLinkedListB::getSize() const { return size; }

// ============================================================
//  Display – full dataset table
// ============================================================
void SinglyLinkedListB::displayList() {
    if (!head) { cout << "List is empty.\n"; return; }

    cout << "\n" << string(95, '=') << "\n";
    cout << left
         << setw(14) << "ResidentID"
         << setw(8)  << "Age"
         << setw(20) << "Transport"
         << setw(18) << "Daily Dist (km)"
         << setw(18) << "Emission Factor"
         << "Days/Month\n";
    cout << string(95, '-') << "\n";

    NodeB* curr = head;
    while (curr) {
        cout << left
             << setw(14) << curr->ResidentID
             << setw(8)  << curr->Age
             << setw(20) << curr->ModeOfTransport
             << setw(18) << curr->DailyDistance
             << setw(18) << fixed << setprecision(2) << curr->CarbonEmissionFactor
             << curr->AverageDayPerMonth << "\n";
        curr = curr->next;
    }
    cout << string(95, '=') << "\n";
    cout << "Total Records : " << size << "\n";
}

// ============================================================
//  Display – sorted results table
// ============================================================
void SinglyLinkedListB::displaySorted(const string& key) {
    cout << "\nSorted by: " << key << "\n";
    cout << string(100, '=') << "\n";
    cout << left
         << setw(14) << "ResidentID"
         << setw(8)  << "Age"
         << setw(20) << "Transport"
         << setw(18) << "Daily Dist (km)"
         << setw(22) << "Monthly CO2 (kg)"
         << "Days/Month\n";
    cout << string(100, '-') << "\n";

    NodeB* curr = head;
    while (curr) {
        double emission = curr->DailyDistance
                        * curr->CarbonEmissionFactor
                        * curr->AverageDayPerMonth;
        cout << left
             << setw(14) << curr->ResidentID
             << setw(8)  << curr->Age
             << setw(20) << curr->ModeOfTransport
             << setw(18) << curr->DailyDistance
             << setw(22) << fixed << setprecision(2) << emission
             << curr->AverageDayPerMonth << "\n";
        curr = curr->next;
    }
    cout << string(100, '=') << "\n";
    cout << "Total Records : " << size << "\n";
}

// ============================================================
//  Comparator  (used by sort algorithms)
// ============================================================
bool SinglyLinkedListB::compare(NodeB* a, NodeB* b, const string& key) {
    if (key == "Age")
        return a->Age < b->Age;
    if (key == "DailyDistance")
        return a->DailyDistance < b->DailyDistance;
    if (key == "CarbonEmission") {
        double ea = a->DailyDistance * a->CarbonEmissionFactor * a->AverageDayPerMonth;
        double eb = b->DailyDistance * b->CarbonEmissionFactor * b->AverageDayPerMonth;
        return ea < eb;
    }
    if (key == "Mode")
        return normalize(a->ModeOfTransport) < normalize(b->ModeOfTransport);
    return normalize(a->ResidentID) < normalize(b->ResidentID);
}

// ============================================================
//  Insertion Sort
// ============================================================
void SinglyLinkedListB::sortedInsert(NodeB* newNodeB, const string& key) {
    if (!sorted || !compare(sorted, newNodeB, key)) {
        newNodeB->next = sorted;
        sorted = newNodeB;
    } else {
        NodeB* curr = sorted;
        while (curr->next && compare(curr->next, newNodeB, key))
            curr = curr->next;
        newNodeB->next = curr->next;
        curr->next    = newNodeB;
    }
}

void SinglyLinkedListB::insertionSort(const string& key) {
    sorted = nullptr;
    NodeB* curr = head;
    while (curr) {
        NodeB* next = curr->next;
        sortedInsert(curr, key);
        curr = next;
    }
    head = sorted;
}

// ============================================================
//  Merge Sort (public wrapper + private recursive core)
// ============================================================
NodeB* SinglyLinkedListB::getMiddle(NodeB* start, NodeB* last) {
    if (!start) return nullptr;
    NodeB* slow = start;
    NodeB* fast = start->next;
    while (fast != last && fast && fast->next != last) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

NodeB* SinglyLinkedListB::merge(NodeB* a, NodeB* b, const string& key) {
    if (!a) return b;
    if (!b) return a;
    NodeB* result;
    if (compare(a, b, key)) {
        result       = a;
        result->next = merge(a->next, b, key);
    } else {
        result       = b;
        result->next = merge(a, b->next, key);
    }
    return result;
}

NodeB* SinglyLinkedListB::mergeSort(NodeB* h, const string& key) {
    if (!h || !h->next) return h;
    NodeB* mid       = getMiddle(h);
    NodeB* nextToMid = mid->next;
    mid->next       = nullptr;
    return merge(mergeSort(h, key), mergeSort(nextToMid, key), key);
}

// Public wrapper – called from the .hpp interface
void SinglyLinkedListB::mergeSort(const string& key) {
    head = mergeSort(head, key);
}

// ============================================================
//  Sorting Experiment  (Age / Daily Distance only – no carbon)
// ============================================================
void SinglyLinkedListB::sortingExperiment() {
    int algoChoice = 0, keyChoice = 0;

    // --- Choose algorithm ---
    while (true) {
        cout << "\nChoose Sorting Algorithm:\n";
        cout << "  1. Insertion Sort\n";
        cout << "  2. Merge Sort\n";
        cout << "Choice: ";
        if (!(cin >> algoChoice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Invalid input – please enter a number.\n";
            continue;
        }
        if (algoChoice < 1 || algoChoice > 2) {
            cout << "[Error] Please enter 1 or 2.\n";
            continue;
        }
        break;
    }

    // --- Choose sort key (carbon emission removed – it is now option 5) ---
    while (true) {
        cout << "Choose Sort Key:\n";
        cout << "  1. Age\n";
        cout << "  2. Daily Distance\n";
        cout << "Choice: ";
        if (!(cin >> keyChoice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Invalid input – please enter a number.\n";
            continue;
        }
        if (keyChoice < 1 || keyChoice > 2) {
            cout << "[Error] Please enter 1 or 2.\n";
            continue;
        }
        break;
    }

    string key = (keyChoice == 1) ? "Age" : "DailyDistance";

    // --- Reload so we always sort the original dataset ---
    reload();

    auto t0 = chrono::high_resolution_clock::now();
    if (algoChoice == 1)
        insertionSort(key);
    else
        head = mergeSort(head, key);
    auto t1 = chrono::high_resolution_clock::now();

    displaySorted(key);

    double ms = chrono::duration<double, milli>(t1 - t0).count();
    string algoName = (algoChoice == 1) ? "Insertion Sort" : "Merge Sort";
    cout << "\n" << algoName << " on '" << key
         << "' completed in " << fixed << setprecision(4) << ms << " ms\n";
}

// ============================================================
//  Linear Search
// ============================================================
void SinglyLinkedListB::linearSearch(const string& key,
                                     int minVal, int maxVal,
                                     string strTarget) {
    int   matches = 0;
    NodeB* curr    = head;

    cout << "\n[Linear Search  |  Key: " << key << "]\n";
    cout << string(100, '=') << "\n";
    cout << left
         << setw(14) << "ResidentID"
         << setw(8)  << "Age"
         << setw(20) << "Transport"
         << setw(18) << "Daily Dist (km)"
         << setw(22) << "Monthly CO2 (kg)"
         << "Days/Month\n";
    cout << string(100, '-') << "\n";

    while (curr) {
        bool ok = false;
        if      (key == "Age")      ok = (curr->Age >= minVal && curr->Age <= maxVal);
        else if (key == "Mode")     ok = (normalize(curr->ModeOfTransport) == normalize(strTarget));
        else if (key == "Distance") ok = (curr->DailyDistance >= minVal);

        if (ok) {
            double e = curr->DailyDistance
                     * curr->CarbonEmissionFactor
                     * curr->AverageDayPerMonth;
            cout << left
                 << setw(14) << curr->ResidentID
                 << setw(8)  << curr->Age
                 << setw(20) << curr->ModeOfTransport
                 << setw(18) << curr->DailyDistance
                 << setw(22) << fixed << setprecision(2) << e
                 << curr->AverageDayPerMonth << "\n";
            matches++;
        }
        curr = curr->next;
    }
    if (matches == 0) cout << "  [No records found]\n";
    cout << string(100, '=') << "\n";
    cout << "Total matches : " << matches << "\n";
}

// ============================================================
//  Binary Search  (list must be sorted before calling)
// ============================================================
void SinglyLinkedListB::binarySearch(const string& key,
                                     int minVal, int maxVal,
                                     string strTarget) {
    int   matches = 0;
    bool  found   = false;

    // Step 1 – binary probe to confirm at least one match exists
    NodeB* start = head;
    NodeB* last  = nullptr;
    while (start != last) {
        NodeB* mid = getMiddle(start, last);
        if (!mid) break;

        bool ok = false;
        if      (key == "Age")      ok = (mid->Age >= minVal && mid->Age <= maxVal);
        else if (key == "Mode")     ok = (normalize(mid->ModeOfTransport) == normalize(strTarget));
        else if (key == "Distance") ok = (mid->DailyDistance >= minVal);   // ✅ inclusive now

        if (ok) { found = true; break; }

        // Narrow search window
        if ((key == "Age"      && mid->Age            <  minVal)  ||
            (key == "Distance" && mid->DailyDistance  <  minVal)  ||   // ✅ changed <= to <
            (key == "Mode"     && normalize(mid->ModeOfTransport) < normalize(strTarget)))
            start = mid->next;
        else
            last = mid;
    }

    cout << "\n[Binary Search  |  Key: " << key << "]\n";
    cout << string(100, '=') << "\n";
    cout << left
         << setw(14) << "ResidentID"
         << setw(8)  << "Age"
         << setw(20) << "Transport"
         << setw(18) << "Daily Dist (km)"
         << setw(22) << "Monthly CO2 (kg)"
         << "Days/Month\n";
    cout << string(100, '-') << "\n";

    // Step 2 – collect all matches from sorted list
    if (found) {
        NodeB* curr = head;
        while (curr) {
            bool ok = false;
            if      (key == "Age")      ok = (curr->Age >= minVal && curr->Age <= maxVal);
            else if (key == "Mode")     ok = (normalize(curr->ModeOfTransport) == normalize(strTarget));
            else if (key == "Distance") ok = (curr->DailyDistance >= minVal);   // ✅ inclusive now

            if (ok) {
                double e = curr->DailyDistance
                         * curr->CarbonEmissionFactor
                         * curr->AverageDayPerMonth;
                cout << left
                     << setw(14) << curr->ResidentID
                     << setw(8)  << curr->Age
                     << setw(20) << curr->ModeOfTransport
                     << setw(18) << curr->DailyDistance
                     << setw(22) << fixed << setprecision(2) << e
                     << curr->AverageDayPerMonth << "\n";
                matches++;
            }
            curr = curr->next;
        }
    } else {
        cout << "  [No records found]\n";
    }
    cout << string(100, '=') << "\n";
    cout << "Total matches : " << matches << "\n";
}

// ============================================================
//  Searching Experiment
// ============================================================
void SinglyLinkedListB::searchingExperiment() {
    int algoChoice = 0, keyChoice = 0;

    // --- Algorithm choice ---
    while (true) {
        cout << "\nChoose Search Algorithm:\n";
        cout << "  1. Linear Search\n";
        cout << "  2. Binary Search\n";
        cout << "Choice: ";
        if (!(cin >> algoChoice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Invalid input – please enter a number.\n";
            continue;
        }
        if (algoChoice < 1 || algoChoice > 2) {
            cout << "[Error] Please enter 1 or 2.\n";
            continue;
        }
        break;
    }

    // --- Search criteria ---
    while (true) {
        cout << "Search Criteria:\n";
        cout << "  1. Age Range\n";
        cout << "  2. Mode of Transport\n";
        cout << "  3. Daily Distance Threshold\n";
        cout << "Choice: ";
        if (!(cin >> keyChoice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Invalid input – please enter a number.\n";
            continue;
        }
        if (keyChoice < 1 || keyChoice > 3) {
            cout << "[Error] Please enter 1, 2 or 3.\n";
            continue;
        }
        break;
    }

    int    minVal = 0, maxVal = 0;
    string strTarget;

    if (keyChoice == 1) {
        while (true) {
            cout << "Enter Minimum Age: ";
            if (!(cin >> minVal)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "[Error] Please enter a valid number.\n";
                continue;
            }
            break;
        }
        while (true) {
            cout << "Enter Maximum Age: ";
            if (!(cin >> maxVal)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "[Error] Please enter a valid number.\n";
                continue;
            }
            break;
        }
    } else if (keyChoice == 2) {
    cout << "Enter Mode of Transport\n"
         << "  (Car | Bus | Bicycle | Walking | School Bus | Carpool): ";
    cin.ignore();
    getline(cin, strTarget);

    } else {
        while (true) {
            cout << "Enter Minimum Daily Distance (km): ";
            if (!(cin >> minVal)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "[Error] Please enter a valid number.\n";
                continue;
            }
            break;
        }
    }

    // For binary search the list must be sorted on the same key we are searching.
    // Reload first so we always search the full original dataset.
    if (algoChoice == 2) {
        reload();
        if      (keyChoice == 1) head = mergeSort(head, "Age");
        else if (keyChoice == 2) head = mergeSort(head, "Mode");         // sort by transport string
        else                     head = mergeSort(head, "DailyDistance");
    }

    auto t0 = chrono::high_resolution_clock::now();
    if (algoChoice == 1) {
        if      (keyChoice == 1) linearSearch("Age",      minVal, maxVal);
        else if (keyChoice == 2) linearSearch("Mode",     0, 0, strTarget);
        else                     linearSearch("Distance", minVal);
    } else {
        if      (keyChoice == 1) binarySearch("Age",      minVal, maxVal);
        else if (keyChoice == 2) binarySearch("Mode",     0, 0, strTarget);
        else                     binarySearch("Distance", minVal);
    }
    auto t1 = chrono::high_resolution_clock::now();

    double ms = chrono::duration<double, milli>(t1 - t0).count();
    string algoName = (algoChoice == 1) ? "Linear Search" : "Binary Search";
    cout << algoName << " completed in "
         << fixed << setprecision(4) << ms << " ms\n";
}

// ============================================================
//  Private helper – prints one age-group emission table
//  (Task 5 sample format)
// ============================================================
void SinglyLinkedListB::printEmissionTable(int minAge, int maxAge,
                                           const string& groupName) {
    // Transport modes for City B (all types)
    const string modes[] = { "Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool" };
    const int    NUM_MODES = 6;

    int    modeCount[NUM_MODES]         = {};
    double modeTotalEmission[NUM_MODES] = {};

    int    groupTotal  = 0;
    double totalGroupE = 0.0;

    NodeB* curr = head;
    while (curr) {
        if (curr->Age >= minAge && curr->Age <= maxAge) {
            groupTotal++;
            double e = curr->DailyDistance
                     * curr->CarbonEmissionFactor
                     * curr->AverageDayPerMonth;
            totalGroupE += e;

            string norm = normalize(curr->ModeOfTransport);
            for (int i = 0; i < NUM_MODES; i++) {
                if (norm == normalize(modes[i])) {
                    modeCount[i]++;
                    modeTotalEmission[i] += e;
                    break;
                }
            }
        }
        curr = curr->next;
    }

    // Print table
    cout << "\nAge Group: " << groupName << "\n";
    cout << string(70, '-') << "\n";
    cout << left
         << setw(20) << "Mode of Transport"
         << setw(10) << "Count"
         << setw(24) << "Total Emission (kg CO2)"
         << "Average per Resident\n";
    cout << string(70, '-') << "\n";

    for (int i = 0; i < NUM_MODES; i++) {
        double avg = (modeCount[i] > 0)
                   ? modeTotalEmission[i] / modeCount[i]
                   : 0.0;
        cout << left
             << setw(20) << modes[i]
             << setw(10) << modeCount[i]
             << setw(24) << fixed << setprecision(2) << modeTotalEmission[i]
             << fixed    << setprecision(2) << avg << "\n";
    }

    cout << string(70, '-') << "\n";
    cout << "Total Emission for Age Group : "
         << fixed << setprecision(2) << totalGroupE << " kg CO2\n";

    double avgPerResident = (groupTotal > 0) ? totalGroupE / groupTotal : 0.0;
    cout << "Average Emission per Resident: "
         << fixed << setprecision(2) << avgPerResident << " kg CO2\n";
    cout << "Total Residents in Group     : " << groupTotal << "\n";
}

// ============================================================
//  Age Group Categorisation  (Task 4)
//  Shows the number of residents in each City B age group only.
//  City B age groups: 6-17 and 18-25
// ============================================================
void SinglyLinkedListB::ageGroupCategory() {
    while (true) {
        int choice = 0;
        cout << "\n--- Age Group Categorisation (City B) ---\n";
        cout << "  1. 6  - 17  : Children & Teenagers\n";
        cout << "  2. 18 - 25  : University Students / Young Adults\n";
        cout << "  3. Total People (Both Age Groups)\n";
        cout << "  4. Back to Menu\n";
        cout << "Enter choice (1-4): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Invalid input - please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            int count = 0;
            NodeB* curr = head;
            while (curr) {
                if (curr->Age >= 6 && curr->Age <= 17) count++;
                curr = curr->next;
            }
            cout << "\n";
            cout << string(45, '-') << "\n";
            cout << "Age Group : 6-17 (Children & Teenagers)\n";
            cout << string(45, '-') << "\n";
            cout << "Number of Residents : " << count << "\n";
            cout << string(45, '-') << "\n";

        } else if (choice == 2) {
            int count = 0;
            NodeB* curr = head;
            while (curr) {
                if (curr->Age >= 18 && curr->Age <= 25) count++;
                curr = curr->next;
            }
            cout << "\n";
            cout << string(50, '-') << "\n";
            cout << "Age Group : 18-25 (University Students / Young Adults)\n";
            cout << string(50, '-') << "\n";
            cout << "Number of Residents : " << count << "\n";
            cout << string(50, '-') << "\n";

        } else if (choice == 3) {
            int count6to17 = 0, count18to25 = 0;
            NodeB* curr = head;
            while (curr) {
                if      (curr->Age >= 6  && curr->Age <= 17) count6to17++;
                else if (curr->Age >= 18 && curr->Age <= 25) count18to25++;
                curr = curr->next;
            }
            int total = count6to17 + count18to25;
            cout << "\n";
            cout << string(50, '=') << "\n";
            cout << "  Total People - City B (All Age Groups)\n";
            cout << string(50, '=') << "\n";
            cout << left << setw(40) << "  6-17  (Children & Teenagers)"
                 << count6to17 << "\n";
            cout << left << setw(40) << "  18-25 (University Students / Young Adults)"
                 << count18to25 << "\n";
            cout << string(50, '-') << "\n";
            cout << left << setw(40) << "  TOTAL" << total << "\n";
            cout << string(50, '=') << "\n";

        } else if (choice == 4) {
            break;
        } else {
            cout << "[Error] Please enter 1, 2, 3 or 4.\n";
        }
    }
}

// ============================================================
//  Carbon Emission Analysis  (Task 5 - standalone menu option 5)
// ============================================================
void SinglyLinkedListB::carbonEmissionAnalysis() {
    while (true) {
        int choice = 0;
        cout << "\n====== Carbon Emission Analysis - City B ======\n";
        cout << "  1. 6  - 17  : Children & Teenagers\n";
        cout << "  2. 18 - 25  : University Students / Young Adults\n";
        cout << "  3. Total People Average (All Age Groups)\n";
        cout << "  4. Back to Menu\n";
        cout << "Enter choice (1-4): ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Invalid input - please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            printEmissionTable(6, 17, "6-17  (Children & Teenagers)");

        } else if (choice == 2) {
            printEmissionTable(18, 25, "18-25 (University Students / Young Adults)");

        } else if (choice == 3) {
            // Calculate total emission and average across all residents
            int totalResidents = 0;
            double totalEmission = 0.0;
            NodeB* curr = head;
            while (curr) {
                double e = curr->DailyDistance
                         * curr->CarbonEmissionFactor
                         * curr->AverageDayPerMonth;
                totalEmission += e;
                totalResidents++;
                curr = curr->next;
            }
            double avgPerResident = (totalResidents > 0)
                                  ? totalEmission / totalResidents
                                  : 0.0;

            cout << "\n" << string(70, '=') << "\n";
            cout << "TOTAL Carbon Emission - City B : "
                 << fixed << setprecision(2) << totalEmission << " kg CO2\n";
            cout << "Average Emission per Resident : "
                 << fixed << setprecision(2) << avgPerResident << " kg CO2\n";
            cout << "Total Residents in Dataset     : " << totalResidents << "\n";
            cout << string(70, '=') << "\n";

        } else if (choice == 4) {
            break;
        } else {
            cout << "[Error] Please enter 1, 2, 3 or 4.\n";
        }
    }
}

