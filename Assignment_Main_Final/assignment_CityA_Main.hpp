#include "assignment_CityA_Header.hpp"

int dataset1CityA();

// ----------------------------------------------- [Linked List Operations] ----------------------------------------------- [start]
// Insertion at the End: appends a new node with given data to the end of the linked list
void insertionAtEnd(node*& head, node*& tail, CityA data)
{
    // Constructor automatically sets newNode->data = data and newNode->next = nullptr
    node* newNode = new node(data);

    if (head == nullptr) 
    { 
        head = newNode; tail = newNode; // first node
    } 
    else                 
    { 
        tail->next = newNode; tail = newNode; // link to end
    } 
}

// Deletion of a Node: frees all nodes in a linked list and resets head to nullptr
void deleteNode(node*& head)
{
    node* curr = head;
    while (curr != nullptr)
    {
        node* tmp = curr;
        curr = curr->next;
        delete tmp;         
    }
    head = nullptr;
}

// ----------------------------------------------- [Linked List Operations] ----------------------------------------------- [end]


// ----------------------------------------------- [CSV Loader] ----------------------------------------------- [start]
node* readCSV(const string& filename)
{
    node* head = nullptr;
    node* tail = nullptr;

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Could not open " << filename << "\n";
        return nullptr;
    }

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);

        // Parse all fields from CSV line into a temporary CityA struct
        CityA temp;
        getline(ss, temp.ResidentID,      ',');
        ss >> temp.Age;
        ss.ignore();
        getline(ss, temp.ModeOfTransport, ',');
        ss >> temp.DailyDistance;
        ss.ignore();
        ss >> temp.CarbonEmissionFactor;
        ss.ignore();
        ss >> temp.AverageDaysPerMonth;
        ss.ignore();

        // Constructor sets newNode->data = temp and newNode->next = nullptr automatically
        node* newNode = new node(temp);

        // Append to end of linked list
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    file.close();
    return head;
}

// ----------------------------------------------- [CSV Loader] ----------------------------------------------- [end]


// ----------------------------------------------- [Display Menu] ----------------------------------------------- [start]
// searchByMenu: shared menu for both linear and binary search
// returns 1=Age Range, 2=Mode of Transport, 3=Daily Distance Threshold
int searchByMenu(int& minAge, int& maxAge, string& outTransport, int& outDistance)
{
    int opt = 0;
    cout << "==================\n";
    cout << "Search By  \n";
    cout << "==================\n\n";
    cout << "1. Age Range \n";
    cout << "2. Mode of Transport \n";
    cout << "3. Daily Distance Threshold (> km) \n";
    cout << ">> ";
    cin >> opt;

    switch (opt)
    {
        case 1:
            cout << "Enter Minimum Age: ";
            cin >> minAge;
            cout << "Enter Maximum Age: ";
            cin >> maxAge;
            break;
        case 2:
            cout << "Enter Mode of Transport: ";
            cin >> outTransport;
            break;
        case 3:
            cout << "Enter Daily Distance threshold (residents with distance > this value): ";
            cin >> outDistance;
            break;
        default:
            cout << "Invalid search criteria selection. \n";
            return -1;
    }
    return opt;
}

int sortByMenu(int& minAge, int& maxAge)
{
    int opt = 0;

    cout << "=========================\n";
    cout << "Age Group Categorization \n";
    cout << "=========================\n\n";
    cout << "Age Group Categorization: \n";
    cout << "1. Age Group 6--17 \n";
    cout << "2. Age Group 18--25 \n";
    cout << "3. Age Group 26--45 \n";
    cout << "4. Age Group 46--60 \n";
    cout << "5. Age Group 61--100 \n";
    cin >> opt;

    switch (opt)
    {
        case 1: 
            cout << "You selected the age group: 6--17: Children & Teenagers \n";               
            minAge =  6; maxAge =  17; 
            break;
        case 2: 
            cout << "You selected the age group: 18--25: University Students / Young Adults \n"; 
            minAge = 18; maxAge =  25; 
            break;
        case 3: 
            cout << "You selected the age group: 26--45: Working Adults (Early Career) \n";     
            minAge = 26; maxAge =  45; 
            break;
        case 4: 
            cout << "You selected the age group: 46--60: Working Adults (Late Career) \n";      
            minAge = 46; maxAge =  60; 
            break;
        case 5: 
            cout << "You selected the age group: 61--100: Senior Citizens / Retirees \n";       
            minAge = 61; maxAge = 100; 
            break;
        default: 
            cout << "Invalid age group selection. \n"; 
            return -1;
    }
    return opt;
}

