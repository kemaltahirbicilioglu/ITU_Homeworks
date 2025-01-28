#include <iostream> 
#include <stdlib.h>
#include <iomanip>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

/////////////////// Player ///////////////////
class publisher
{
public:
    string name;
    float na_sales;
    float eu_sales;
    float others_sales;
};

/////////////////// Node ///////////////////
class Node{
public:
    publisher key;
    int color; // "Red"=1 or "Black"=0
    Node *parent, *left, *right;

    Node(publisher);  
    ~Node();     
    int get_color();
    void set_color(int);     
};

/////////////////// BST-Tree ///////////////////
class BST_tree {
private:
    Node* root;
public:    
    publisher* best_seller[3];    // to store top sellers in 3 categories
    stack<string> tree_deep_stack; // a stack to track traversal depth (used somewhere?)

    Node* get_root(); // returns the root of the tree

    Node* BST_insert(Node* root, Node* ptr);    // insert a node in the BST
    void insertValue(vector<string>);            // takes input data  adds it to tree
	Node* search_for_publisher(Node*, const string&); // for finding a publisher by name in tree
    void find_best_seller();                     // to find top sellers

    int in_order_traversal(Node*, int);            // traverse the tree to find best sellers, also return the maximum depth
    void delete_subtree(Node* node);              // helper function to free nodes (recursively), uses postorder traversal

    BST_tree();                                  // constructor
    ~BST_tree();                                 // destructor
};

// Function to write a vector of sorted publisher data back into a CSV file
void write_to_csv(const vector<vector<string>>& sorted_data, const string& filename) {
    ofstream out_file(filename, ios_base::out);
    if (!out_file.is_open()) {
        cerr << "Error: Could not open file for writing: " << filename << "\n";
        return;
    }

    // write the CSV header (column names)
    out_file << "Name,Platform,Year_of_Release,Publisher,NA_Sales,EU_Sales,Other_Sales\n";
    
    // write the sorted data into the CSV, line by line
    for (const auto& row : sorted_data) {
        for (size_t i = 0; i < row.size(); ++i) {
            out_file << row[i];
            if (i != row.size() - 1) {
                out_file << ","; // Add commas between values
            }
        }
        out_file << "\n";
    }

    out_file.close();
}

//prints the best sellers at the end of each year
void print_best_sellers(int year, publisher* temp_publisher[3]){
    cout.precision(5);
    cout << "End of the " + to_string(year) + " Year" << "\n";
    cout << "Best seller in North America: " + temp_publisher[0]->name + " - " << temp_publisher[0]->na_sales << " million" << "\n";
    cout << "Best seller in Europe: " + temp_publisher[1]->name + " - " << temp_publisher[1]->eu_sales << " million" << "\n";
    cout << "Best seller rest of the World: " + temp_publisher[2]->name + " - " << temp_publisher[2]->others_sales << " million" << "\n";
}

// helper function parses a csv row into a vector<string>
vector<string> extractPublisherData(const string& data) {
    stringstream ss(data);
    string temp;
    vector<string> publisher_data;

    getline(ss, temp, ','); // Name
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // Platform
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // Year_of_Release
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // Publisher
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // NA_Sales
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // EU_Sales
    publisher_data.push_back(temp);

    getline(ss, temp);      // Other_Sales
    publisher_data.push_back(temp);

    return publisher_data;
}

// generates a BST from a CSV file, line by line
BST_tree generate_BST_tree_from_csv(string file_name) {
    BST_tree temp_BSTree;
    ifstream file(file_name);
    string line;

    getline(file, line); // skip the header

    while (getline(file, line)) {
        vector<string> publisher_data = extractPublisherData(line);

        temp_BSTree.insertValue(publisher_data); // insert data into the BST
    }

    file.close();

    return temp_BSTree;
}

// function to generate the list of best-selling publishers and return publisher names vector, we will use it while randomly searching
vector<string> generate_list_of_best_selling(const string& fname, BST_tree& BSTree, const vector<int>& years_to_process) {
    ifstream file(fname);
    vector<string> publisher_names;

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fname << "\n";
        return publisher_names;
    }

    string line;
    getline(file, line); // skip the header line

    unsigned int current_year_index = 0;
    int year = 0;

    while (getline(file, line)) {
        vector<string> publisher_data = extractPublisherData(line);
        publisher_names.push_back(publisher_data[3]); // store publisher name

        year = stoi(publisher_data[2]); // convert year to integer

        // check if it's time to process data for specific years
        if (year > years_to_process[current_year_index]) {
            BSTree.find_best_seller();
            print_best_sellers(years_to_process[current_year_index], BSTree.best_seller);
            current_year_index++;
        }

        BSTree.insertValue(publisher_data); // insert the data into the BST
    }

    // ensure the best sellers for the last year are processed
    if (current_year_index <= years_to_process.size() - 1 && year >= years_to_process[current_year_index]) {
        BSTree.find_best_seller();
        print_best_sellers(years_to_process[current_year_index], BSTree.best_seller);
    }

    file.close();
    return publisher_names; // publishers in the dataset
}

