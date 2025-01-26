/*@Author
StudentName: Kemal Tahir Bıcılıoğlu
StudentID: 150210083
Date: 01.11.23
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

void print_vector(vector<Employee> &employees){
    int size = employees.size();
    if(size == 0){
        cout << "vector is empty\n";
    }
    for(int i = 0; i < size; i++){
        cout << "id: " << employees[i].get_id() << " salary: " << employees[i].get_salary() << " department: " << employees[i].get_department() << "\n";
    }
}

void update_employee_vector(vector<Employee> &employees, int id, int salary, int department){
    int size = employees.size();
    int curr_id;
    for(int i = 0; i < size; i++){
        curr_id = employees[i].get_id();
        if(curr_id == id){
            employees[i].set_salary(salary);
            employees[i].set_department(department);
            return;
        }
    }
    cout << "ERROR: An invalid ID to update\n";
}

void delete_employee_vector(vector<Employee> &employees, int id){
    int size = employees.size();
    if(size == 0){
        cout << "ERROR: There is no Employee\n";
        return;
    }
    int curr_id;
    int found_index = -1;
    for(int i = 0; i < size; i++){
        curr_id = employees[i].get_id();
        if(curr_id == id){
            found_index = i;
            break;
        }
    }
    if(found_index == -1){
        cout << "ERROR: An invalid ID to delete\n";
        return;
    }
    employees.erase(employees.begin() + found_index);
}

int main(int argc, char *argv[]){

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
        return 0;
    }

    string line;
    getline(data_fptr, line); // discard the first line.

    // copy all the element from the file to the linked list using add_element_to_end function.
    string s_id, s_salary, s_department;
    int id, salary, department, index;
    int last_id = 0;
    vector<Employee> employees;
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

        Employee new_employee = Employee(id, salary, department);
        employees.push_back(new_employee);
    }
    last_id = id;
    data_fptr.close();

// opening operation file.
    fstream operation_fptr;
    operation_fptr.open(operation_file, ios::in);
    if(!operation_fptr.is_open()){
        cout << "operation file cannot be opened\n";
        return 1;
    }   

    string operation;

clock_t start = clock();

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
            Employee new_employee = Employee(id, salary, department);
            employees.push_back(new_employee);
            
            //clock_t end = clock();
            //cout << "ADD: vector solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
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

            update_employee_vector(employees, id, salary, department);

            //clock_t end = clock();
            //cout << "UPDATE: vector solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
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
            delete_employee_vector(employees, id);
            
            //clock_t end = clock();
            //cout << "DELETE: vector solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }
        else{
            cout << "invalid operation has been obtained from operations file: " << operation << "\n";
        }
    }
    operation_fptr.close();
    clock_t end = clock();
    cout << "main: vector solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";


    fstream vector_solution_fptr;
    vector_solution_fptr.open("vector_solution.csv", ios_base:: out);
    vector_solution_fptr << "Employee_ID;Salary;Department\n";

    int size = employees.size();
    for(int i = 0; i < size; i++){
        vector_solution_fptr << employees[i].get_id() << ";" << employees[i].get_salary() << ";" << employees[i].get_department() << "\n";
    }

    vector_solution_fptr.close();


    return 0;
}