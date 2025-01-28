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

/////////////////// RB-Tree ///////////////////
class RB_tree{
	private:
		Node* root;
	public:	
        publisher* best_seller[3];	// to store top sellers in 3 categories
		stack<string> tree_deep_stack;  // a stack to track traversal depth, ı used stack but not this stack for iterative preorder

        Node* get_root();   // returns the root of the tree

		Node* RB_insert(Node* root, Node* ptr); // insert a node in the RBT
        void insertValue(vector<string>);   // takes input data  adds it to tree
        Node* search_for_publisher(Node*, const string&);   // for finding a publisher by name in tree
        void find_best_seller();    // to find top sellers
		void RB_left_rotate(Node*); // utility function for fixup function
		void RB_right_rotate(Node*);    // utility function for fixup function
		void RB_insert_fixup(Node*); // call this after insertion to balance the tree
        void preorder();    // print the tree in preorder
        int in_order_traversal(Node*, int); // traverse the tree to find best sellers, also return the maximum depth
        RB_tree(); // constructor
        void deleteSubtree(Node* node); // helper function to free nodes (recursively), uses postorder traversal
		~RB_tree(); // destructor
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

// generates a RBT from a CSV file, line by line
RB_tree generate_RB_tree_from_csv(string file_name) {
    RB_tree temp_RBTree;
    ifstream file(file_name);
    string line;

    getline(file, line); // skip the header

    while (getline(file, line)) {
        vector<string> publisher_data = extractPublisherData(line);

        temp_RBTree.insertValue(publisher_data); // insert data into the RBT
    }

    file.close();

    return temp_RBTree;
}

// function to generate the list of best-selling publishers and return publisher names vector, we will use it while randomly searching
vector<string> generate_list_of_best_selling(const string& fname, RB_tree& RBTree, const vector<int>& years_to_process) {
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
            RBTree.find_best_seller();
            print_best_sellers(years_to_process[current_year_index], RBTree.best_seller);
            current_year_index++;
        }

        RBTree.insertValue(publisher_data); // insert the data into the RBT
    }

    // ensure the best sellers for the last year are processed
    if (current_year_index <= years_to_process.size() - 1 && year >= years_to_process[current_year_index]) {
        RBTree.find_best_seller();
        print_best_sellers(years_to_process[current_year_index], RBTree.best_seller);
    }

    file.close();
    return publisher_names; // publishers in the dataset
}

// function to perform 50 random searches on publisher names
void publishers_random_search(const vector<string>& publisher_names, RB_tree& RBTree, int times) {
    for (int i = 0; i < times; i++) {
        int random_index = rand() % publisher_names.size();
        string publisher_to_search = publisher_names[random_index];
        Node* found_publisher = RBTree.search_for_publisher(RBTree.get_root(), publisher_to_search);

        // Because printing this also adds the runtime while measuring it, i commented it to be able to search only the searching time.
        if (found_publisher != nullptr) {
            //cout << "Publisher found: " << publisher_to_search << " EU Sales: " << found_publisher->key.eu_sales << "\n";
        } else {
            //cout << "Publisher not found: " << publisher_to_search << "\n";
        }
    }
}

