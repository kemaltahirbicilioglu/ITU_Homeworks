#ifndef METHODS_H
#define METHODS_H

#include "item.h"

// Find the maximum value in a vector of items by a given attribute
int getMax(std::vector<Item>& items, const std::string& attribute);

// Calculate the raririty of an item
void calculateRarityScores(std::vector<Item>& items, int ageWindow = 50);

// Function prototypes for sorting
std::vector<Item> countingSort(std::vector<Item>& items, const std::string& attribute, bool ascending);
void heapify(std::vector<Item>& items, int n, int i, bool descending);
std::vector<Item> heapSortByRarity(std::vector<Item>& items, bool descending = true);

// Function prototypes for utilities
std::vector<Item> readItemsFromFile(const std::string& filename);
void writeItemsToFile(const std::string& filename, const std::vector<Item>& items);

#endif
