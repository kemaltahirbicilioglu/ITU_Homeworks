/*
    - do we need to write delete root->employee and als delete root or just wrting delete root is enough
    - do we need to delete check_employee in the remove function when checking if desired to be deleted node exists in bt.
*/

/*@Author
StudentName: Kemal Tahir Bıcılıoğlu
StudentID: 150210083
Date: 18.12.23
*/
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
    
    public:
        Employee();
        Employee(int id, int salary, int department);
        void set_id(int id);
        void set_salary(int salary);
        void set_department(int department);
        int get_id();
        int get_salary();
        int get_department();

};
// mandatory functions for employee definition.
Employee::Employee(){
    this->id = 0;
    this->salary = 0;
    this->department = 0;
}

Employee::Employee(int id, int salary, int department){
    this->id = id;
    this->salary = salary;
    this->department = department;
}

void Employee::set_id(int id){
    this->id = id;
}

void Employee::set_salary(int salary){
    this->salary = salary;
}

void Employee::set_department(int department){
    this->department = department;
}

int Employee::get_id(){
    return this->id;
}

int Employee::get_salary(){
    return this->salary;
}

int Employee::get_department(){
    return this->department;
}

class Node{
    private:
        Employee *employee;
        Node *left;
        Node *right;    
    public:
        Node(Employee *employee);
        Employee* get_employee();
        Node* get_left();
        Node* get_right();
        void set_employee(Employee *employee);
        void set_left(Node *left);
        void set_right(Node *right);
};

Node::Node(Employee *employee){
    this->employee = employee;
    this->left = nullptr;
    this->right = nullptr;
}

Employee* Node::get_employee(){
    return this->employee;
}

Node* Node::get_left(){
    return this->left;
}

Node* Node::get_right(){
    return this->right;
}

void Node::set_employee(Employee* employee){
    this->employee = employee;
}

void Node::set_left(Node* left){
    this->left = left;
}

void Node::set_right(Node* right){
    this->right = right;
}


class BinaryTree{
    private:
        Node *root;
    public:
        BinaryTree();
        BinaryTree(Node* root);
        Node* get_root();
        void set_root(Node* root); 
        void insert(Employee* employee); // inserts node into bt
        void update(int id, int salary, int department); // updates salary and department
        Node* remove(int id); // removes node and put the appropriate node deleted place in order to maintain the inorder structure.
        Employee *search(int id);
        void getHeight(Node* root, int* max_count, int curr_count); // writes the depth of the binary tree into max_count.
        void printToFile(ofstream &output_file, Node* curr); // writes the content of the bt into output file recursively. (inorder)
        void printToConsole(Node* curr); // writes the content of the bt into the termineal recursively. (preorder)
        void clear_content(Node* root); // deallocates all the content of the binary tree.
};

BinaryTree::BinaryTree(){
    this->root = nullptr;
}
BinaryTree::BinaryTree(Node* root){
    this->root = root;
}

Node* BinaryTree::get_root(){
    return this->root;
}

void BinaryTree::set_root(Node* root){
    this->root = root;
}

void print_curr(Node* curr){
    int cout_right, cout_left;
    if(curr->get_left() == nullptr) cout_left = -5;
    else cout_left = curr->get_left()->get_employee()->get_id();

    if(curr->get_right() == nullptr) cout_right = -5;
    else cout_right = curr->get_right()->get_employee()->get_id();
     
    cout << "curr = " << curr->get_employee()->get_id() << " left : " << cout_left << " right : " << cout_right << "\n";
}

void BinaryTree::insert(Employee* employee){
    Node* new_node = new Node(employee);
    Node* temp = this->get_root();
    
    if(temp == nullptr){  // if binary tree is empty set the new node as root.
        this->set_root(new_node);
        return;
    }

    int id = employee->get_id();
    int curr_id;
    while(temp != nullptr){
        curr_id = temp->get_employee()->get_id();
        
        if(id < curr_id){
            if(temp->get_left() == nullptr){ // if there is no left child then temp's left child is the correct place for the node.
                temp->set_left(new_node);
                return;
            }
            temp = temp->get_left();
        }
        else if(id > curr_id){ 
            if(temp->get_right() == nullptr){ // if there is no right child then temp's right child is the correct place for the node.
                temp->set_right(new_node);
                return;
            }
            temp = temp->get_right();
        }
        else{ // id = curr_id means that we are trying to add an id which is already in the bt. 
            cout << "will the bt have duplicate id's ??\n";
            return;
        }
    }
}

