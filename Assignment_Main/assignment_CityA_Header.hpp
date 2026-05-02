#ifndef ASSIGNMENT_CITYA_MAIN_HPP
#define ASSIGNMENT_CITYA_MAIN_HPP

#include <iostream>
#include <string>
#include <fstream>   
#include <sstream>   
#include <chrono>
using namespace std;

// ----------------------------------------------- [Linked list] -----------------------------------------------

struct CityA
{
    string ResidentID;
    int Age;
    string ModeOfTransport;
    int DailyDistance;
    double CarbonEmissionFactor;
    int AverageDaysPerMonth;
};

struct node
{
    CityA data;
    node* next;

    // Constructor — initializes data and sets next to nullptr automatically
    node(CityA d) : data(d), next(nullptr) {}
};

struct modeNode
{
    string modeName;
    modeNode* next;
};

// ----------------------------------------------- [Class] -----------------------------------------------

class CityAData {
public:
    // Linked List & CSV
    void insertionAtEnd(node*& head, node*& tail, CityA data);
    void deleteNode(node*& head);
    node* readCSV(const string& filename);

    // Menus
    int searchByMenu(int& outAge, string& outTransport, int& outDistance);
    int sortByMenu(int& minAge, int& maxAge);
    int sortFieldMenu();

    // Display
    void printTableRow(const CityA& c);
    void printTableHeader();
    void displaySearchResults(node* results);
    void displaySortResults(node* group, int groupOpt, int n, string sortName, double timeTaken);

    // Searching
    void linearSearch(node* head);
    node* binarySearch(node* sortedGroup, int total, string transportMode);
    void binarySearchUser(node* head);

    // Sorting
    void insertionSort(node* head, int sortField);
    void splitList(node* source, node*& front, node*& back);
    node* mergeSorted(node* a, node* b, int sortField);
    node* mergeSortList(node* head, int sortField);
    void mergeSort(node* head, int sortField);
};

#endif 