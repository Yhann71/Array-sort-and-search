#include "WKO_C.hpp"

int dataset3CityC() {
    while (true) {
        SinglyLinkedList list;
        if (!list.readFromCSV()) {
            return 1;
        }

        int userChoice = 0;
        while (true) {
            cout << "\nSelect options provided: " << endl;
            cout << "  1. Display Full Dataset List" << endl;
            cout << "  2. Display Age Group Category" << endl;
            cout << "  3. Sorting Experiment" << endl;
            cout << "  4. Searching Experiment" << endl;
            cout << "  5. Exit" << endl;
            cout << "Enter choice (1-5): ";
            cin >> userChoice;

            switch (userChoice) {
                case 1: list.displayList(); break;
                case 2: list.ageGroupCategory(); break;
                case 3: list.sortingExperiment(); break;
                case 4: list.searchingExperiment(); break;
                case 5: return 0;
                default:
                    cout << "Invalid choice! Enter (1-5)" << endl;
                    break;
            }
        }
    }
    return 0;
}