void BinaryTree::update(int id, int salary, int department){
    Node* temp = this->get_root();

    int curr_id;
    while(temp != nullptr){
        curr_id = temp->get_employee()->get_id();
        if(id < curr_id){
            if(temp->get_left() == nullptr){ // if we need to go to the left to update the id, and there is no left child, it means id is invalid.
                cout << "ERROR: An invalid ID to update\n";
                return;
            }
            temp = temp->get_left();
        }
        else if(id > curr_id){ 
            if(temp->get_right() == nullptr){ // if we need to go to the right to update the id, and there is no right child, it means id is invalid.
                cout << "ERROR: An invalid ID to update\n";
                return;
            }
            temp = temp->get_right();
        }
        else{ // we found the id.
            temp->get_employee()->set_salary(salary);
            temp->get_employee()->set_department(department);
            return;
        }
    }
    cout << "ERROR: An invalid ID to update\n";
}

Node* BinaryTree::remove(int id){
    Node* curr = this->get_root();
    Node* parent = nullptr;

    
    // find the node to be deleted.
    while (curr != nullptr && curr->get_employee()->get_id() != id) {
        //print_curr(curr);
        parent = curr;
        if (id < curr->get_employee()->get_id())
            curr = curr->get_left();
        else
            curr = curr->get_right();
    }

    if (curr == nullptr){ // there is no employee with the id.
        cout << "ERROR: An invalid ID to delete\n";
        return this->root;
    }

    // if node has no child
    if (curr->get_left() == nullptr && curr->get_right() == nullptr){
        //cout << "has no child\n";
        //print_curr(curr);
        if (parent == nullptr){
            //cout << "parent nullptr\n";
            delete this->get_root();
            return nullptr;
        }  
        else if(parent->get_left() == curr){
            parent->set_left(nullptr);
        } 
        else{
            parent->set_right(nullptr);
        }
        delete curr->get_employee();
        delete curr;
    }

    // When we have two childs, we need to conserve the inorder property of the binary tree while removing.
    else if(curr->get_left() != nullptr && curr->get_right() != nullptr){
        //cout << "have two childs\n";
        //print_curr(curr);
        Node* replace = curr->get_right();
        Node* replaceParent = curr;

        while (replace->get_left() != nullptr) {
            replaceParent = replace;
            replace = replaceParent->get_left();
        }

        // Copy the data of the successor to the current node
        curr->get_employee()->set_id(replace->get_employee()->get_id());
        curr->get_employee()->set_salary(replace->get_employee()->get_salary());
        curr->get_employee()->set_department(replace->get_employee()->get_department());

        // Remove the successor node
        if (replaceParent->get_left() == replace)
            replaceParent->set_left(replace->get_right());
        else
            replaceParent->set_right(replace->get_right());
            
        delete replace->get_employee();
        delete replace;
    }
    // if just 1 child
    else{
        //cout << "have one child\n";
        //print_curr(curr);
        Node* child;
        if(curr->get_left() != nullptr)  child = curr->get_left();
        else child = curr->get_right();
        
        if (parent == nullptr)
            this->set_root(child);
        else if (parent->get_left() == curr)
            parent->set_left(child);
        else
            parent->set_right(child);
        delete curr->get_employee();
        delete curr;
    }
    return this->root;
}

Employee* BinaryTree::search(int id){
    Node* temp = this->get_root();

    int curr_id;
    while(temp != nullptr){
        curr_id = temp->get_employee()->get_id();
        
        if(id < curr_id) temp = temp->get_left();
        
        else if(id > curr_id) temp = temp->get_right();
        
        else return temp->get_employee(); // we found the employee
    }
    Employee *empty = new Employee();
    return empty; // there is no employee with the id.
}

