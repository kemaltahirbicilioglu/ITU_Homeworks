/*@Author
StudentName: Kemal Tahir Bıcılıoğlu
StudentID: 150210083
Date: 01.10.23
*/
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Employee{
    private:
        int id;
        int salary;
        int department;
        Employee *next;
    
    public:
        Employee();
        Employee(int id, int salary, int department);
        void set_id(int id);
        void set_salary(int salary);
        void set_department(int department);
        void set_next(Employee* next);
        
        int get_id();
        int get_salary();
        int get_department();
        Employee* get_next();
};

// functions:

Employee::Employee(){
    this->id = 0;
    this->salary = 0;
    this->department = 0;
    this->next = NULL;
}

Employee::Employee(int id, int salary, int department){
    this->id = id;
    this->salary = salary;
    this->department = department;
    this->next = NULL;
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

void Employee::set_next(Employee *next){
    this->next = next;
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

Employee* Employee::get_next(){
    return this->next;
}

void print_linked_list(Employee *head){
    if (head == nullptr) {
        cout << "Linked list is empty." << "\n";
        return;
    }

    Employee *temp = head;
    while (temp != nullptr) {
        cout << "id: " << temp->get_id() << " salary: " << temp->get_salary() << " department: " << temp->get_department() << "\n";
        temp = temp->get_next();
    }
}

void delete_linked_list(Employee *head){
    Employee *temp1 = head;
    while (temp1 != NULL) {
        Employee *temp2 = temp1;
        temp1 = temp1->get_next();
        delete temp2;
    }
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

void add_element_linked_list(Employee **head, Employee **tail, int size, Employee *new_element){
    if(size == 0){
        *head = new_element;
        *tail = new_element;
        return;
    }

    /* 
    Employee *temp = head; // in this way, we are adding new element in O(n). Since we are gonna add n times it goes to O(n^2). Instead use tail.
    while(temp->get_next() != NULL){
        temp = temp->get_next();
    }
    temp->set_next(new_element);
    */

    (*tail)->set_next(new_element);
    *tail = new_element;
}

void update_employee_linked_list(Employee **head, int id, int salary, int department){
    Employee *temp = *head;
    int curr_id;
    while (temp != nullptr) {
        curr_id = temp->get_id();
        if (curr_id == id) {
            temp->set_salary(salary);
            temp->set_department(department);
            return; // When updated, we can return.
        }
        temp = temp->get_next();
    }
    cout << "ERROR: An invalid ID to update\n";
}

bool delete_employee_linked_list(Employee **head, Employee **tail, int id, int size){
    if(size == 0){
        cout << "ERROR: There is no Employee" << "\n";
        return false;
    }
    else if(size == 1){
        if((*head)->get_id() == id){
            delete (*head);
            (*head) = nullptr;
            (*tail) = nullptr;
            return true;
        }
        else {
            cout << "ERROR: Invalid ID to delete" << "\n";
            return false;
        }
    }

    Employee *current = *head;
    Employee *prev = nullptr;

    while (current != nullptr) {
        if (current->get_id() == id) {
            if (current == *head) { // Deleting the head node
                *head = current->get_next();
                delete current;
                return true;
            }
            else if (current == *tail) { // Deleting the tail node
                *tail = prev;
                (*tail)->set_next(nullptr);
                delete current;
                return true;
            }
            else { // Deleting a node in the middle
                prev->set_next(current->get_next());
                delete current;
                return true;
            }
        }
        prev = current;
        current = current->get_next();
    }

    cout << "ERROR: Invalid ID to delete" << "\n";
    return false;
}


int main(int argc, char *argv[]){

    // argc argv, get file names here.
    string data_file, operation_file;
    if(argc == 3){
        data_file = argv[1];
        operation_file = argv[2];
    }else{
        cout << "invalid argument count\n";
        return 1;
    }

    // opening data file.
    fstream data_fptr;
    data_fptr.open(data_file, ios_base::in);
    if(!data_fptr.is_open()){
        cout << "data file cannot be opened\n";
        return 0;
    }
    string line;
    getline(data_fptr, line); // discard the first line.

    // copy all the element from the file to the linked list using add_element_to_end function.
    string s_id, s_salary, s_department;
    int id, salary, department, index;
    Employee *head = new Employee();
    Employee *tail = new Employee();
    int size = 0;
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

        Employee *new_employee = new Employee(id, salary, department);
        add_element_linked_list(&head, &tail, size, new_employee);
        size++;
    }
    if(size > 0){
        last_id = id;
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
            add_element_linked_list(&head, &tail, size, new_employee);
            size++;            
            
            //clock_t end = clock();
            //cout << "ADD: linked_list_solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
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

            update_employee_linked_list(&head, id, salary, department);

            //clock_t end = clock();
            //cout << "UPDATE: linked_list_solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
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
            bool is_deleted = delete_employee_linked_list(&head, &tail, id, size);
            if(is_deleted){
                size--;
            }
            //clock_t end = clock();
            //cout << "DELETE: linked_list_solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }
        else{
            cout << "invalid operation has been obtained from operations file: " << operation << "\n";
        }
    }
    operation_fptr.close();

    fstream linked_list_solution_fptr;
    linked_list_solution_fptr.open("linked_list_solution.csv", ios_base::out);

    linked_list_solution_fptr << "Employee_ID;Salary;Department\n";
    Employee* temp = head;
    while(temp != NULL){
        linked_list_solution_fptr << temp->get_id() << ";" << temp->get_salary() << ";" << temp->get_department() << "\n";
        temp = temp->get_next();
    }

    linked_list_solution_fptr.close();
    delete_linked_list(head);
    head = NULL;
    tail = NULL;

//clock_t end = clock();
//cout << "main: linked_list_solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
    return 0;
}