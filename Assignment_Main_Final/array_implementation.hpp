#ifndef ARRAY_IMPLEMENTATION_HPP
#define ARRAY_IMPLEMENTATION_HPP

#include <iostream>
#include <string>
using namespace std;

// -------- STRUCT --------
struct Residents {
    string id;
    int age;
    string mode;
    double distance;
    double factor;
    int days;
};

// main
int binaryInsertion();

// -------- GLOBAL --------
extern Residents arr[1000];
extern int sizeArr;

// -------- FUNCTIONS --------
void loadCSV(string filename);
void displayHeader();
void printRow(Residents r);

int chooseDataset();
void loadSelectedDataset(int opt);

// Search
void searchMenu();
void binarySearchAge(int target);
void binarySearchAgeGroup(int minAge);
void binarySearchMode(string target);
void binarySearchDistance(double threshold);

// Sorting
void insertionSort();
void displaySortedList();
void sortByDistance();

// Analysis
void analysis();
int getAgeGroup(int& minAge, int& maxAge);

#endif