RB_tree sort_data_by_publisher(const string& fname) {
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

    //generate the RBT from the sorted CSV file
    RB_tree sorted_RBT = generate_RB_tree_from_csv(sorted_filename);

    return sorted_RBT;
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

    RB_tree RBTree;

    /////////////////// PRINTING BEST SELLING PUBLISHERS AND 50 RANDOM SEARCH //////////////////

    // function is generic, any years can be written in the years_to_process array (in an ascending order) 
    // to see the best selling publishers up to that year
    vector<int> years_to_process = {1990, 2000, 2010, 2020}; // years we want to process
    
    // finding best selling publishers for decades affect the duration time of insertion. 
    // generate_RB_tree_from_csv function can be used to measure the time 
    auto start_insertion = high_resolution_clock::now();
    vector<string> publisher_names = generate_list_of_best_selling(fname, RBTree, years_to_process);
    auto end_insertion = high_resolution_clock::now();

    auto insertion_duration = duration_cast<microseconds>(end_insertion - start_insertion);
    cout << "Time taken to insert all data into RBT: " << insertion_duration.count() << " microseconds" << "\n";

    auto start_search = high_resolution_clock::now();
    int times = 50;
    publishers_random_search(publisher_names, RBTree, times);
    auto end_search = high_resolution_clock::now();

    auto search_duration = duration_cast<nanoseconds>(end_search - start_search);
    cout << "Average time for " <<  times << " random searches RBT: " << search_duration.count() / times << " nanoseconds" << "\n";
    
    int RBTree_depth = RBTree.in_order_traversal(RBTree.get_root(), 0);
    cout << "RBTree_depth: " << RBTree_depth << "\n";
    /////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////// ORDERING THE DATA BY PUBLISHER NAME //////////////////
    RB_tree RBTree_sorted = sort_data_by_publisher(fname);
    int RBTree_sorted_depth = RBTree_sorted.in_order_traversal(RBTree_sorted.get_root(), 0);
    cout << "RBTree_sorted_depth: " << RBTree_sorted_depth << "\n";

    auto start_search_sorted = high_resolution_clock::now();
    publishers_random_search(publisher_names, RBTree_sorted, times);
    auto end_search_sorted = high_resolution_clock::now();

    auto search_duration_sorted = duration_cast<nanoseconds>(end_search_sorted - start_search_sorted);
    cout << "Average time for " << times << " random searches RBT SORTED: " << search_duration_sorted.count() / times << " nanoseconds" << "\n";
    /////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////// PREORDER //////////////////
    RBTree.preorder();
    /////////////////////////////////////////////////////////////////////////////////////////////



    return EXIT_SUCCESS;
}


/////////////////// Node ///////////////////

Node::Node(publisher key){
	this->key = key;
    this->color = 1; // "RED";
	this->parent = NULL;
	this->left = NULL;
	this->right = NULL;
}

/////////////////// RB-Tree ///////////////////
Node* RB_tree::get_root() {
    return this->root;
}


int Node::get_color() {
    return this->color;
}

void Node::set_color(int color) {
    this->color = color;
}


