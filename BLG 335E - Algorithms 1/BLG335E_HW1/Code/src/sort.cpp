#include "tweet.h"

/* because all of the functions need these comparasions, we can return a boolean value to be swapped or not to be swapped
the statement (isMerge && first.tweetID == second.tweetID) is to preserve stablisim also for the merge sort. 
[{2,10}, {1,8}, {1,7}, {2,15}], their relative order should be preserved. In swap case, we should not swap when they are equal.
In merge sort case we should choose the first one while merging the two values 
The resultened array should be [{1,8}, {1,7}, {2,10}, {2,15}]*/ 
bool shouldSwap(const Tweet& first, const Tweet& second, const std::string& sortBy, bool ascending, bool isMerge) 
{
    if (sortBy == "tweetID") {
        if (ascending) { // if ascending, and first one is bigger then second one, they need to be swapped.
            return first.tweetID > second.tweetID || (isMerge && first.tweetID == second.tweetID);
        } else { // if descending, and the first one is smaller then the second one, they need to be swapped.
            return first.tweetID < second.tweetID || (isMerge && first.tweetID == second.tweetID);
        } // same goes for other tweet values
    } else if (sortBy == "retweetCount") {
        if (ascending) {
            return first.retweetCount > second.retweetCount || (isMerge && first.retweetCount == second.retweetCount);
        } else {
            return first.retweetCount < second.retweetCount || (isMerge && first.retweetCount == second.retweetCount);
        }
    } else if (sortBy == "favoriteCount") {
        if (ascending) {
            return first.favoriteCount > second.favoriteCount || (isMerge && first.favoriteCount == second.favoriteCount);
        } else {
            return first.favoriteCount < second.favoriteCount || (isMerge && first.favoriteCount == second.favoriteCount);
        }
    }
    return false; // we dealt with all the possible cases, here is dummy default case, function should not come here.
}

// creates subarray from the tweets array starting with the start index and up to end index (exculeded)
std::vector<Tweet> create_subarray(const std::vector<Tweet>& tweets, int start, int end)
{
    std::vector<Tweet> subarray;
    for (int i = start; i < end; ++i) {
        subarray.push_back(tweets[i]);
    }
    return subarray;
}


void bubbleSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    int size = tweets.size();

    for (int i = 0; i < size - 1; i++) {
        bool swapped  = false; // little optimization, if current array is already sorted do not continue with checking
        for (int j = 0; j < size - 1 - i; j++) { // start comparasion from the beginning
            if (shouldSwap(tweets[j], tweets[j+1], sortBy, ascending, false)) {
                std::swap(tweets[j], tweets[j+1]);
                swapped = true;
            }
        }
        if(!swapped) break;
    }    
}


void insertionSort(std::vector<Tweet>& tweets, const std::string& sortBy, bool ascending) 
{
    int size = tweets.size();

    for (int i = 1; i < size; i++) {
        int right_index = i;
        int left_index = i - 1;
        // insert the current element to the correct place by comparing the adjacent elements
        while (left_index >= 0 && shouldSwap(tweets[left_index], tweets[right_index], sortBy, ascending, false)) {
            std::swap(tweets[left_index], tweets[right_index]);
            left_index--;
            right_index--;
        }
    }
}


void merge(std::vector<Tweet>& tweets, int left, int mid, int right, const std::string& sortBy, bool ascending) 
{
    int size_l = mid - left + 1; // size of the left subarray
    int size_r = right - mid; // size of the right subarray
    
    // O(n) space complexity
    std::vector<Tweet> left_tweets = create_subarray(tweets, left, mid + 1); // [left, mid]
    std::vector<Tweet> right_tweets = create_subarray(tweets, mid + 1, right + 1); // [mid+1, right]
    
    int i = 0; // Index for left subarray
    int j = 0; // Index for right subarray
    int index = left; // Index for original array

    // we know that left and right subarrays are sorted by themselves. place the values to the original array accordingly
    while (i < size_l && j < size_r) {
        if (shouldSwap(right_tweets[j], left_tweets[i], sortBy, ascending, true)) {
            tweets[index] = left_tweets[i];
            i++;
        } else {
            tweets[index] = right_tweets[j];
            j++;
        }
        index++;
    }
    // there can be elements left in the left or right subarray, we need to place them
    while (i < size_l) {
        tweets[index] = left_tweets[i];
        i++;
        index++;
    }

    while (j < size_r) {
        tweets[index] = right_tweets[j];
        j++;
        index++;
    }
}

// split the arrays, then uses the merge function to merge the splitted array in a sorted way.
void mergeSort(std::vector<Tweet>& tweets, int left, int right, const std::string& sortBy, bool ascending) 
{
    if (left >= right) { // base case, left > right invalid range, left == right is 1 sized array which is sorted by itself
        return;
    }
    int mid = left + (right - left) / 2;

    // Recursively split the left and right subarrays
    mergeSort(tweets, left, mid, sortBy, ascending);
    mergeSort(tweets, mid + 1, right, sortBy, ascending);

    // Merge left and right subarrays in a sorted way
    merge(tweets, left, mid, right, sortBy, ascending);
}
