#include <iostream>
#include <string>
#include "assignment_CityA_Main.hpp"
#include "array_implementation.hpp"
#include "MergeLinear.hpp"
#include "City_C_menu.hpp"
using namespace std;

int main();

void linkedListMenu()
{
    int temp;
    int *opt = &temp;
    cout << "=============================================== \n";
    cout << "Choose data set \n";
    cout << "=============================================== \n";
    cout << "1. City A \n";
    cout << "2. City B \n";
    cout << "3. City C \n";
    cout << "4. Return to main menu \n";
    cout << ">>";
    cin >> *opt;
    cout << "\n";

    switch(temp)
    {
        case 1:
            dataset1CityA();
            break;
        case 2:
            cout << "You have chosen City B dataset. \n";
            break;
        case 3:
            dataset3CityC();
            break;
        case 4:
            cout << "Returning to main menu. \n";
            main();
        default:
            cout << "Invalid selection. Please choose 1, 2, 3, or 4. \n";
            linkedListMenu();
    }
}

void arrayMenu()
{
    int temp;
    int *opt = &temp;
    cout << "=============================================== \n";
    cout << "Choose data set \n";
    cout << "=============================================== \n";
    cout << "1. Binary Insertion \n";
    cout << "2. Linear Merge \n";
    cout << "3. Return to main menu \n";
    cout << ">>";
    cin >> *opt;
    cout << "\n";

    switch(temp)
    {
        case 1:
            binaryInsertion();
            break;
        case 2:
            linearMerge();
            break;
        case 3:
            cout << "Returning to main menu. \n";
            main();
        default:
            cout << "Invalid selection. Please choose 1, 2, or 3. \n";
            arrayMenu();
    }
}

int main()
{
    int temp;

    while (true) {
        cout << "=============================================== \n";
        cout << "Data structures in C++ Main Menu \n";
        cout << "=============================================== \n";
        cout << "choose your program on array or linked list: \n";
        cout << "1. Array \n";
        cout << "2. Linked List \n";
        cout << "3. Exit \n";
        cout << ">>";
        cin >> temp;
        cout << "\n";

        switch(temp)
        {
            case 1:
                arrayMenu();
                break;
            case 2:
                linkedListMenu();
                break;
            case 3:
                cout << "You have chosen to exit. Goodbye! \n";
                return 0;
            default:
                cout << "Invalid selection. Please choose 1, 2, or 3. \n";
                main();
        }   
    }  
}