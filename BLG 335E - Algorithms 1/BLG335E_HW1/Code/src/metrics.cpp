#include "tweet.h"

// compares tweets[index] with key, equals = 0, smaller key = -1, larger key = 1 
int checkKey(const std::vector<Tweet>& tweets, long long key, const int index, const std::string& sortBy)
{
    if (sortBy == "tweetID") {
        if (key == tweets[index].tweetID) return 0; // key is found
        else if (key < tweets[index].tweetID) return -1; // key is smaller
        else if (key > tweets[index].tweetID) return 1; // key is larger
    }
    else if (sortBy == "retweetCount") {
        if (key == tweets[index].retweetCount) return 0; // key is found
        else if (key < tweets[index].retweetCount) return -1; // key is smaller
        else if (key > tweets[index].retweetCount) return 1; // key is larger
    }
    else if (sortBy == "favoriteCount") {
        if (key == tweets[index].favoriteCount) return 0; // key is found
        else if (key < tweets[index].favoriteCount) return -1; // key is smaller
        else if (key > tweets[index].favoriteCount) return 1; // key is larger
    }
    return -2; // program should not come here, invalid sortBy input 
}

// array is assumed to be sorted, returns the index of the key in the array, if does not exist returns -1
int binarySearch(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy)
{
    // array is assumed to be sorted already
    int left = 0;
    int right = tweets.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; //not to get overflow when left + right passes the integer range.
        int result = checkKey(tweets, key, mid, sortBy);
        if (result == 0) return mid; // 0 means success, key is found
        else if (result == -1) right = mid - 1; // -1 means failure, key is smaller than the mid element.
        else if (result == 1) left = mid + 1; // 1 means failure, key is larger than the mid element.
        else{
            std::cerr << "invalid check_key return: " << "(" << result << ")\n";
            break;
        }
    }
    return - 1; // key is not in the tweets
}

// how can I write a test code for this? Ask your friends for the answer of 250 threshold or sort and see the first index under threshold
int countAboveThreshold(const std::vector<Tweet>& tweets, const std::string& metric, int threshold) 
{
    int size = tweets.size();
    int count = 0;
    for (int i = 0; i < size; i++) {
        int result = checkKey(tweets, threshold, i, metric); // if threshold < tweets[i] , it will return -1
        if (result == -1) count++; // if current element is greater than the threshold count.         
    }

    return count;
}
