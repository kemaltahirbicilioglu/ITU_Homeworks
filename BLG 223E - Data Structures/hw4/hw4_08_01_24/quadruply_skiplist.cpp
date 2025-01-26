/*@Author
StudentName: Kemal Tahir Bıcılıoğlu
StudentID: 150210083
Date: 29.12.23
*/
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int find_string_size(string s); // return string's size.
bool is_equal_strings(string s1, string s2); // return if two strings equal
int rand_num(int lower, int upper); // return random number between lower and upper. including lower and upper nums.
int approximate_log2(int num);


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


class QuadruplySkipList_Node{
    private:
        Employee* data;
        QuadruplySkipList_Node* next;
        QuadruplySkipList_Node* prev;
        QuadruplySkipList_Node* below;
        QuadruplySkipList_Node* above;

    public:
        QuadruplySkipList_Node();
        QuadruplySkipList_Node(Employee *data);
        QuadruplySkipList_Node* get_next();
        QuadruplySkipList_Node* get_below();
        QuadruplySkipList_Node* get_prev();
        QuadruplySkipList_Node* get_above();
        
        void set_next(QuadruplySkipList_Node* next);
        void set_below(QuadruplySkipList_Node* below);
        void set_prev(QuadruplySkipList_Node* prev);
        void set_above(QuadruplySkipList_Node* above);

        void set_employee(Employee* Employee);
        Employee* get_employee();
};

QuadruplySkipList_Node::QuadruplySkipList_Node(){
    this->data = nullptr;
    this->next = nullptr;
    this->below = nullptr;
    this->prev = nullptr;
    this->above = nullptr;
}

QuadruplySkipList_Node::QuadruplySkipList_Node(Employee *data){
    this->data = data;
    this->next = nullptr;
    this->below = nullptr;
    this->prev = nullptr;
    this->above = nullptr;
}

void QuadruplySkipList_Node::set_next(QuadruplySkipList_Node* next){
    this->next = next;
}

void QuadruplySkipList_Node::set_below(QuadruplySkipList_Node* below){
    this->below = below;
}

void QuadruplySkipList_Node::set_prev(QuadruplySkipList_Node* prev){
    this->prev = prev;
}

void QuadruplySkipList_Node::set_above(QuadruplySkipList_Node* above){
    this->above = above;
}

