#ifndef ITEM_TESTS_H
#define ITEM_TESTS_H

#include "munit.h"
#include "methods.h"
#include <stdlib.h>
#include <algorithm>

extern std::vector<Item> readFromTxt(std::string path);
// Helper function to get dataset of items for testing
extern std::vector<Item> getSampleItems();
// Helper function to get age sorted dataset of items for testing
extern std::vector<Item> getAgeSortedItems();
// Helper function to get age sorted and rarity score calculated dataset of items for testing
extern std::vector<Item> getRarityCalculatedItems();
// Helper function to get rarirty sorted dataset of items for testing
extern std::vector<Item> getRaritySortedItems();

struct TestItems
{
  std::vector<Item> items;
  std::vector<Item> items_age_sorted;
  std::vector<Item> items_rarity_calculated;
  std::vector<Item> items_rarity_sorted;
};

// Test to find max element given attribute
static MunitResult test_get_max(const MunitParameter params[], void* data) {
  TestItems* test_items = (TestItems*)data;
  std::vector<Item> items = test_items->items;

  auto maxAgeItem = std::max_element(items.begin(), items.end(),
          [](const Item& a, const Item& b) { return a.age < b.age; });

  int maxVal = getMax(items, "age");
  
  munit_assert_int(maxVal, ==, maxAgeItem->age);

  return MUNIT_OK;
}

// Test for Counting Sort in ascending order
static MunitResult test_counting_sort_age_ascending(const MunitParameter params[], void* data) {
  TestItems* test_items = (TestItems*)data;
  std::vector<Item> items = test_items->items;
  std::vector<Item> sorted_items = test_items->items_age_sorted;

  std::vector<Item> sorted = countingSort(items, "age", true);
  
  int itemSize = items.size();
  // Verify the order of the age after sorting (ascending)
  for(int i = 0; i < itemSize; i++)
  {
    munit_assert_int(sorted[i].age, ==, sorted_items[i].age);
  }

  return MUNIT_OK;
}

// Test for Counting Sort in descending order
static MunitResult test_counting_sort_age_descending(const MunitParameter params[], void* data) {
  TestItems* test_items = (TestItems*)data;
  std::vector<Item> items = test_items->items;
  std::vector<Item> sorted_items = test_items->items_age_sorted;

  std::vector<Item> sorted = countingSort(items, "age", false);
  
  int itemSize = items.size();
  // Verify the order of the age after sorting (ascending)
  for(int i = 0; i < itemSize; i++)
  {
    munit_assert_int(sorted[i].age, ==, sorted_items[itemSize - 1 - i].age);
  }

  return MUNIT_OK;
}

bool checkAlmostEqual(double a, double b, int i=0, double epsilon = 1e-5) {
    double diff = std::abs(a - b);
    if (diff > epsilon){
        std :: cout << "a: " << a << " b: " << b << " diff: " << diff <<  " i: " << i << std::endl;
    }
    return diff <= epsilon;
}

// Test for Rarity Score Calculation
static MunitResult test_calculate_rarity_scores(const MunitParameter params[], void* data) {
  TestItems* test_items = (TestItems*)data;
  std::vector<Item> items = test_items->items; 
  std::vector<Item> items_rarity_calculated = test_items->items_rarity_calculated;

  std::vector<Item> sorted = countingSort(items, "age", true);
  calculateRarityScores(sorted, 50);
  
  int itemSize = items.size();
  // Verify the order of the age after sorting (ascending)
  for(int i = 0; i < itemSize; i++)
  {
    munit_assert(checkAlmostEqual(sorted[i].rarityScore, items_rarity_calculated[i].rarityScore, i));
  }

  return MUNIT_OK;
}

// Test for Heap Sort in descending order
static MunitResult test_heap_sort_rarity_descending(const MunitParameter params[], void* data) {
  TestItems* test_items = (TestItems*)data;
  std::vector<Item> items = test_items->items_rarity_calculated;
  std::vector<Item> sorted_items = test_items->items_rarity_sorted;

  std::vector<Item> sorted = heapSortByRarity(items, true);
  
  int itemSize = items.size();
  // Verify the order of the rarityScore after sorting
  for(int i = 0; i < itemSize; i++)
  {
    munit_assert(checkAlmostEqual(sorted[i].rarityScore, sorted_items[i].rarityScore, i));
  }

  return MUNIT_OK;
}

// Test for Heap Sort in ascending order
static MunitResult test_heap_sort_rarity_ascending(const MunitParameter params[], void* data) {
  TestItems* test_items = (TestItems*)data;
  std::vector<Item> items = test_items->items_rarity_calculated;
  std::vector<Item> sorted_items = test_items->items_rarity_sorted;

  std::vector<Item> sorted = heapSortByRarity(items, false);
  
  int itemSize = items.size();
  // Verify the order of the age after sorting 
  for(int i = 0; i < itemSize; i++)
  {
    munit_assert(checkAlmostEqual(sorted[i].rarityScore, sorted_items[itemSize - 1 - i].rarityScore, itemSize - 1 - i));
  }

  return MUNIT_OK;
}

// Setup function for initializing TestItems
static void* setup_test_items(const MunitParameter params[], void* user_data) {
    TestItems* test_items = new TestItems{
        getSampleItems(),
        getAgeSortedItems(),
        getRarityCalculatedItems(),
        getRaritySortedItems()
    };
    return test_items;
}

// Clean up function for TestItems
static void teardown_test_items(void* data) {
    delete static_cast<TestItems*>(data);
}

// Parameters for the tests (none needed for these)
static MunitTest itemTests[] = {
  { (char*) "/get_max", test_get_max, setup_test_items, teardown_test_items, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/counting_sort_age_ascending", test_counting_sort_age_ascending, setup_test_items, teardown_test_items, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/counting_sort_age_descending", test_counting_sort_age_descending, setup_test_items, teardown_test_items, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/calculate_rarity_scores", test_calculate_rarity_scores, setup_test_items, teardown_test_items, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/heap_sort_rarity_descending", test_heap_sort_rarity_descending, setup_test_items, teardown_test_items, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/heap_sort_rarity_ascending", test_heap_sort_rarity_ascending, setup_test_items, teardown_test_items, MUNIT_TEST_OPTION_NONE, NULL },
  
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

// Define the test suite
const MunitSuite itemTestSuite = {
  (char*) "/ItemTests", 
  itemTests, 
  NULL, 
  1, 
  MUNIT_SUITE_OPTION_NONE 
};

#endif