// sortFieldMenu: user chooses which field to sort by
// returns 1=Age, 2=Daily Distance, 3=Carbon Emission
int sortFieldMenu()
{
    int opt = 0;
    cout << "=========================\n";
    cout << "Sort By                  \n";
    cout << "=========================\n\n";
    cout << "1. Age \n";
    cout << "2. Daily Distance \n";
    cout << "3. Carbon Emission \n";
    cout << ">> ";
    cin >> opt;

    switch (opt)
    {
        case 1: cout << "You selected: Age \n";             break;
        case 2: cout << "You selected: Daily Distance \n";  break;
        case 3: cout << "You selected: Carbon Emission \n"; break;
        default:
            cout << "Invalid sort field selection. \n";
            return -1;
    }
    return opt;
}

// ----------------------------------------------- [Display Menu] ------------------------------------------------ [end]


// ----------------------------------------------- [Display Table Results] ----------------------------------------------- [start]

// Print a single row of the results table
void printTableRow(const CityA& c)
{
    // Monthly carbon emission = DailyDistance * CarbonEmissionFactor * AverageDaysPerMonth
    double monthlyEmission = c.DailyDistance * c.CarbonEmissionFactor * c.AverageDaysPerMonth;
    cout << "| " << c.ResidentID
         << " | " << c.Age
         << " | " << c.ModeOfTransport
         << " | " << c.DailyDistance   << " km"
         << " | " << c.CarbonEmissionFactor
         << " | " << c.AverageDaysPerMonth << " days"
         << " | " << monthlyEmission   << " kg CO2"
         << " |\n";
}

// Print table header
void printTableHeader()
{
    cout << "\n-----------------------------------------------------------------------\n";
    cout << "| ResidentID | Age | Transport | Distance | Factor | Days | Monthly CO2 |\n";
    cout << "-----------------------------------------------------------------------\n";
}

// displaySearchResults: walks a linked list of matched records and prints each row
void displaySearchResults(node* results)
{
    if (results == nullptr)
    {
        cout << "No matching records found.\n";
        return;
    }
    int count = 0;
    node* curr = results;
    while (curr != nullptr) 
    { 
        count++; curr = curr->next; 
    } // count matches

    cout << "\nSearch Results (" << count << " record(s) found):\n";
    printTableHeader();
    curr = results;
    while (curr != nullptr)
    {
        printTableRow(curr->data);
        curr = curr->next;
    }
    cout << "-----------------------------------------------------------------------\n";
}

node* binarySearch(node* sortedGroup, int total, string transportMode);

