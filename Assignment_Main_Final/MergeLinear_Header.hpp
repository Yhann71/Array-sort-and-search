#ifndef MERGE_LINEAR_HPP
#define MERGE_LINEAR_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <cctype>

using namespace std;

// Define MAX size for arrays
const int MAX = 1000;

// Resident data structure
struct Resident {
    string id;
    int age;
    string transportMode;
    double dailyDistance;
    double carbonEmission;
    int daysPerMonth;
};

// --- GLOBAL VARIABLE DECLARATIONS (with 'extern') ---
extern Resident cityA[MAX];
extern Resident cityB[MAX];
extern Resident cityC[MAX];
extern int sizeA;
extern int sizeB;
extern int sizeC;

// --- FUNCTION DECLARATIONS ---

// 1. Data Loading
void loadData(string filename, Resident arr[], int& size);

// 2. Linear Search Algorithms
void searchByAge(Resident arr[], int size, string cityName, int minAge, int maxAge);
void searchByTransport(Resident arr[], int size, string cityName, string targetMode);
void searchByDistance(Resident arr[], int size, string cityName, double minD, double maxD);
void searchByCarbonEmission(Resident arr[], int size, string cityName, double targetCarbon);

// 3. Sorting Algorithms
void merge(Resident arr[], int left, int mid, int right, int sortBy);
void mergeSort(Resident arr[], int left, int right, int sortBy);

// 4. Analysis and Display
void displaySorted(Resident arr[], int size, string cityName, int sortChoice);
void emissionAnalysis(Resident arr[], int size, string cityName, int ageChoice);

// 5. Utility
void copyArray(Resident src[], Resident dst[], int size);
int selectCity();

// --- THE FIX: Declare linearMerge so mainMenu can see it ---
int linearMerge();

#endif // MERGE_LINEAR_HPP
