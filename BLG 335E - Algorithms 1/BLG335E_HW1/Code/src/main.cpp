#include "tweet.h"
#include <algorithm> 
#include <chrono>

// TEST CODES START

// checks if Tweets are equal, this requires stable sort
bool tweets_equal(const Tweet &t1, const Tweet &t2)
{
    if(t1.tweetID != t2.tweetID)
        return false;
    if(t1.retweetCount != t2.retweetCount)
        return false;
    if(t1.favoriteCount != t2.favoriteCount)
        return false;

    return true;
}

// checks if both arrays are equal
bool check_arrays(const std::vector<Tweet> &t1, const std::vector<Tweet> &t2)
{
    if(t1.size() != t2.size()){
        std::cerr << "size is not the same\n";        
        return false;
    }

    bool ans = true;
    int size = t1.size();
    for(int i = 0; i < size; i++){
        if(!tweets_equal(t1[i], t2[i])){
            std::cout << "LINE " << i << " IS INCORRECT\n";
            ans = false;
            break;
        }
             
    }

    return ans;
}

// sorts the array according to the given sorting algoririthm and other parameters. writes in output/my_sorted.csv
void sort_my_array(std::vector<Tweet> &tweets, const std::string sorting_algorithm, const std::string sortBy, bool ascending)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    if (sorting_algorithm == "bubbleSort") 
        bubbleSort(tweets, sortBy, ascending);
    else if (sorting_algorithm == "insertionSort")
        insertionSort(tweets, sortBy, ascending);
    else if (sorting_algorithm == "mergeSort")
        mergeSort(tweets, 0, tweets.size() - 1, sortBy, ascending);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cerr << sorting_algorithm << ": (" << duration << "ms)\n"; 
    writeTweetsToFile("output/my_sorted.csv", tweets);
}

// sorts tweets using std::sort(), writes in output/std_sort.csv, also outputs the running time.
void sort_in_std(std::vector<Tweet> &tweets, const std::string& sortBy, bool ascending){
    auto start_time = std::chrono::high_resolution_clock::now();

    std::stable_sort(tweets.begin(), tweets.end(), [&](const Tweet& a, const Tweet& b) {
        if (sortBy == "tweetID") {
            return ascending ? (a.tweetID < b.tweetID) : (a.tweetID > b.tweetID);
        } 
        else if (sortBy == "retweetCount") {
            return ascending ? (a.retweetCount < b.retweetCount) : (a.retweetCount > b.retweetCount);
        } 
        else if (sortBy == "favoriteCount") {
            return ascending ? (a.favoriteCount < b.favoriteCount) : (a.favoriteCount > b.favoriteCount);
        } 
        // If no valid sort field, don't change the order
        return false;
    });
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cerr << "("<< duration << "ms) " << "tweet vector is STD::sorted by " << sortBy << " in " << (ascending ? "ascending" : "descending") << " order\n";
    writeTweetsToFile("output/std_sort.csv", tweets);
}

// to be able to use std::binary_search 
std::vector<int> tweetID_only(const std::vector<Tweet>& tweets)
{
    int size = tweets.size();
    std::vector<int> tweetIDs(size, 0);
    for(int i = 0; i < size; i++){
        tweetIDs[i] = tweets[i].tweetID;
    }
    return tweetIDs;
}

