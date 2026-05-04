#ifndef WKO_C_HPP
#define WKO_C_HPP
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
using namespace std;
 
// main function for City C dataset
int dataset3CityC();

struct Node {
    string ResidentID;
    int    Age;
    string ModeOfTransport;
    int    DailyDistance;
    double CarbonEmissionFactor;
    int    AverageDayPerMonth;
    Node*  next;
 
    // Constructor
    Node(string RID, int age, string transport,
         int distance, double carbon, int avgDay);
};
 
class SinglyLinkedList {
    public:
        Node* head;
        Node* sorted;
        int   size;
 
        // Constructor & Destructor
        SinglyLinkedList();
        ~SinglyLinkedList();
 
        // ---- Core Operations ----
        void append(string RID, int age, string transport,
                    int distance, double carbon, int avgDay);
        bool readFromCSV();
        void displayList();
        void displayData(Node* node);
        bool isEmpty();
        void reload();
 
        // ---- Searching ----
        void linearSearch(string targetID);
        void binarySearch(string targetID);
        Node* getMiddle(Node* start, Node* last);
        void linearSearch(string searchKey, string strTarget,
                          int minInt, int maxInt);
        void binarySearch(string searchKey, string strTarget,
                          int minInt, int maxInt);
        void searchingExperiment();
 
        // ---- Sorting ----
        bool compareNodes(Node* a, Node* b, string key);
        void sortedInsert(Node* newnode, string sortKey);
        void insertionSort(string sortKey = "ResidentID");
        Node* merge(Node* left, Node* right, string sortKey);
        Node* sort(Node* node, string sortKey);
        void mergeSort(string sortKey = "ResidentID");
        void displaySortedData(string sortKey);
        void sortingExperiment();
 
        // ---- Analysis ----
        void ageGroupCategory();
};
 
#endif // WKO_C_HPP