Node* RB_tree::RB_insert(Node* root, Node* ptr) {
    if (root == NULL) { // no node in the subtree
        return ptr; // if empty RBT, new node becomes the root
    }

    if (ptr->key.name < root->key.name) {
        root->left = RB_insert(root->left, ptr); // go to the left subtree
        root->left->parent = root; // set parent for the left child
    } else if (ptr->key.name > root->key.name) {
        root->right = RB_insert(root->right, ptr); // go to right
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



void RB_tree::insertValue(vector<string> n) {
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

    this->root = RB_insert(this->root, newNode); // add new node to tree
    RB_insert_fixup(newNode);  // Maintain the red-black properties
}


void RB_tree::RB_left_rotate(Node *ptr) {
    Node* rightChild = ptr->right;
    ptr->right = rightChild->left;

    if (rightChild->left != NULL) {
        rightChild->left->parent = ptr;
    }

    rightChild->parent = ptr->parent;
    if (ptr->parent == NULL) {
        this->root = rightChild;
    } else if (ptr == ptr->parent->left) {
        ptr->parent->left = rightChild;
    } else {
        ptr->parent->right = rightChild;
    }

    rightChild->left = ptr;
    ptr->parent = rightChild;
}


void RB_tree::RB_right_rotate(Node *ptr) {
    Node* leftChild = ptr->left;
    ptr->left = leftChild->right;

    if (leftChild->right != NULL) {
        leftChild->right->parent = ptr;
    }

    leftChild->parent = ptr->parent;
    if (ptr->parent == NULL) {
        this->root = leftChild;
    } else if (ptr == ptr->parent->right) {
        ptr->parent->right = leftChild;
    } else {
        ptr->parent->left = leftChild;
    }

    leftChild->right = ptr;
    ptr->parent = leftChild;
}


void RB_tree::RB_insert_fixup(Node* ptr) {
    while (ptr != this->root && ptr->parent && ptr->parent->get_color() == 1) {
        if (ptr->parent == ptr->parent->parent->left) {
            Node* uncle = ptr->parent->parent->right;
            if (uncle && uncle->get_color() == 1) {
                // Case 1: Uncle is red
                ptr->parent->set_color(0);
                uncle->set_color(0);
                ptr->parent->parent->set_color(1);
                ptr = ptr->parent->parent;
            } else {
                if (ptr == ptr->parent->right) {
                    // Case 2: Right child
                    ptr = ptr->parent;
                    RB_left_rotate(ptr);
                }
                // Case 3: Left child
                ptr->parent->set_color(0);
                ptr->parent->parent->set_color(1);
                RB_right_rotate(ptr->parent->parent);
            }
        } else {
            // Mirror of above logic for right subtree
            Node* uncle = ptr->parent->parent->left;
            if (uncle && uncle->get_color() == 1) {
                ptr->parent->set_color(0);
                uncle->set_color(0);
                ptr->parent->parent->set_color(1);
                ptr = ptr->parent->parent;
            } else {
                if (ptr == ptr->parent->left) {
                    ptr = ptr->parent;
                    RB_right_rotate(ptr);
                }
                ptr->parent->set_color(0);
                ptr->parent->parent->set_color(1);
                RB_left_rotate(ptr->parent->parent);
            }
        }
    }

    if (this->root) this->root->set_color(0);
}


void RB_tree::preorder() {
    // Preorder traversal (root, left, right)
    if (this->root != NULL) {
        stack<pair<int, Node*>> s;
        s.push(make_pair(0, this->root));

        while (!s.empty()) {
            Node* node = s.top().second;
            int length = s.top().first; 
            s.pop();
            for(int i = 0; i < length; i++){ // put length times '-'
                cout << "-";
            }
            cout << "(" << (node->get_color() == 1 ? "RED" : "BLACK") << ") " << node->key.name << "\n";

            if (node->right != NULL) s.push(make_pair(length + 1, node->right));
            if (node->left != NULL) s.push(make_pair(length + 1,  node->left));
        }
    }
}


int RB_tree::in_order_traversal(Node* node, int depth) {
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

void RB_tree::find_best_seller() {
    // clear the best_seller array.
    for (int i = 0; i < 3; i++) {
        best_seller[i] = nullptr; // reset previous best sellers.
    }

    // perform in-order traversal to update the best seller array and find the depth of the tree.
    int max_depth = in_order_traversal(root, 0);

    // output the maximum depth, this is good for comparing the BST and RBT as their complexity rely on the max depth
    // cout << "Maximum depth of the RBT: " << max_depth << "\n";
}


// recursive function to search for a publisher, returns null if it does not exist
Node* RB_tree::search_for_publisher(Node* root, const string& publisher_name) {
    if (root == nullptr || root->key.name == publisher_name) {
        return root; // return node if found or null 
    }

    if (publisher_name < root->key.name) {
        return search_for_publisher(root->left, publisher_name); // check left
    }

    return search_for_publisher(root->right, publisher_name); // check right
}

RB_tree::RB_tree(){
	this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
}

Node::~Node() {
	// no dynamic allocation for the node.
}


// Helper function to free the nodes using postorder traversal
void RB_tree::deleteSubtree(Node* node) {
    if (!node) return;

    // Recursively delete the left and right subtrees
    deleteSubtree(node->left);
    deleteSubtree(node->right);

    // Delete the current node
    delete node;
}

// Destructor for the red-black tree
RB_tree::~RB_tree() {
    // Use the helper function to free all nodes in the tree
    deleteSubtree(this->root);

    // Set root to null
    this->root = nullptr;
}