// function to perform 50 random searches on publisher names
void publishers_random_search(const vector<string>& publisher_names, BST_tree& BSTree, int times) {
    for (int i = 0; i < times; i++) {
        int random_index = rand() % publisher_names.size();
        string publisher_to_search = publisher_names[random_index];
        Node* found_publisher = BSTree.search_for_publisher(BSTree.get_root(), publisher_to_search);

        // Because printing this also adds the runtime while measuring it, i commented it to be able to search only the searching time.
        if (found_publisher != nullptr) {
            //cout << "Publisher found: " << publisher_to_search << " EU Sales: " << found_publisher->key.eu_sales << "\n";
        } else {
            //cout << "Publisher not found: " << publisher_to_search << "\n";
        }
    }
}

BST_tree sort_data_by_publisher(const string& fname) {
    ifstream file(fname);
    string line;
    vector<vector<string>> all_data;

    // skip the header line
    getline(file, line);

    // read each line in the CSV file and extract publisher data
    while (getline(file, line)) {
        vector<string> publisher_data = extractPublisherData(line); 
        all_data.push_back(publisher_data); 
    }

    file.close(); 

    // sort the data by publisher name
    sort(all_data.begin(), all_data.end(), [](const vector<string>& a, const vector<string>& b) {
        return a[3] < b[3];
    });

    // write the sorted data to a new CSV file
    string sorted_filename = "sorted_" + fname;  
    write_to_csv(all_data, sorted_filename);

    //generate the BST from the sorted CSV file
    BST_tree sorted_BST = generate_BST_tree_from_csv(sorted_filename);

    return sorted_BST;
}

////////////////////////////////////////////
//----------------- MAIN -----------------//
////////////////////////////////////////////
int main(int argc, char* argv[]) {
    srand(time(0));

    if (argc < 2) {
        cerr << "invalid argument count \n";
        return EXIT_FAILURE;
    }

    string fname = argv[1]; // File name containing the data

    BST_tree BSTree;

    /////////////////// PRINTING BEST SELLING PUBLISHERS AND 50 RANDOM SEARCH //////////////////

    // function is generic, any years can be written in the years_to_process array (in an ascending order) 
    // to see the best selling publishers up to that year
    vector<int> years_to_process = {1990, 2000, 2010, 2020}; // years we want to process
    
    // finding best selling publishers for decades affect the duration time of insertion. 
    // generate_BST_tree_from_csv function can be used to measure the time 
    auto start_insertion = high_resolution_clock::now();
    vector<string> publisher_names = generate_list_of_best_selling(fname, BSTree, years_to_process);
    auto end_insertion = high_resolution_clock::now();

    auto insertion_duration = duration_cast<microseconds>(end_insertion - start_insertion);
    cout << "Time taken to insert all data into BST: " << insertion_duration.count() << " microseconds" << "\n";

    auto start_search = high_resolution_clock::now();
    int times = 50;
    publishers_random_search(publisher_names, BSTree, times);
    auto end_search = high_resolution_clock::now();

    auto search_duration = duration_cast<nanoseconds>(end_search - start_search);
    cout << "Average time for " << times << " random searches BST: " << search_duration.count() / times << " nanoseconds" << "\n";
    
    int BSTree_depth = BSTree.in_order_traversal(BSTree.get_root(), 0);
    cout << "BSTree_depth: " << BSTree_depth << "\n";
    /////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////// ORDERING THE DATA BY PUBLISHER NAME //////////////////
    BST_tree BSTree_sorted = sort_data_by_publisher(fname);
    int BSTree_sorted_depth = BSTree_sorted.in_order_traversal(BSTree_sorted.get_root(), 0);
    cout << "BSTree_sorted_depth: " << BSTree_sorted_depth << "\n";

    auto start_search_sorted = high_resolution_clock::now();
    publishers_random_search(publisher_names, BSTree_sorted, 50);
    auto end_search_sorted = high_resolution_clock::now();

    auto search_duration_sorted = duration_cast<nanoseconds>(end_search_sorted - start_search_sorted);
    cout << "Average time for " << times << " random searches BST SORTED: " << search_duration_sorted.count() / times << " nanoseconds" << "\n";
    /////////////////////////////////////////////////////////////////////////////////////////////

    return EXIT_SUCCESS;
}