void BinaryTree::getHeight(Node* root, int* max_count, int curr_count){
    if(root == nullptr){
        return;
    }
    curr_count++;
    if(curr_count > *max_count) *max_count = curr_count;

    BinaryTree::getHeight(root->get_left(), max_count, curr_count);
    BinaryTree::getHeight(root->get_right(), max_count, curr_count);
}

void BinaryTree::printToFile(ofstream &output_file, Node* curr){ // inorder traversal left root right
    if(curr == nullptr){
        return;
    }

    BinaryTree::printToFile(output_file, curr->get_left());
    output_file << curr->get_employee()->get_id() << ";" << curr->get_employee()->get_salary() << ";" << curr->get_employee()->get_department() << "\n";
    BinaryTree::printToFile(output_file, curr->get_right()); 
}

void BinaryTree::printToConsole(Node* curr){ // preorder trevarsal root left right
    if(curr == nullptr){
        return;
    }

    cout << curr->get_employee()->get_id() << ";" << curr->get_employee()->get_salary() << ";" << curr->get_employee()->get_department() << "\n";
    BinaryTree::printToConsole(curr->get_left());
    BinaryTree::printToConsole(curr->get_right());
}

void BinaryTree::clear_content(Node* root){ // postorder traversal left right root
    if(root == nullptr){
        return;
    }
    // we should use postorder because if we delete node which has child, we cannot reach its child anymore. 
    // So we have to be sure that every deleted node is leaf node
    BinaryTree::clear_content(root->get_left());
    BinaryTree::clear_content(root->get_right());
    delete root->get_employee();
    delete root;
}

int find_string_size(string s);
bool is_equal_strings(string s1, string s2);
int expand_vector(int id); // it expands the vector when its needed, also returns the correct binary tree to do operations

vector<BinaryTree*> vector_of_trees;