void displaySortResults(node* group, int groupOpt, int n, string sortName, double timeTaken)
{
    if (group == nullptr)
    {
        cout << "No records found in this age group.\n";
        cout << "(Your dataset may not contain residents in this age range.)\n\n\n\n";
        dataset1CityA();
        return;
    }

    // Map group option to readable label
    string groupLabel;
    switch (groupOpt)
    {
        case 1: 
            groupLabel = "Age group 6-17";           
            break;
        case 2: 
            groupLabel = "Age group 18-25";           
            break;
        case 3: 
            groupLabel = "Age group 26-45"; 
            break;
        case 4:
            groupLabel = "Age group 46-60";   
            break;
        case 5: 
            groupLabel = "Age group 61-100";   
            break;
        default: 
            groupLabel = "Unknown";
    }

    // --- Step 1: Discover unique transport modes by walking the sorted list ---
    modeNode* modeHead = nullptr; 
    modeNode* modeTail = nullptr;

    node* curr = group;
    while (curr != nullptr)
    {
        bool seen = false;
        modeNode* mCurr = modeHead;
        while (mCurr != nullptr)
        {
            if (mCurr->modeName == curr->data.ModeOfTransport)
            {
                seen = true;
                break;
            }
            mCurr = mCurr->next;
        }

        
        if (!seen)
        {
            modeNode* newMode = new modeNode();
            newMode->modeName = curr->data.ModeOfTransport;
            newMode->next = nullptr;
            if (modeHead == nullptr) 
            { 
                modeHead = newMode; modeTail = newMode; 
            }
            else                     
            { 
                modeTail->next = newMode; modeTail = newMode; 
            }

        }
        curr = curr->next;
    }

    // --- Step 2: Count total nodes for binary search index ---
    int total = 0;
    curr = group;
    while (curr != nullptr)
    {
        total++;
        curr = curr->next;
    }

    cout << "\nAge Group: " << groupLabel << "\n";
    cout << "----------------------------------------\n";
    cout << "Mode of Transport    Count   Total Emission (kg CO2)   Average per Resident\n";

    double groupTotal = 0.0;

    // --- Step 3: For each transport mode, call binarySearch() to get matching nodes ---
    modeNode* mWalk = modeHead;
    while (mWalk != nullptr)
    {
        node* matches = binarySearch(group, total, mWalk->modeName);

        int count = 0;
        double totalEmission = 0.0;
        node* curr2 = matches;
        while (curr2 != nullptr)
        {
            count++;
            // Monthly emission = DailyDistance * CarbonEmissionFactor * AverageDaysPerMonth
            totalEmission += curr2->data.DailyDistance
                           * curr2->data.CarbonEmissionFactor
                           * curr2->data.AverageDaysPerMonth;
            curr2 = curr2->next;
        }
        deleteNode(matches);

        double avg;
        if (count > 0)
        {
            avg = totalEmission / count;
        }
        else
        {
            avg = 0.0;
        }

        groupTotal += totalEmission;

        // Print mode name, pad to 21 chars
        cout << mWalk->modeName;
        for (int p = mWalk->modeName.length(); p < 21; p++)
        {
            cout << " ";
        }

        // Count, pad to 8 chars
        string countStr = to_string(count);
        cout << count;
        for (size_t p = countStr.length(); p < 8; p++)
        {
            cout << " ";
        }

        // Total emission, pad to 26 chars
        string totStr = to_string((int)totalEmission);
        cout << (int)totalEmission;
        for (size_t p = totStr.length(); p < 26; p++)
        {
            cout << " ";
        }

        cout << avg << "\n";

        mWalk = mWalk->next; // move to next transport mode
    }

    // Free the modeNode linked list
    modeNode* mTemp = modeHead;
    while (mTemp != nullptr)
    {
        modeNode* mDel = mTemp;
        mTemp = mTemp->next;
        delete mDel;
    }

    cout << "----------------------------------------\n";
    cout << "Total Emission for Age Group: " << groupTotal << " kg CO2\n";

    // --- Memory Usage ---
    cout << "\n--- Memory Usage ---\n";
    cout << "Memory Usage : " << n << " elements / " << sizeof(CityA) << " bytes per element = " << (n * sizeof(CityA)) << " bytes\n";

    // --- Time Complexity ---
    cout << "\n--- Time Complexity (" << sortName << ") ---\n";
    if (sortName == "Insertion Sort")
    {
        cout << "Time Complexity           : O(n^2)\n";
    }
    else
    {
        cout << "Time Complexity           : O(n log n)\n";
    }
    cout << "Time Taken                : " << timeTaken << " ms\n";

    // --- Post-table menu ---
    cout << "\n==================\n";
    cout << "1. Display Sorted List\n";
    cout << "2. Exit\n";
    cout << "Select: ";
    int postOpt = 0;
    cin >> postOpt;

    switch (postOpt)
    {
        case 1:
        {
            cout << "\nSorted List " << groupLabel << "\n";
            printTableHeader();
            node* row = group;
            while (row != nullptr)
            {
                printTableRow(row->data);
                row = row->next;
            }
            cout << "-----------------------------------------------------------------------\n";
            dataset1CityA();
            break;
        }
        default:
            return;
    }
}

