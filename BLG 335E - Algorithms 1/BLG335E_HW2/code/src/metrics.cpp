#include "methods.h"

// getMax and getMin function are useful to know the range of the inputs while sorting them in counting sort
int getMax(std::vector<Item>& items, const std::string& attribute) {
    int maxVal = -2147483648; // Minimum possible value for a 32-bit signed integer

    for (const auto& item : items) {
        if (attribute == "age") {
            maxVal = std::max(maxVal, item.age);
        } else if (attribute == "type") {
            maxVal = std::max(maxVal, item.type);
        } else if (attribute == "origin") {
            maxVal = std::max(maxVal, item.origin);
        }
    }

    return maxVal;
}

// min = age - ageWindow
// max = age + ageWindow
// rarityScore = (1 - probability) * (1 + item.age/ageMax)
void calculateRarityScores(std::vector<Item>& items, int ageWindow) {
    int n = items.size();
    
    // Find the maximum age for normalization in rarity score
    std::string age_attiribute = "age";
    int maxAge = getMax(items, age_attiribute);

    // Initialize left and right pointers
    int left = 0, right = 0;

    // Determine the initial right index for the first item's age window
    int minAge = items[0].age - ageWindow;
    int maxAgeWindow = items[0].age + ageWindow;
    while (right < n && items[right].age <= maxAgeWindow) {
        right++;
    }

    // Loop through all items
    for (int i = 0; i < n; ++i) {
        minAge = items[i].age - ageWindow;
        maxAgeWindow = items[i].age + ageWindow;

        // Adjust the left pointer to ensure it's within the current item's age window
        while (left < i && items[left].age < minAge) {
            left++;
        }

        // Adjust the right pointer to include all items within the current item's age window
        while (right < n && items[right].age <= maxAgeWindow) {
            right++;
        }

        // Count items in the window (left to right-1) and find similar ones
        int countTotal = 0, countSimilar = 0;
        for (int j = left; j < right; ++j) {
            countTotal++;
            if (items[j].type == items[i].type && items[j].origin == items[i].origin) {
                countSimilar++;
            }
        }

        // Calculate probability P for the current item
        double P = (countTotal > 0) ? static_cast<double>(countSimilar) / countTotal : 0;

        // Calculate rarity score for the current item with integer division for age normalization
        items[i].rarityScore = (1 - P) * (1 + static_cast<double>(items[i].age) / maxAge);  // Integer division
    }
}