void QuadruplySkipList_Node::set_employee(Employee* Employee){
    this->data = Employee;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_next(){
    return this->next;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_below(){
    return this->below;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_prev(){
    return this->prev;
}

QuadruplySkipList_Node* QuadruplySkipList_Node::get_above(){
    return this->above;
}

Employee* QuadruplySkipList_Node::get_employee(){
    return this->data;
}


class QuadruplySkipList{
    private:
        QuadruplySkipList_Node* head;
        int height;
    public:
        QuadruplySkipList();
        QuadruplySkipList(int height_in);
        QuadruplySkipList_Node* get_head();
        void set_head(QuadruplySkipList_Node *head);
        int get_height();
        void set_height(int height);
        void insert(Employee* data);
        void remove(int remove_id);
        void update(int id, int salary, int department);

        Employee* search(int search_id);
        void print_to_console();
        void dumpToFile(ofstream& out_file);
        void clear_content();
};

QuadruplySkipList::QuadruplySkipList(){
    this->head = nullptr;
    this->height = 0;
}

QuadruplySkipList_Node* QuadruplySkipList::get_head(){
    return this->head;
}

void QuadruplySkipList::set_head(QuadruplySkipList_Node *head){
    this->head = head;
}

int QuadruplySkipList::get_height(){
    return this->height;
}


void QuadruplySkipList::set_height(int height){ // creates starting nodes as INT_MIN and ending nodes as INT_MAX
    this->height = height;
    Employee* min_value = new Employee(INT32_MIN, 0, 0);
    this->head = new QuadruplySkipList_Node(min_value); // first node that head is pointing to
    QuadruplySkipList_Node* temp = this->head;

    for(int i = 1; i < this->height; i++){ // set start nodes bound them to each other.
        Employee* min_value = new Employee(INT32_MIN, 0, 0);
        QuadruplySkipList_Node* start_node = new QuadruplySkipList_Node(min_value);
        temp->set_below(start_node);
        start_node->set_above(temp);
        temp = temp->get_below();
    }

    QuadruplySkipList_Node* prev_added = nullptr;
    temp = this->head;
    for(int i = 0; i < this->height; i++){ // set end nodes bound them with the start nodes.
        Employee* max_value = new Employee(INT32_MAX, 0, 0);
        QuadruplySkipList_Node* end_node = new QuadruplySkipList_Node(max_value);
        if(prev_added == nullptr){
            prev_added = end_node;
        }else{
            prev_added->set_below(end_node);
            end_node->set_above(prev_added);
            prev_added = prev_added->get_below();
        }
        temp->set_next(end_node);
        end_node->set_prev(temp);
        temp = temp->get_below();
    }
}


QuadruplySkipList::QuadruplySkipList(int height_in){  // creates starting nodes as INT_MIN and ending nodes as INT_MAX
    this->height = height;
    Employee* min_value = new Employee(INT32_MIN, 0, 0);
    this->head = new QuadruplySkipList_Node(min_value); // first node that head is pointing to
    QuadruplySkipList_Node* temp = this->head;

    for(int i = 1; i < this->height; i++){ // set start nodes bound them to each other.
        Employee* min_value = new Employee(INT32_MIN, 0, 0);
        QuadruplySkipList_Node* start_node = new QuadruplySkipList_Node(min_value);
        temp->set_below(start_node);
        start_node->set_above(temp);
        temp = temp->get_below();
    }

    QuadruplySkipList_Node* prev_added = nullptr;
    temp = this->head;
    for(int i = 0; i < this->height; i++){ // set end nodes bound them with the start nodes.
        Employee* max_value = new Employee(INT32_MAX, 0, 0);
        QuadruplySkipList_Node* end_node = new QuadruplySkipList_Node(max_value);
        if(prev_added == nullptr){
            prev_added = end_node;
        }else{
            prev_added->set_below(end_node);
            end_node->set_above(prev_added);
            prev_added = prev_added->get_below();
        }
        temp->set_next(end_node);
        end_node->set_prev(temp);
        temp = temp->get_below();
    }
}

void QuadruplySkipList::insert(Employee* data){ // inserts given employee to the skip list    
    int rand_height = rand_num(1, this->height); // generate random number between 1 and the max_height of the skiplist.
    QuadruplySkipList_Node* temp = this->head;
    int go_below = this->height - rand_height;
    for(int i = 0; i < go_below; i++){ // go below to insert rand_height times new node and start from the node in the rand_height place from bottom.
        temp = temp->get_below();
    }

    QuadruplySkipList_Node* prev_added = nullptr; 
// while we can directly set_next, we cannot just set_below, so we need to keep track of the above node to set its below when we find its below node
    
    while(temp != nullptr){
        if(temp->get_next()->get_employee()->get_id() > data->get_id()){
            QuadruplySkipList_Node* new_node = new QuadruplySkipList_Node(data);
            QuadruplySkipList_Node* keep_next = temp->get_next(); // keep previous's next node 
            temp->set_next(new_node); // bound the new node to the previous
            temp->get_next()->set_next(keep_next); // bound new node's next to previously node's next.
            keep_next->set_prev(new_node); // set new_nodes next node's prev to new node.
            if(prev_added == nullptr){ // firstly finding the inserted id.
                prev_added = temp->get_next();
            }else{
                prev_added->set_below(temp->get_next()); // we have prev node from the last iteration to set its below node to currently added node.
                temp->get_next()->set_above(prev_added); // set currently added node's above to the previosly added node from the last iteration.
                prev_added = prev_added->get_below();
            }

            temp = temp->get_below(); // go below to keep adding new nodes rand_height amount of time. It will exit the loop when there is no other below pointer.

        }else{ // since we won't have duplicated ids this condition is if curr_id < id we need to go right 
            temp = temp->get_next();  
        }
    }

}

void QuadruplySkipList::update(int id, int salary, int department){ // updates the id with the given salary and the department
    bool found = false;
    QuadruplySkipList_Node* temp = this->head;
    
    while(temp != nullptr){
        if(temp->get_next()->get_employee()->get_id() > id){
            temp = temp->get_below(); // if next is higher id, we need to go to below because we need to look ids between curr and next and they exist below.
        }
        else if(temp->get_next()->get_employee()->get_id() == id){ // found, update.
            found = true;
            temp->get_next()->get_employee()->set_id(id);
            temp->get_next()->get_employee()->set_salary(salary);
            temp->get_next()->get_employee()->set_department(department);
            temp = temp->get_below(); // continue to below since we need to update all the nodes with the desired id.
        }else{
            temp = temp->get_next(); // if curr_id < id go to next
        }
    }
    if(!found){ // if we have not found the id 
        cout << "ERROR: An invalid ID to update\n";
    }
}

void QuadruplySkipList::remove(int remove_id){ // remove all the nodes with the desired id
    bool found = false;
    QuadruplySkipList_Node* temp = this->head;
    for(int i = 1; i < this->height; i++){ // to check if there is no employee, we need to go to the bottom. If there is an element it is guarenteed that it lies on the most below height.
        temp = temp->get_below();
    }

    if(temp->get_next()->get_employee()->get_id() == INT32_MAX){ // if next is INT_MAX it means we have just starting and ending nodes.
        cout << "ERROR: There is no Employee\n";
        return;
    }
    temp = this->head; // intialize back to the head.

    while(temp != nullptr){ // same traversal logic
        if(temp->get_next()->get_employee()->get_id() > remove_id){
            temp = temp->get_below();
        }
        else if(temp->get_next()->get_employee()->get_id() == remove_id){
            found = true;
            QuadruplySkipList_Node* keep_next = temp->get_next()->get_next();
            delete temp->get_next();
            temp->set_next(keep_next);
            keep_next->set_prev(temp);
            temp = temp->get_below(); // continue deleting all the nodes with the desired id.
        }
        else{
            temp = temp->get_next();
        }
    }
    if(!found){
        cout << "ERROR: An invalid ID to delete\n";
    }
}

Employee* QuadruplySkipList::search(int search_id){ // searchs for the given id
    QuadruplySkipList_Node* temp = this->head;
    Employee *find_employee = new Employee();
    
    while(temp != nullptr){
        if(temp->get_next()->get_employee()->get_id() > search_id){
            temp = temp->get_below();
        }
        else if(temp->get_next()->get_employee()->get_id() == search_id){
            find_employee = temp->get_next()->get_employee();
        }else{
            temp = temp->get_next();
        }
    }
    return find_employee; 
}

void QuadruplySkipList::print_to_console(){ // print all the content of the skip list to the console
    cout << "Employee_ID;Salary;Department\n";
    QuadruplySkipList_Node *temp = this->head;
    for(int i = 1; i < this->height; i++){  // we should go to the bottom because all the elements exist there distinctly.
        temp = temp->get_below();
    }

    int id, salary, department;
    temp = temp->get_next(); // do not take the starting nodes. skip them.
    while(temp->get_next() != nullptr){ // check temp->next not to get ending nodes.
        id = temp->get_employee()->get_id();
        salary = temp->get_employee()->get_salary();
        department = temp->get_employee()->get_department();

        cout << id << ";" << salary << ";" << department << "\n";
        temp = temp->get_next();
    }
}

void QuadruplySkipList::dumpToFile(ofstream& out_file){ // print all the content of the skip list to the console
    out_file << "Employee_ID;Salary;Department\n";
    QuadruplySkipList_Node *temp = this->head;
    for(int i = 1; i < this->height; i++){ // we should go to the bottom because all the elements exist there distinctly.
        temp = temp->get_below();
    }

    int id, salary, department;
    temp = temp->get_next(); // do not take the starting nodes. skip them.
    while(temp->get_next() != nullptr){ // check temp->next not to get ending nodes.
        id = temp->get_employee()->get_id();
        salary = temp->get_employee()->get_salary();
        department = temp->get_employee()->get_department();

        out_file << id << ";" << salary << ";" << department << "\n";
        temp = temp->get_next();
    }

}

void QuadruplySkipList::clear_content(){ // deallocates the nodes.
    QuadruplySkipList_Node* temp = this->head;

    while(temp != nullptr){ // to be able to go down
        QuadruplySkipList_Node* temp1 = temp;
        while(temp1 != nullptr){  // to delete all the nodes in that height
            QuadruplySkipList_Node* temp2 = temp1;
            temp1 = temp1->get_next();
            delete temp2;
        }
        temp = temp->get_below();
    }
}

QuadruplySkipList skip_list = QuadruplySkipList();


int main(int argc, char *argv[]){
//clock_t start = clock(); 

    srand(time(NULL)); // to get random numbers in each execution
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

    int height, count;
    count = 0;
    // get how many lines are there to calculate optimize height of the skip list which is O(logn)
    while(getline(data_fptr, line)){
        count++;
    }
    data_fptr.close();
    data_fptr.open(data_file, ios_base::in);
    getline(data_fptr, line); // discard the first line.

    height = approximate_log2(count);
    skip_list.set_height(height);

    // copy all the element from the file to the skip_list.
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
        skip_list.insert(new_employee);
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
    while(getline(operation_fptr, line)){
        index = 0;
        s_id = s_salary = s_department = operation = "";
        while(line[index] != ';'){ 
            operation += line[index];
            index++;
        }
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
            skip_list.insert(new_employee);
            //clock_t end = clock();
            //cout << "ADD: skip_list solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
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

            skip_list.update(id, salary, department);
            //clock_t end = clock();
            //cout << "UPDATE: skip_list solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
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

            skip_list.remove(id);    
            
            //clock_t end = clock();
            //cout << "DELETE: skip_list solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";

        }
        else{
            cout << "invalid operation has been obtained from operations file: " << operation << "\n";
        }
    }
    operation_fptr.close();

    ofstream skip_list_file_fptr;
    skip_list_file_fptr.open("output.csv");

    skip_list.dumpToFile(skip_list_file_fptr);
    skip_list_file_fptr.close();

    skip_list.clear_content(); // deallocate the used memory.

//clock_t end = clock();
//cout << "main: skip_list solution with height " << height << " " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";

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

int rand_num(int lower, int upper){
    int res = (rand() % (upper - lower + 1)) + lower;
    return res;
}

int approximate_log2(int num){
    int ans = 0;
    while(num != 0){
        num /= 2;
        ans++;
    }
    return ans;
}