// ----------------------------------------------- [Display Table Results] ----------------------------------------------- [end]


// ----------------------------------------------- [Sorting Algorithms] ----------------------------------------------- [start]

// insertionSort: sorts by chosen field (1=Age, 2=Daily Distance, 3=Carbon Emission)
void insertionSort(node* head, int sortField)
{
    int minAge = 0, maxAge = 0, groupOpt = -1;

    if (sortField == 0)
    {
        // Carbon Emission Analysis mode — filter by age group first
        groupOpt = sortByMenu(minAge, maxAge);
        if (groupOpt == -1) { dataset1CityA(); return; }
    }

    // Step 1: Linear Search — collect matching nodes
    node* group     = nullptr;
    node* groupTail = nullptr;
    node* curr = head;
    while (curr != nullptr)
    {
        bool include = false;
        if (sortField == 0)
            include = (curr->data.Age >= minAge && curr->data.Age <= maxAge);
        else
            include = true; // sort all records for Sort By mode

        if (include)
            insertionAtEnd(group, groupTail, curr->data);
        curr = curr->next;
    }

    // Count n
    int n = 0;
    curr = group;
    while (curr != nullptr) { n++; curr = curr->next; }

    // Step 2: Insertion Sort — start chrono timer
    auto startTime = chrono::high_resolution_clock::now();

    node* sorted = nullptr;
    curr = group;
    while (curr != nullptr)
    {
        node* next = curr->next;

        // Compare based on chosen sort field
        bool insertAtFront = false;
        if (sorted == nullptr)
        {
            insertAtFront = true;
        }
        else if (sortField == 1 || sortField == 0) // Age
        {
            insertAtFront = curr->data.Age <= sorted->data.Age;
        }
        else if (sortField == 2) // Daily Distance
        {
            insertAtFront = curr->data.DailyDistance <= sorted->data.DailyDistance;
        }
        else if (sortField == 3) // Carbon Emission
        {
            double currEmission  = curr->data.DailyDistance * curr->data.CarbonEmissionFactor * curr->data.AverageDaysPerMonth;
            double frontEmission = sorted->data.DailyDistance * sorted->data.CarbonEmissionFactor * sorted->data.AverageDaysPerMonth;
            insertAtFront = currEmission <= frontEmission;
        }

        if (insertAtFront)
        {
            curr->next = sorted;
            sorted = curr;
        }
        else
        {
            node* pos = sorted;
            while (pos->next != nullptr)
            {
                bool keepGoing = false;
                if (sortField == 1 || sortField == 0) // Age
                    keepGoing = pos->next->data.Age <= curr->data.Age;
                else if (sortField == 2) // Daily Distance
                    keepGoing = pos->next->data.DailyDistance <= curr->data.DailyDistance;
                else if (sortField == 3) // Carbon Emission
                {
                    double posEmission  = pos->next->data.DailyDistance * pos->next->data.CarbonEmissionFactor * pos->next->data.AverageDaysPerMonth;
                    double currEmission = curr->data.DailyDistance * curr->data.CarbonEmissionFactor * curr->data.AverageDaysPerMonth;
                    keepGoing = posEmission <= currEmission;
                }
                if (!keepGoing) break;
                pos = pos->next;
            }
            curr->next = pos->next;
            pos->next  = curr;
        }
        curr = next;
    }
    group = sorted;

    auto endTime = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration<double, milli>(endTime - startTime).count();

    if (sortField == 0)
    {
        // Carbon Emission Analysis — show emission table
        displaySortResults(group, groupOpt, n, "Insertion Sort", timeTaken);
    }
    else
    {
        // Sort By mode — show sorted list table directly
        cout << "\nSorted List (Insertion Sort)\n";
        printTableHeader();
        node* row = group;
        while (row != nullptr) { printTableRow(row->data); row = row->next; }
        cout << "-----------------------------------------------------------------------\n";
        cout << "\n--- Memory Usage ---\n";
        cout << "Memory Usage : " << n << " elements / " << sizeof(CityA) << " bytes per element = " << (n * sizeof(CityA)) << " bytes\n";
        cout << "\n--- Time Complexity (Insertion Sort) ---\n";
        cout << "Time Complexity : O(n^2)\n";
        cout << "Time Taken      : " << timeTaken << " ms\n";
    }
    deleteNode(group);
}