/////////////////// Node ///////////////////
Node::Node(publisher key){
    this->key = key;
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
}

Node* BST_tree::get_root() {
    return this->root;
}

BST_tree::BST_tree(){
    this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
}

Node* BST_tree::BST_insert(Node* root, Node* ptr) {
    if (root == NULL) { // no node in the subtree
        return ptr; // if empty BST, new node becomes the root
    }

    if (ptr->key.name < root->key.name) {
        root->left = BST_insert(root->left, ptr); // go to the left subtree
        root->left->parent = root; // set parent for the left child
    } else if (ptr->key.name > root->key.name) {
        root->right = BST_insert(root->right, ptr); // go to right
        root->right->parent = root; // set parent for the right child
    } else {
        // already exists, update sales 
        root->key.na_sales += ptr->key.na_sales;
        root->key.eu_sales += ptr->key.eu_sales;
        root->key.others_sales += ptr->key.others_sales;
    
        //cout << "new values for the key: " << ptr->key.name << " = " << root->key.na_sales << "," << root->key.eu_sales << "," << root->key.others_sales << "\n"; 
    }

    return root; // maintain structure
}

void BST_tree::insertValue(vector<string> n) {
    if (n.size() < 7) {
        cerr << "Error: Insufficient data to insert.\n";
        return;
    }

    publisher newPublisher;
    newPublisher.name = n[3];           // Publisher
    newPublisher.na_sales = stof(n[4]); // NA_Sales
    newPublisher.eu_sales = stof(n[5]); // EU_Sales
    newPublisher.others_sales = stof(n[6]); // Other_Sales

    Node* newNode = new Node(newPublisher);

    this->root = BST_insert(this->root, newNode); // add new node to tree
}

// recursive function to search for a publisher, returns null if it does not exist
Node* BST_tree::search_for_publisher(Node* root, const string& publisher_name) {
    if (root == nullptr || root->key.name == publisher_name) {
        return root; // return node if found or null 
    }

    if (publisher_name < root->key.name) {
        return search_for_publisher(root->left, publisher_name); // check left
    }

    return search_for_publisher(root->right, publisher_name); // check right
}

void BST_tree::find_best_seller() {
    // clear the best_seller array.
    for (int i = 0; i < 3; i++) {
        best_seller[i] = nullptr; // reset previous best sellers.
    }

    // perform in-order traversal to update the best seller array and find the depth of the tree.
    int max_depth = in_order_traversal(root, 0);

    // output the maximum depth, this is good for comparing the BST and RBT as their complexity rely on the max depth
    //cout << "Maximum depth of the BST: " << max_depth << "\n";
}


int BST_tree::in_order_traversal(Node* node, int depth) {
    if (!node) return depth; // Return current depth if the node is null

    // Traverse the left subtree and determine its maximum depth
    int left_depth = in_order_traversal(node->left, depth + 1);

    publisher* pub = &(node->key); // pub is a publisher pointer which keeps the address of the current node's key
    // best seller array keeps publisher pointers.

    if (!best_seller[0] || pub->na_sales > best_seller[0]->na_sales) {
        best_seller[0] = pub; // Best in NA_Sales
    }

    if (!best_seller[1] || pub->eu_sales > best_seller[1]->eu_sales) {
        best_seller[1] = pub; // Best in EU_Sales
    }

    if (!best_seller[2] || pub->others_sales > best_seller[2]->others_sales) {
        best_seller[2] = pub; // Best in Other Other_Sales
    }

    // Traverse the right subtree and determine its maximum depth
    int right_depth = in_order_traversal(node->right, depth + 1);

    // Return the maximum depth encountered so far
    return max(left_depth, right_depth);
}


Node::~Node() {
    // nothing to clean up no dynamic allocation
}

void BST_tree::delete_subtree(Node* node) {
    if (!node) return;

    delete_subtree(node->left);
    delete_subtree(node->right);

    delete node; // clear the node itself
}

BST_tree::~BST_tree() {
    delete_subtree(this->root); // clear all nodes
    this->root = NULL;
}
