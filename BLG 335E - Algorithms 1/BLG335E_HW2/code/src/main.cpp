#include "methods.h"
// I TESTED MY CODE WITH STD_STABLE SORT. SINCE THE HEADERS DOES NOT HAVE THAT, I COMMENTED ALL THE TEST CODE I WROTE FOR THE HOMEWORK
// IF YOU WANT TO SEE HOW MY TESTING FUNCTIONS ARE WORKING, ALGORITHM AND CHRONO LIBRARIES SHOULD BE INCLUDED IN THE HEADER

// TEST CODES START

// //checks if items are equal, this requires stable sort
// bool items_equal(const Item &item1, const Item &item2)
// {
//     if(item1.age != item2.age)
//         return false;
//     if(item1.type != item2.type)
//         return false;
//     if(item1.origin != item2.origin)
//         return false;
        
//     return true;
// }

// // checks if both arrays are equal
// bool check_arrays(const std::vector<Item> &item1, const std::vector<Item> &item2)
// {
//     if(item1.size() != item2.size()){
//         std::cerr << "size is not the same\n";        
//         return false;
//     }

//     bool ans = true;
//     int size = item1.size();
//     for(int i = 0; i < size; i++){
//         if(!items_equal(item1[i], item2[i])){
//             std::cout << "LINE " << i << " IS INCORRECT\n";
//             ans = false;
//             break;
//         }
//     }

//     return ans;
// }

// // sorts the array according to the given sorting algorithm and other parameters. writes in output/my_sorted.csv
// std::vector<Item> sort_my_array(std::vector<Item> &items, const std::string sorting_algorithm, const std::string sortBy, bool ascending)
// {
//     auto start_time = std::chrono::high_resolution_clock::now();
//     std::vector<Item> sorted_items;
//     if (sorting_algorithm == "countingSort") 
//         sorted_items = countingSort(items, sortBy, ascending);
//     else if (sorting_algorithm == "heapSortByRarity")
//         sorted_items = heapSortByRarity(items, !ascending);

//     auto end_time = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
//     std::cerr << sorting_algorithm << ": (" << duration << "ms)\n"; 
//     writeItemsToFile("output/my_sorted.csv", sorted_items);
//     return sorted_items;
// }

// // sorts items using std::sort(), writes in output/std_sort.csv, also outputs the running time.
// std::vector<Item> sort_in_std(std::vector<Item> &items, const std::string& sortBy, bool ascending){
//     auto start_time = std::chrono::high_resolution_clock::now();

//     std::stable_sort(items.begin(), items.end(), [&](const Item& a, const Item& b) {
//         if (sortBy == "age") {
//             return ascending ? (a.age < b.age) : (a.age > b.age);
//         } 
//         else if (sortBy == "type") {
//             return ascending ? (a.type < b.type) : (a.type > b.type);
//         } 
//         else if (sortBy == "origin") {
//             return ascending ? (a.origin < b.origin) : (a.origin > b.origin);
//         } 
//         else if (sortBy == "rarityScore") {
//             return ascending ? (a.rarityScore < b.rarityScore) : (a.rarityScore > b.rarityScore);
//         } 
//         // If no valid sort field, don't change the order
//         return false;
//     });

//     auto end_time = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
//     std::cerr << "("<< duration << "ms) " << "Item vector is STD::sorted by " << sortBy << " in " << (ascending ? "ascending" : "descending") << " order\n";
//     writeItemsToFile("output/std_sort.csv", items);
//     return items;
// }

// // call this to check whether the sorting algorithm is working correctly, it will check with the std::stable_sort().
// void util_test_sorting(const std::string input_file, const std::string sorting_algorithm, const std::string& sortBy, bool ascending)
// {
//     // first create the expected result from the input file.
//     std::vector<Item> items = readItemsFromFile(input_file);
//     std::vector<Item> sorted_std = sort_in_std(items, sortBy, ascending);
//     std::vector<Item> sorted_my = sort_my_array(items, sorting_algorithm, sortBy, ascending);

//     if(check_arrays(sorted_std, sorted_my)){
//         std::cerr << input_file << " is CORRECTLY sorted by " << sortBy << " in " << (ascending ? "ascending" : "descending") << "order\n";
//     }else{
//         std::cerr << input_file << " CANNOT be sorted by " << sortBy << " in " << (ascending ? "ascending" : "descending") << "order\n";
//     }
// }

// TEST CODES END

int main() 
{   
    //util_test_sorting("data/items_s.csv", "countingSort", "age", true); // enough to check if counting sort is working
    
    // std::vector<Item> items = readItemsFromFile("data/items_s.csv");
    // std::vector<Item> sorted_items = sort_my_array(items, "countingSort", "age", true);
    // auto start_time = std::chrono::high_resolution_clock::now();
    // calculateRarityScores(items);
    // auto end_time = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    // std::cerr << "time for rarity:" << "("<< duration << "ms) " << "\n";
    // std::vector<Item> heap_sorted_items = sort_my_array(items, "heapSortByRarity", "rarityScore", true);
    // std::vector<Item> std_sorted = sort_in_std(items, "rarityScore", true);
    // std::cerr << check_arrays(std_sorted, heap_sorted_items) << "\n";

  // std::vector<Item> items = readItemsFromFile("data/items_l.csv");
  // std::vector<Item> sorted = countingSort(items, "age", true);
  // writeItemsToFile("data/items_l_sorted.csv", sorted);
  // calculateRarityScores(sorted, 50);
  // writeItemsToFile("data/items_l_rarity.csv", sorted);
  // sorted = heapSortByRarity(sorted, true);
  // writeItemsToFile("data/items_l_rarity_sorted.csv", sorted);

  return 0;
}