// splitList: splits list into two halves using slow/fast pointer technique
void splitList(node* source, node*& front, node*& back)
{
    if (source == nullptr || source->next == nullptr)
    {
        front = source;
        back  = nullptr;
        return;
    }
    node* slow = source;
    node* fast = source->next;
    // fast moves 2 steps, slow moves 1 — when fast reaches end, slow is at midpoint
    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    front = source;
    back  = slow->next;
    slow->next = nullptr; // cut the list in half
}

// mergeSorted: merges two sorted linked lists based on chosen sort field
node* mergeSorted(node* a, node* b, int sortField)
{
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    // Compare based on sort field
    bool aFirst = false;
    if (sortField == 1 || sortField == 0) // Age
        aFirst = a->data.Age <= b->data.Age;
    else if (sortField == 2) // Daily Distance
        aFirst = a->data.DailyDistance <= b->data.DailyDistance;
    else if (sortField == 3) // Carbon Emission
    {
        double aEmission = a->data.DailyDistance * a->data.CarbonEmissionFactor * a->data.AverageDaysPerMonth;
        double bEmission = b->data.DailyDistance * b->data.CarbonEmissionFactor * b->data.AverageDaysPerMonth;
        aFirst = aEmission <= bEmission;
    }

    node* result = nullptr;
    if (aFirst)
    {
        result = a;
        result->next = mergeSorted(a->next, b, sortField);
    }
    else
    {
        result = b;
        result->next = mergeSorted(a, b->next, sortField);
    }
    return result;
}

// mergeSortList: recursive merge sort on a linked list
node* mergeSortList(node* head, int sortField)
{
    if (head == nullptr || head->next == nullptr) return head;

    node* front = nullptr;
    node* back  = nullptr;
    splitList(head, front, back);
    front = mergeSortList(front, sortField);
    back  = mergeSortList(back,  sortField);
    return mergeSorted(front, back, sortField);
}

// mergeSort: sorts by chosen field (1=Age, 2=Daily Distance, 3=Carbon Emission)
void mergeSort(node* head, int sortField)
{
    int minAge = 0, maxAge = 0, groupOpt = -1;

    if (sortField == 0)
    {
        // Carbon Emission Analysis mode — filter by age group first
        groupOpt = sortByMenu(minAge, maxAge);
        if (groupOpt == -1) { dataset1CityA(); return; }
    }

    // Step 1: Linear Search — collect matching nodes
    node* group     = nullptr;
    node* groupTail = nullptr;
    node* curr = head;
    while (curr != nullptr)
    {
        bool include = false;
        if (sortField == 0)
            include = (curr->data.Age >= minAge && curr->data.Age <= maxAge);
        else
            include = true; // sort all records for Sort By mode

        if (include)
            insertionAtEnd(group, groupTail, curr->data);
        curr = curr->next;
    }

    // Count n
    int n = 0;
    curr = group;
    while (curr != nullptr) { n++; curr = curr->next; }

    // Step 2: Merge Sort — start chrono timer
    auto startTime = chrono::high_resolution_clock::now();
    group = mergeSortList(group, sortField);
    auto endTime = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration<double, milli>(endTime - startTime).count();

    if (sortField == 0)
    {
        // Carbon Emission Analysis — show emission table
        displaySortResults(group, groupOpt, n, "Merge Sort", timeTaken);
    }
    else
    {
        // Sort By mode — show sorted list table directly
        cout << "\nSorted List (Merge Sort)\n";
        printTableHeader();
        node* row = group;
        while (row != nullptr) { printTableRow(row->data); row = row->next; }
        cout << "-----------------------------------------------------------------------\n";
        cout << "\n--- Memory Usage ---\n";
        cout << "Memory Usage : " << n << " elements / " << sizeof(CityA) << " bytes per element = " << (n * sizeof(CityA)) << " bytes\n";
        cout << "\n--- Time Complexity (Merge Sort) ---\n";
        cout << "Time Complexity : O(n log n)\n";
        cout << "Time Taken      : " << timeTaken << " ms\n";
    }
    deleteNode(group);
}