int main(int argc, char *argv[]){
//clock_t start = clock(); 

    // argc, argv statements. get file names here
    string data_file;
    string operation_file;
    if(argc == 3){
        data_file = argv[1];
        operation_file = argv[2];
    }
    else{
        cout << "invalid argument count\n";
        return 0;
    }

    // opening data file.
    fstream data_fptr;
    data_fptr.open(data_file, ios::in);
    if(!data_fptr.is_open()){
        cout << "data file cannot be opened\n";
        return 1;
    }

    string line;
    getline(data_fptr, line); // discard the first line.

    // copy all the element from the file to the map.
    string s_id, s_salary, s_department;
    int id, salary, department, index;
    int last_id = 0;
    while(getline(data_fptr, line)){
        index = 0;  // for the line taken from the file
        s_id = s_salary = s_department = "";
        while(line[index] != ';'){
            s_id += line[index];
            index++;
        }

        index++;
        while(line[index] != ';'){
            s_salary += line[index];
            index++;
        }

        index++;
        while(line[index] != '\0'){
            s_department += line[index];
            index++;
        }
        id = stoi(s_id);
        salary = stoi(s_salary);
        department = stoi(s_department);
        // we got the id, salary, department.
        if(id > last_id) last_id = id;

        Employee *new_employee = new Employee(id, salary, department);
        int place = expand_vector(id);
        vector_of_trees[place]->insert(new_employee);
    }
    data_fptr.close();

// opening operation file.
    fstream operation_fptr;
    operation_fptr.open(operation_file, ios::in);
    if(!operation_fptr.is_open()){
        cout << "operation file cannot be opened\n";
        return 1;
    }   

    string operation;
    bool p_h; // if the operation is PRINT or HEIGHT do not continue the while loop since there wont be any salary or department data.
    while(getline(operation_fptr, line)){
        p_h = false;
        index = 0;
        s_id = s_salary = s_department = operation = "";
        while(line[index] != ';' && !p_h){ // checks until it sees a ';' or it finds the operations PRINT or HEIGHT
            if(is_equal_strings("PRINT", operation)){
                p_h = true;
                cout << "P\n";
                int place = expand_vector(id);
                vector_of_trees[place]->printToConsole(vector_of_trees[place]->get_root());
            }else if(is_equal_strings("HEIGHT", operation)){
                p_h = true;
                int height = 0;

                int place = expand_vector(id);
                vector_of_trees[place]->getHeight(vector_of_trees[place]->get_root(), &height, 0);
                cout << "H " << height << "\n";
            } 
            operation += line[index];
            index++;
        }
        if(p_h) continue; // check if the operation is PRINT or HEIGHT.
        index++;

        if(is_equal_strings("ADD", operation)){
            //clock_t start = clock();
            s_salary = s_department = "";
            while(line[index] != ';'){
               s_salary += line[index];
                index++;
            }

            index++;
            while(line[index] != '\0'){
                s_department += line[index];
                index++;
            }
            salary = stoi(s_salary);
            department = stoi(s_department);
            // we got the salary and the department.
            last_id++;
            id = last_id;
            Employee *new_employee = new Employee(id, salary, department);
            
            int place = expand_vector(id);
            vector_of_trees[place]->insert(new_employee);
            //clock_t end = clock();
            //cout << "ADD: binary_tree solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }
        else if(is_equal_strings("UPDATE", operation)){
            //clock_t start = clock();
            s_id = s_salary = s_department = "";

            while(line[index] != ';'){
                s_id += line[index];
                index++;
            }
            
            index++;
            while(line[index] != ';'){
                s_salary += line[index];
                index++;
            }
            
            index++;
            while(line[index] != '\0'){
                s_department += line[index];
                index++;
            }
            id = stoi(s_id);
            salary = stoi(s_salary);
            department = stoi(s_department);
            // we got id, salary, department.
            int place = id/5000;
            int size = vector_of_trees.size();
            if(place + 1 > size){
                cout << "ERROR: An invalid ID to update\n"; 
            }else{
                vector_of_trees[place]->update(id, salary, department);
            }
            //clock_t end = clock();
            //cout << "UPDATE: binary_tree solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }
        else if(is_equal_strings("DELETE", operation)){
            //clock_t start = clock();
            s_id = "";
            while(line[index] != '\0'){
                s_id += line[index];
                index++;
            }         
            id = stoi(s_id);
            // We got the id data to delete from employee array.
            int place = id/5000;
            int size = vector_of_trees.size();
            if(place + 1 > size){
                cout << "ERROR: An invalid ID to delete\n"; 
            }else{
                vector_of_trees[place]->set_root(vector_of_trees[place]->remove(id)); 
            }            
            //clock_t end = clock();
            //cout << "UPDATE: binary_tree solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";

        }
        else{
            cout << "invalid operation has been obtained from operations file: " << operation << "\n";
        }
    }
    operation_fptr.close();

    ofstream bt_solution_fptr;
    bt_solution_fptr.open("output.csv");
    bt_solution_fptr << "Employee_ID;Salary;Department\n";
    for(unsigned int i = 0; i < vector_of_trees.size(); i++){
        BinaryTree *bt = vector_of_trees[i];
        bt->printToFile(bt_solution_fptr, bt->get_root());

        bt->clear_content(bt->get_root());
    }
    bt_solution_fptr.close();

//clock_t end = clock();
//cout << "main: binary_tree solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";

    return 0;    
}


int find_string_size(string s){
    int index = 0;
    while(s[index] != '\0'){
        index++;
    } 
    return index;
}

bool is_equal_strings(string s1, string s2){
    int size1 = find_string_size(s1);
    int size2 = find_string_size(s2);
    if(size1 != size2){
        return false;
    }

    for(int i = 0; i < size1; i++){
        if(s1[i] != s2[i]){
            return false;
        }
    }
    return true;
}

int expand_vector(int id){ // it expands the vector when its needed, also returns the correct binary tree to do operations
    int curr_size = vector_of_trees.size();
    if ((id / 5000) + 1 > curr_size){
        for(int i = curr_size; i < (id/5000) + 1; i++){
            BinaryTree *empty_tree = new BinaryTree();
            vector_of_trees.push_back(empty_tree);
        }
    }
    return id/5000; // correct binary tree place to do operations
}