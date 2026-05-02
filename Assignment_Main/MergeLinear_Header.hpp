#ifndef MERGE_LINEAR_HEADER_HPP
#define MERGE_LINEAR_HEADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>  
#include <chrono>   

using namespace std;

// 1. Data Structure
struct Resident {
    string id;
    int    age;
    string transportMode;
    double dailyDistance;
    double carbonEmission;
    int    daysPerMonth;
};

// 2. Global Variables (Using 'extern' so the compiler knows they exist)
const int MAX = 500;

extern Resident cityA[MAX];
extern Resident cityB[MAX];
extern Resident cityC[MAX];

extern int sizeA;
extern int sizeB;
extern int sizeC;

// 3. Function Declarations (Notice the semicolons!)
void loadData(string filename, Resident arr[], int& size);

void merge(Resident arr[], int left, int mid, int right, int sortBy);
void mergeSort(Resident arr[], int left, int right, int sortBy);
void copyArray(Resident src[], Resident dst[], int size);

void searchByAge(Resident arr[], int size, string cityName, int targetAge);
void searchByTransport(Resident arr[], int size, string cityName, string targetMode);
void searchByDistance(Resident arr[], int size, string cityName, double minD, double maxD);

void displaySorted(Resident arr[], int size, string cityName, int sortChoice);
void emissionAnalysis(Resident arr[], int size, string cityName);
int selectCity();

#endif
