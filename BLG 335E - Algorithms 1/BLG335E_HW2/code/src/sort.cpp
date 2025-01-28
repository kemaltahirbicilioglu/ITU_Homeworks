#include "methods.h"

int getMin(std::vector<Item>& items, const std::string& attribute) {
    int minVal = 2147483647; // Maximum possible value for a 32-bit signed integer

    for (const auto& item : items) {
        if (attribute == "age") {
            minVal = std::min(minVal, item.age);
        } else if (attribute == "type") {
            minVal = std::min(minVal, item.type);
        } else if (attribute == "origin") {
            minVal = std::min(minVal, item.origin);
        }
    }

    return minVal;
}

std::vector<int> createCountArray(std::vector<Item>& items, const std::string& attribute, bool ascending) {
    int minVal = getMin(items, attribute);
    int maxVal = getMax(items, attribute);
    int range = maxVal - minVal + 1; // we determined the item elements range.

    std::vector<int> count(range, 0);

    for (const auto& item : items) {
        int value = (attribute == "age") ? item.age : (attribute == "type") ? item.type : item.origin;
        count[value - minVal]++;
    }
    // If ascending, their corresponding last indeces is just a prefix sum of the occurences so that smaller values have smaller indeces
    // If descending, their corresponding last indeces should be summed in reverse order so that smaller values have larger indeces 
    if (ascending) {
        for (int i = 1; i < range; i++) {
            count[i] += count[i - 1];
        }
    } else {
        for (int i = range - 2; i >= 0; i--) {
            count[i] += count[i + 1];
        }
    }

    return count;
}

std::vector<Item> countingSort(std::vector<Item>& items, const std::string& attribute, bool ascending) {
    int minVal = getMin(items, attribute);
    std::vector<int> count = createCountArray(items, attribute, ascending); // this will give us the correct indeces to put the values

    std::vector<Item> sortedItems(items.size()); // initialize an array, we are going to place the values according to the count array
    
    // We can preserve the relative order of the items (stable sorting) by traversing the array
    // in reverse order since the count array tells us the last available position for a given value.
    for (int i = items.size() - 1; i >= 0; i--) {
        int value = (attribute == "age") ? items[i].age : (attribute == "type") ? items[i].type : items[i].origin;
        int index = count[value - minVal] - 1; // last index to put the current value
        sortedItems[index] = items[i]; // put the current value to that index
        count[value - minVal]--; // then decrement to the next available place for that value 
    }

    return sortedItems;
}

// Function to heapify a subtree rooted with node i in the array of items
void heapify(std::vector<Item>& items, int n, int i, bool descending) {
    int largestOrSmallest = i; // Initialize the current index as largestOrSmallest candidate as if no swap required
    int left = 2 * i + 1; // Left child index
    int right = 2 * i + 2; // Right child index

    // Determine if we need to compare for max-heap (ascending) or min-heap (descending)
    if (left < n) { // index check
        bool condition = (!descending) ? items[left].rarityScore > items[largestOrSmallest].rarityScore : items[left].rarityScore < items[largestOrSmallest].rarityScore;
        if (condition) {
            largestOrSmallest = left;
        }
    }

    if (right < n) { // index check
        bool condition = (!descending) ? items[right].rarityScore > items[largestOrSmallest].rarityScore : items[right].rarityScore < items[largestOrSmallest].rarityScore;
        if (condition) {
            largestOrSmallest = right;
        }
    }

    // We compared the current index with left and right child and correct index is in largestOrSmallest.
    // If largestOrSmallest is not the root, swap and continue heapifying
    if (largestOrSmallest != i) {
        std::swap(items[i], items[largestOrSmallest]);
        heapify(items, n, largestOrSmallest, descending); // Recursively heapify the affected subtree
    }
    // if the current index is leaf, no comparasion will be made, so largestOrSmallest will be i, and no recursive call for them
}

// Function to perform heap sort on rarityScore scores of items
std::vector<Item> heapSortByRarity(std::vector<Item>& items, bool descending) {
    int n = items.size();

    // Build the heap (rearrange the array), call heapify for the internal nodes
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(items, n, i, descending);
    }

    // One by one extract elements from the heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end of the subarray, 
        // ascending is max-heap, max element is items[0], so swap it to last index so that ascending 
        std::swap(items[0], items[i]); 

        // Call heapify on the reduced heap
        heapify(items, i, 0, descending);
    }

    return items; // Return the sorted array
}