// ----------------------------------------------- [Sorting Algorithms] ----------------------------------------------- [end]


// ----------------------------------------------- [Search Algorithms] ----------------------------------------------- [start]

// linearSearch: walks every node, collects matches, shows memory and time complexity
void linearSearch(node* head)
{
    int minAge = -1, maxAge = -1, outDistance = -1;
    string outTransport = "";
    int field = searchByMenu(minAge, maxAge, outTransport, outDistance);
    if (field == -1) { dataset1CityA(); return; }

    // Count total n for memory usage
    int n = 0;
    node* tmp = head;
    while (tmp != nullptr) { n++; tmp = tmp->next; }

    node* results     = nullptr;
    node* resultsTail = nullptr;

    // Start chrono timer
    auto startTime = chrono::high_resolution_clock::now();

    node* curr = head;
    while (curr != nullptr)
    {
        bool match = false;
        // field 1: check if Age is within the user-specified range
        if      (field == 1 && curr->data.Age >= minAge && curr->data.Age <= maxAge) match = true;
        else if (field == 2 && curr->data.ModeOfTransport == outTransport)           match = true;
        else if (field == 3 && curr->data.DailyDistance   >  outDistance)            match = true;

        if (match)
            insertionAtEnd(results, resultsTail, curr->data);
        curr = curr->next;
    }

    auto endTime = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration<double, milli>(endTime - startTime).count();

    cout << "\n--- Linear Search Results ---\n";
    displaySearchResults(results);

    cout << "\n--- Memory Usage ---\n";
    cout << "Memory Usage : " << n << " elements / " << sizeof(CityA) << " bytes per element = " << (n * sizeof(CityA)) << " bytes\n";
    cout << "\n--- Time Complexity (Linear Search) ---\n";
    cout << "Time Complexity : O(n)\n";
    cout << "Time Taken      : " << timeTaken << " ms\n";

    deleteNode(results);
}

// binarySearch: searches a sorted linked list for a specific transport mode
// uses binary search to find a starting position hint, then collects all matching nodes
// returns a linked list of all nodes matching the transport mode
node* binarySearch(node* sortedGroup, int total, string transportMode)
{
    // Binary search: find index of any node matching transportMode
    int lo = 0, hi = total - 1, startIndex = -1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;

        // Walk to mid-th node
        node* midNode = sortedGroup;
        for (int i = 0; i < mid; i++) 
        {
            midNode = midNode->next;
        }

        if (midNode->data.ModeOfTransport == transportMode)
        {
            startIndex = mid;   // found a match — record position and search left for first
            hi = mid - 1;
        }
        else
        {
            lo = mid + 1;       // not found at mid, search right half
        }
    }

    node* results     = nullptr;
    node* resultsTail = nullptr;
    node* curr = sortedGroup;
    while (curr != nullptr)
    {
        if (curr->data.ModeOfTransport == transportMode)
        {
            insertionAtEnd(results, resultsTail, curr->data);
        }
        curr = curr->next;
    }

    return results;
}

