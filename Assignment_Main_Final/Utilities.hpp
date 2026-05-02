#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// ============ Helper Function ============
static inline string normalize(const string& s) {
    string out = s;
    while (!out.empty() && isspace(out.front())) out.erase(out.begin());
    while (!out.empty() && isspace(out.back())) out.pop_back();
    for (char& c : out) c = tolower(c);
    return out;
}

#endif // UTILITIES_HPP
