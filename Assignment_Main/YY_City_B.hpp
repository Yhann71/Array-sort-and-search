#ifndef YY_CITY_B_HPP
#define YY_CITY_B_HPP

#include <iostream>
#include <string>
using namespace std;

// ============================================================
//  NodeB  –  one record from dataset2-cityB.csv
// ============================================================
class NodeB {
public:
    // --- Data fields (order matches constructor initialiser list) ---
    string ResidentID;
    int    Age;
    string ModeOfTransport;
    int    DailyDistance;
    double CarbonEmissionFactor;
    int    AverageDayPerMonth;
    NodeB*  next;

    // --- Public constructor ---
    NodeB(string RID, int age, string transport,
         int distance, double carbon, int avgDay);
};

// ============================================================
//  SinglyLinkedListB  –  manages the City B dataset
// ============================================================
class SinglyLinkedListB {
public:
    // ---- Constructor & Destructor ----
    SinglyLinkedListB();
    ~SinglyLinkedListB();

    // ---- File Operations ----
    bool readFromCSV();
    void reload();
    void clear();

    // ---- Display ----
    void displayList();
    void displaySorted(const string& key);

    // ---- Sorting Algorithms ----
    void insertionSort(const string& key);
    void mergeSort(const string& key);          // public wrapper
    void sortingExperiment();

    // ---- Searching Algorithms ----
    void linearSearch(const string& key,
                      int minVal = 0, int maxVal = 0,
                      string strTarget = "");
    void binarySearch(const string& key,
                      int minVal = 0, int maxVal = 0,
                      string strTarget = "");
    void searchingExperiment();

    // ---- Age Group Categorisation (Task 4 + Task 5) ----
    void ageGroupCategory();

    // ---- Carbon Emission Analysis (Task 5 – standalone menu option) ----
    void carbonEmissionAnalysis();

    // ---- Getter ----
    int getSize() const;

private:
    NodeB* head;
    NodeB* sorted;
    int   size;

    // --- Internal helpers ---
    void  append(string RID, int age, string transport,
                 int distance, double carbon, int avgDay);
    bool  compare(NodeB* a, NodeB* b, const string& key);
    void  sortedInsert(NodeB* newNodeB, const string& key);
    NodeB* getMiddle(NodeB* start, NodeB* last = nullptr);
    NodeB* merge(NodeB* a, NodeB* b, const string& key);
    NodeB* mergeSort(NodeB* h, const string& key);   // private recursive core

    // --- Carbon emission helper ---
    void printEmissionTable(int minAge, int maxAge, const string& groupName);
};

#endif // YY_CITY_B_HPP
