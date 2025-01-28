#ifndef TWEET_TESTS_H
#define TWEET_TESTS_H

#include "munit.h"
#include "tweet.h"
#include <stdlib.h>

// Helper function to create a sample dataset of tweets for testing
std::vector<Tweet> createSampleTweets() {
    std::vector<Tweet> tweets = {
        {12345, 50, 100},
        {56789, 20, 200},
        {1773335, 70, 150},
        {98765, 90, 250},
        {54321, 10, 50}
    };
    return tweets;
}

// Test for Bubble Sort in ascending order
static MunitResult test_bubble_sort_ascending(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();
    bubbleSort(tweets, "retweetCount", true);

    // Verify the order of the retweetCount after sorting (ascending)
    munit_assert_int(tweets[0].retweetCount, ==, 10);
    munit_assert_int(tweets[1].retweetCount, ==, 20);
    munit_assert_int(tweets[2].retweetCount, ==, 50);
    munit_assert_int(tweets[3].retweetCount, ==, 70);
    munit_assert_int(tweets[4].retweetCount, ==, 90);

    return MUNIT_OK;
}

// Test for Bubble Sort in descending order
static MunitResult test_bubble_sort_descending(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();
    bubbleSort(tweets, "retweetCount", false);

    // Verify the order of the retweetCount after sorting (descending)
    munit_assert_int(tweets[0].retweetCount, ==, 90);
    munit_assert_int(tweets[1].retweetCount, ==, 70);
    munit_assert_int(tweets[2].retweetCount, ==, 50);
    munit_assert_int(tweets[3].retweetCount, ==, 20);
    munit_assert_int(tweets[4].retweetCount, ==, 10);

    return MUNIT_OK;
}

// Test for Insertion Sort in ascending order
static MunitResult test_insertion_sort_ascending(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();
    insertionSort(tweets, "favoriteCount", true);

    // Verify the order of the favoriteCount after sorting (ascending)
    munit_assert_int(tweets[0].favoriteCount, ==, 50);
    munit_assert_int(tweets[1].favoriteCount, ==, 100);
    munit_assert_int(tweets[2].favoriteCount, ==, 150);
    munit_assert_int(tweets[3].favoriteCount, ==, 200);
    munit_assert_int(tweets[4].favoriteCount, ==, 250);

    return MUNIT_OK;
}

// Test for Insertion Sort in descending order
static MunitResult test_insertion_sort_descending(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();
    insertionSort(tweets, "favoriteCount", false);

    // Verify the order of the favoriteCount after sorting (descending)
    munit_assert_int(tweets[0].favoriteCount, ==, 250);
    munit_assert_int(tweets[1].favoriteCount, ==, 200);
    munit_assert_int(tweets[2].favoriteCount, ==, 150);
    munit_assert_int(tweets[3].favoriteCount, ==, 100);
    munit_assert_int(tweets[4].favoriteCount, ==, 50);

    return MUNIT_OK;
}

// Test for Merge Sort in ascending order
static MunitResult test_merge_sort_ascending(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();
    mergeSort(tweets, 0, tweets.size() - 1, "tweetID", true);

    // Verify the order of the tweetID after sorting (ascending)
    munit_assert_int(tweets[0].tweetID, ==, 12345);
    munit_assert_int(tweets[1].tweetID, ==, 54321);
    munit_assert_int(tweets[2].tweetID, ==, 56789);
    munit_assert_int(tweets[3].tweetID, ==, 98765);
    munit_assert_int(tweets[4].tweetID, ==, 1773335);

    return MUNIT_OK;
}

// Test for Merge Sort in descending order
static MunitResult test_merge_sort_descending(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();
    mergeSort(tweets, 0, tweets.size() - 1, "tweetID", false);

    // Verify the order of the tweetID after sorting (descending)
    munit_assert_int(tweets[0].tweetID, ==, 1773335);
    munit_assert_int(tweets[1].tweetID, ==, 98765);
    munit_assert_int(tweets[2].tweetID, ==, 56789);
    munit_assert_int(tweets[3].tweetID, ==, 54321);
    munit_assert_int(tweets[4].tweetID, ==, 12345);

    return MUNIT_OK;
}

// Test for Binary Search
static MunitResult test_binary_search(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();
    
    // The dataset is not sorted yet, so let's sort it by tweetID before performing binary search
    mergeSort(tweets, 0, tweets.size() - 1, "tweetID", true);

    // Perform Binary Search for tweetID 1773335
    int index = binarySearch(tweets, 1773335, "tweetID");

    // Verify if the correct tweet was found
    munit_assert_int(index, !=, -1);
    munit_assert_int(tweets[index].tweetID, ==, 1773335);

    return MUNIT_OK;
}

// Test for counting tweets with favorite count above 50
static MunitResult test_count_above_threshold(const MunitParameter params[], void* data) {
    std::vector<Tweet> tweets = createSampleTweets();

    // Perform the count for favorite count above 50
    int count = countAboveThreshold(tweets, "favoriteCount", 50);

    // Verify the result
    munit_assert_int(count, ==, 4);  // 4 tweets have favoriteCount > 50

    return MUNIT_OK;
}


// Parameters for the tests (none needed for these)
static MunitTest tweetTests[] = {
    { (char*) "/bubble_sort_ascending", test_bubble_sort_ascending, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/bubble_sort_descending", test_bubble_sort_descending, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/insertion_sort_ascending", test_insertion_sort_ascending, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/insertion_sort_descending", test_insertion_sort_descending, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/merge_sort_ascending", test_merge_sort_ascending, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/merge_sort_descending", test_merge_sort_descending, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/binary_search", test_binary_search, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/count_above_threshold", test_count_above_threshold, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

// Define the test suite
const MunitSuite tweetTestSuite = {
    (char*) "/TweetTests", 
    tweetTests, 
    NULL, 
    1, 
    MUNIT_SUITE_OPTION_NONE 
};

#endif
