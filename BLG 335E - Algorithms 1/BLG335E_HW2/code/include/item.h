#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream> 
#include <sstream>
#include <ctime> 
//#include <chrono> // EXTRA, to be able to measure the time
//#include <algorithm> // EXTRA, to test with stable_sort, and binary_search 

struct Item {
    int age;                // Item Age
    int type;               // Item Type
    int origin;             // Item Origin
    double rarityScore;     // Item Rarity Score
};

enum types {
    pottery = 0,
    sculpture = 1,
    jewelry = 2,
    weaponry = 3,
    tool = 4
};

enum origins {
    Roman = 0,
    Egyptian = 1,
    Aztec = 2,
    Greek = 3,
    Chinese = 4,
    Persian = 5,
    Viking = 6
};

#endif
