#include <iostream>
#include "YY_City_B.hpp"   // include linked list implementation
using namespace std;

int dataset2CityB() {
    cout << "\n";
    cout << "  ==========================================\n";
    cout << "     CITYVILLE TRANSPORTATION STUDY\n";
    cout << "       TARGET: CITY B  (UNI TOWN)\n";
    cout << "  ==========================================\n";
    cout << "Start program? (Y/N): ";

    string choice;
    cin >> choice;

    if (choice != "Y" && choice != "y") return 0;

    SinglyLinkedListB list;
    if (!list.readFromCSV()) return 1;

    while (true) {
        cout << "\n==========================================\n";
        cout << "           City B   Main Menu\n";
        cout << "==========================================\n";
        cout << "  1. Display Full Dataset\n";
        cout << "  2. Age Group Categorisation\n";
        cout << "  3. Sorting Experiments\n";
        cout << "  4. Searching Experiments\n";
        cout << "  5. Carbon Emission Analysis\n";
        cout << "  6. Exit\n";
        cout << "==========================================\n";
        cout << "Enter choice (1-6): ";

        int option = 0;
        if (!(cin >> option)) {
            // Non-numeric input (letters etc.)
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Error] Invalid input – please enter a number between 1 and 6.\n";
            continue;
        }

        switch (option) {
            case 1: list.displayList();             break;
            case 2: list.ageGroupCategory();        break;
            case 3: list.sortingExperiment();       break;
            case 4: list.searchingExperiment();     break;
            case 5: list.carbonEmissionAnalysis();  break;
            case 6:
                cout << "Exiting City B program. Goodbye!\n";
                return 0;
            default:
                cout << "[Error] Invalid choice. Please enter a number between 1 and 6.\n";
        }

        // After returning from any function, reload original data
        // so the next operation always starts from the clean dataset.
        list.reload();
    }

    return 0;
}