// binarySearchUser: sorts full list by the chosen field using insertion sort first,
// then applies binary search to find matching records
void binarySearchUser(node* head)
{
    int minAge = -1, maxAge = -1, outDistance = -1;
    string outTransport = "";
    int field = searchByMenu(minAge, maxAge, outTransport, outDistance);
    if (field == -1) { dataset1CityA(); return; }

    // Count total n for memory usage
    int n = 0;
    node* tmp = head;
    while (tmp != nullptr) { n++; tmp = tmp->next; }

    // Step 1: Copy full list
    node* sorted     = nullptr;
    node* sortedTail = nullptr;
    node* curr = head;
    while (curr != nullptr)
    {
        insertionAtEnd(sorted, sortedTail, curr->data);
        curr = curr->next;
    }

    // Step 2: Insertion Sort by the SAME field user picked
    auto startTime = chrono::high_resolution_clock::now();

    node* sortedResult = nullptr;
    curr = sorted;
    while (curr != nullptr)
    {
        node* next = curr->next;
        bool insertAtFront = false;

        if (sortedResult == nullptr)
        {
            insertAtFront = true;
        }
        else if (field == 1) // sort by Age
        {
            insertAtFront = curr->data.Age <= sortedResult->data.Age;
        }
        else if (field == 2) // sort by ModeOfTransport alphabetically
        {
            insertAtFront = curr->data.ModeOfTransport <= sortedResult->data.ModeOfTransport;
        }
        else if (field == 3) // sort by DailyDistance
        {
            insertAtFront = curr->data.DailyDistance <= sortedResult->data.DailyDistance;
        }

        if (insertAtFront)
        {
            curr->next = sortedResult;
            sortedResult = curr;
        }
        else
        {
            node* pos = sortedResult;
            while (pos->next != nullptr)
            {
                bool keepGoing = false;
                if      (field == 1) keepGoing = pos->next->data.Age             <= curr->data.Age;
                else if (field == 2) keepGoing = pos->next->data.ModeOfTransport <= curr->data.ModeOfTransport;
                else if (field == 3) keepGoing = pos->next->data.DailyDistance   <= curr->data.DailyDistance;
                if (!keepGoing) break;
                pos = pos->next;
            }
            curr->next = pos->next;
            pos->next  = curr;
        }
        curr = next;
    }
    sorted = sortedResult;

    // Step 3: Binary Search on the sorted list
    node* results     = nullptr;
    node* resultsTail = nullptr;

    if (field == 1) // Binary search by Age RANGE (minAge to maxAge)
    {
        // Find leftmost node where Age >= minAge
        int lo = 0, hi = n - 1, found = -1;
        while (lo <= hi)
        {
            int mid = lo + (hi - lo) / 2;
            node* midNode = sorted;
            for (int i = 0; i < mid; i++) midNode = midNode->next;

            if (midNode->data.Age >= minAge)
            {
                found = mid;    // qualifies, keep searching left for first
                hi = mid - 1;
            }
            else
            {
                lo = mid + 1;
            }
        }
        // Collect all nodes where Age is within [minAge, maxAge]
        if (found != -1)
        {
            node* n2 = sorted;
            for (int i = 0; i < found; i++) n2 = n2->next;
            while (n2 != nullptr && n2->data.Age <= maxAge)
            {
                if (n2->data.Age >= minAge)
                    insertionAtEnd(results, resultsTail, n2->data);
                n2 = n2->next;
            }
        }
    }
    else if (field == 2) // Binary search by Mode of Transport
    {
        int lo = 0, hi = n - 1, found = -1;
        while (lo <= hi)
        {
            int mid = lo + (hi - lo) / 2;
            node* midNode = sorted;
            for (int i = 0; i < mid; i++) midNode = midNode->next;

            if      (midNode->data.ModeOfTransport == outTransport) { found = mid; hi = mid - 1; }
            else if (midNode->data.ModeOfTransport <  outTransport)   lo = mid + 1;
            else                                                       hi = mid - 1;
        }
        if (found != -1)
        {
            node* n2 = sorted;
            for (int i = 0; i < found; i++) n2 = n2->next;
            while (n2 != nullptr && n2->data.ModeOfTransport == outTransport)
            {
                insertionAtEnd(results, resultsTail, n2->data);
                n2 = n2->next;
            }
        }
    }
    else if (field == 3) // Binary search by Daily Distance threshold
    {
        int lo = 0, hi = n - 1, found = -1;
        while (lo <= hi)
        {
            int mid = lo + (hi - lo) / 2;
            node* midNode = sorted;
            for (int i = 0; i < mid; i++) midNode = midNode->next;

            if (midNode->data.DailyDistance > outDistance)
            {
                found = mid;
                hi = mid - 1;
            }
            else
            {
                lo = mid + 1;
            }
        }
        if (found != -1)
        {
            node* n2 = sorted;
            for (int i = 0; i < found; i++) n2 = n2->next;
            while (n2 != nullptr && n2->data.DailyDistance > outDistance)
            {
                insertionAtEnd(results, resultsTail, n2->data);
                n2 = n2->next;
            }
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    double timeTaken = chrono::duration<double, milli>(endTime - startTime).count();

    cout << "\n--- Binary Search Results ---\n";
    displaySearchResults(results);

    cout << "\n--- Memory Usage ---\n";
    cout << "Memory Usage : " << n << " elements / " << sizeof(CityA) << " bytes per element = " << (n * sizeof(CityA)) << " bytes\n";
    cout << "\n--- Time Complexity (Binary Search) ---\n";
    cout << "Time Complexity : O(n log n)\n";
    cout << "Time Taken      : " << timeTaken << " ms\n";

    deleteNode(results);
    deleteNode(sorted);
}
// ----------------------------------------------- [Search Algorithms] ----------------------------------------------- [end]

// ----------------------------------------------- [Main Function] ----------------------------------------------- [start]
int dataset1CityA()
{
    int temp;
    int *opt = &temp;

    node* head = readCSV("dataset1-cityA.csv");
    if (head == nullptr)
    {
        cout << "No data loaded. Exiting.\n";
        return 1;
    }

    cout << "=============================================== \n";
    cout << "Welcome to the C++ Programming Assignment! \n";
    cout << "=============================================== \n\n";
    cout << "1. Searching Experiments \n";
    cout << "2. Sorting Experiments \n";
    cout << "3. Exit \n";
    cout << "Select your action: ";
    cin >> *opt;

    switch (temp)
    {
        case 1:
        {
            // Searching Experiments: user picks search algorithm
            cout << "\n=========================\n";
            cout << "Searching Experiments \n";
            cout << "=========================\n\n";
            cout << "1. Linear Search \n";
            cout << "2. Binary Search \n";
            cout << "Select a search algorithm: ";
            cin >> *opt;

            switch (temp)
            {
                case 1:
                    linearSearch(head);
                    break;
                case 2:
                    binarySearchUser(head);
                    break;
                default:
                    cout << "Invalid search algorithm selection.\n";
            }
            break;
        }
        case 2:
        {
            // Sorting Experiments
            cout << "\n===================\n";
            cout << "Sorting Experiments \n";
            cout << "===================\n\n";
            cout << "1. Insertion Sort \n";
            cout << "2. Merge Sort \n";
            cout << "Select a sorting algorithm: ";
            cin >> *opt;

            int sortAlgo = temp;

            cout << "\n==================\n";
            cout << "1. Sort By \n";
            cout << "2. Carbon Emission Analysis \n";
            cout << "Select: ";
            cin >> *opt;

            switch (temp)
            {
                case 1:
                {
                    // Sort By — user picks field, sort all dataset
                    int sortField = sortFieldMenu();
                    if (sortField == -1) 
                    { 
                        cout << "Invalid selection.\n"; 
                        dataset1CityA();
                        break; 
                    }

                    if (sortAlgo == 1)
                        insertionSort(head, sortField);
                    else if (sortAlgo == 2)
                        mergeSort(head, sortField);
                    else
                        cout << "Invalid algorithm selection.\n";
                    break;
                }
                case 2:
                {
                    if (sortAlgo == 1)
                        insertionSort(head, 0);
                    else if (sortAlgo == 2)
                        mergeSort(head, 0);
                    else
                        cout << "Invalid algorithm selection.\n";
                    break;
                }
                default:
                    cout << "Invalid selection.\n";
            }
            break;
        }
        case 3:
            cout << "Exiting the program. Goodbye!\n";
            break;
        default:
            cout << "Invalid action selection. \n";
    }

    deleteNode(head);
    return 0;
}
// ----------------------------------------------- [Main Function] ----------------------------------------------- [end]