// serachs tweetIds using std::binary_search(), return the index found. Also outputs the running time.
int binary_search_in_std(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy)
{
    std::vector<int> new_array = tweetID_only(tweets);    
    auto start_time = std::chrono::high_resolution_clock::now();
    // First, check if the key exists using std::binary_search
    bool exists = std::binary_search(new_array.begin(), new_array.end(), key);

    int index = -1;
    if (exists) {
        // If found, use std::lower_bound to get the index of the key
        auto it = std::lower_bound(new_array.begin(), new_array.end(), key);
        index = std::distance(new_array.begin(), it); // Get the index
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cerr << "(" << duration << "ms) " << "STD::binary search with tweetID\n";
    return index;
}

// call this to check whether the sorting algorithm is working correctly, it will check with the std::stable_sort().
void util_test_sorting(const std::string input_file, const std::string sorting_algorithm, const std::string& sortBy, bool ascending)
{
    // first create the expected result from the input file.
    std::vector<Tweet> tweets_std = readTweetsFromFile(input_file);
    std::vector<Tweet> tweets_my = readTweetsFromFile(input_file);
    sort_in_std(tweets_std, sortBy, ascending);
    sort_my_array(tweets_my, sorting_algorithm, sortBy, ascending);

    if(check_arrays(tweets_std, tweets_my)){
        std::cerr << input_file << " is CORRECTLY sorted by " << sortBy << " in " << (ascending ? "ascending" : "descending") << "order\n";
    }else{
        std::cerr << input_file << " CANNOT be sorted by " << sortBy << " in " << (ascending ? "ascending" : "descending") << "order\n";
    }
}

// call this to check whether the binarySearch algorithm is working correclty, it will check with the std::binary_search().
void util_test_metrics_binary_search(const std::vector<Tweet>& tweets, long long key, const std::string& sortBy)
{
    int std_index = binary_search_in_std(tweets, key, sortBy);

    auto start_time = std::chrono::high_resolution_clock::now();

    int my_index = binarySearch(tweets, key, sortBy);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    if(std_index == my_index){
        std::cerr << "("<< duration << "ms) " << "key of " << sortBy << ": " << key << " is CORRECTLY found in index " << my_index << "\n";
    }else{
        std::cerr << "("<< duration << "ms) " << "key of " << sortBy << ": " << key << " CANNOT be found\n";
    }
}

// call this to check the value of the above threshold.
void util_test_metrics_count_above(const std::string input_file, const std::string& metric, int threshold) 
{
    std::vector<Tweet> tweets = readTweetsFromFile(input_file);

    auto start_time = std::chrono::high_resolution_clock::now();
    int ans = countAboveThreshold(tweets, metric, threshold);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cerr << "("<< duration << "ms) " << "thereshold " << threshold << ": " << ans << "\n";
}

// TEST CODES END

std::string selectSorting()
{
    char selectSorting;
    std::string answer;
    std::cout << "which algorithm:\n (a) bubbleSort \t (b) insertionSort \t (c) mergeSort: ";
    std::cin >> selectSorting;
    if(selectSorting != 'a' && selectSorting != 'b' && selectSorting != 'c'){
        std::cerr << "invalid algorithm selection\n";
        return answer;
    }
    switch (selectSorting)
    {
    case 'a':
        answer = "bubbleSort";
        break;
    case 'b':
        answer = "insertionSort";
        break;
    case 'c':
        answer = "mergeSort";
        break;
    default:
        break;
    }
    return answer;
}

// creates the file name desired from the choices
std::string inputFileSelector(int choice)
{
    std::string input_file_name = "tweets";
    switch (choice)
    {
    case 11:
        input_file_name = input_file_name;
        break;
    case 12:
        input_file_name += "NS";
        break;
    case 13:
        input_file_name += "SA";
        break;
    case 14:
        input_file_name += "SD";
        break;
    case 21:
        input_file_name += "5K";
        break;
    case 22:
        input_file_name += "10K";
        break;
    case 23:
        input_file_name += "20K";
        break;
    case 24:
        input_file_name += "30K";
        break;
    case 25:
        input_file_name += "40K";
        break;
    case 26:
        input_file_name += "50K";
        break;
    default:
        break;
    }
    input_file_name += ".csv";
    
    return input_file_name;
}


std::string selectInputFile()
{
    std::string input_file_name;
    int choice1 = -1;
    int choice2 = -1;
    std::cout << "select dataset:\n (1) permutations \t (2) sizes: ";
    std::cin >> choice1;
    if (choice1 == 1) {
        std::cout << "select dataset:\n (11) tweets \t (12) tweetsNS \t (13) tweetsSA \t (14) tweetsSD: ";
        std::cin >> choice2;
    }
    else if (choice1 == 2) {
        std::cout << "select dataset:\n (21) 5K \t (22) 10K \t (23) 20K \t (24) 30K \t (25) 40K: \t (26) 50K: ";
        std::cin >> choice2;
    }
    else {
        std::cerr << "invalid selection\n";
        return input_file_name;
    }
    input_file_name = choice1 == 1 ? "data/permutations/" : "data/sizes/";
    input_file_name += inputFileSelector(choice2);
    return input_file_name;
}


std::string selectSortBy()
{
    std::string sortBy;
    std::cout << "sortBy: ";
    std::cin >> sortBy;
    return sortBy;
}


bool selectAscending()
{
    bool ascending = true;
    std::cout << "(1) ascending \t (0) descending: ";
    std::cin >> ascending;
    return ascending;
}


int selectKey()
{
    int key;
    std::cerr << "give key to search: ";
    std::cin >> key;
    return key;
}

// sorts all of the datasets using all of the sorting algorithms and compares them with the std::stable_sort().
void runForAll()
{
    std::vector<std::string> sorting_algorithms = {"bubbleSort", "insertionSort", "mergeSort"}; // all sortings
    std::vector<int> input_file_names = {11, 12, 13, 14, 21, 22, 23, 24, 25, 26 }; // all csvs: 11, 12, 13, 14, 21, 22, 23, 24, 25, 26 
    std::vector<std::string> sort_bys = {"tweetID", "retweetCount", "favoriteCount"}; // "tweetID", "retweetCount", "favoriteCount"
    
    for(const std::string& sorting_algorithm : sorting_algorithms){
        for(const int& input_file_num : input_file_names){
            std::string input_file_name = input_file_num / 10 == 1 ? "data/permutations/" : "data/sizes/";
            input_file_name.append(inputFileSelector(input_file_num));   
            for(const std::string& sort_by : sort_bys){
                util_test_sorting(input_file_name, sorting_algorithm, sort_by, true); // in ascending order
                //util_test_sorting(input_file_name, sorting_algorithm, sort_by, false); // in descending order
            }
        }
    }
}

int main() 
{
    runForAll();

    /*
     further comment lines can be used to test any specific function 
    */

    // std::cerr << "test sorting:\n";

    // std::string sorting_algorithm = selectSorting();
    // std::string input_file_name_sort = selectInputFile();
    // std::string sortBy = selectSortBy();
    // bool ascending = selectAscending();

    // util_test_sorting(input_file_name_sort, sorting_algorithm, sortBy, ascending);

    // std::cerr << "test binary_search\n";
    // std::string input_file_name_search = selectInputFile();
    // std::vector<Tweet> tweets = readTweetsFromFile(input_file_name_search);

    // sort_my_array(tweets, "mergeSort", "tweetID", true); // tweetIDs are unique, binary search is meaningful just for them
    // writeTweetsToFile("../output/output.csv", tweets); // to be able to find a key to binary search
    // int key = selectKey();
    // util_test_metrics_binary_search(tweets, key, "tweetID");


    // std::cerr << "test count above threshold\n";
    // std::string input_file_name_threshold = selectInputFile();

    // util_test_metrics_count_above(input_file_name_threshold, "favoriteCount", 250);

    return 0;
}
