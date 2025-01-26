/*@Author
StudentName: Kemal Tahir Bıcılıoğlu
StudentID: 150210083
Date: 25.10.23
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
    int id = this->id;
    return id;
}

int Employee::get_salary(){
    int salary = this->salary;
    return salary;
}

int Employee::get_department(){
    int department = this->department;
    return department;
}

int get_inputs(fstream &data_fptr, Employee *employees){ // it returns the number of employees added to the array.
    int i = -1;
    string line, s_id, s_salary, s_department;
    int id, salary, department;
    while(getline(data_fptr, line)){
        if(i == -1){ // not to get the first line which does not have data values.
            i++;
            continue;
        }
        int index = 0;  // for the line taken from the file
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
        employees[i] = new_employee;
        i++;
    }
    return i;
}

int clear_empty_spaces(Employee *employees, Employee *new_employees, int employee_size){ // it returns the last id of the new_employees array.
    Employee curr_employee;
    for(int i = 0; i < employee_size; i++){
        curr_employee = employees[i];
        new_employees[i] = curr_employee;
    }
    int last_id = curr_employee.get_id(); 
    // curr employee will be the last employee of the new_employees array, could have been new_employees[employee_size - 1].

    return last_id;    
}

void print_employees(Employee *employees, int size){
    Employee curr_employee;
    for(int i = 0; i < size; i++){
        curr_employee = employees[i];
        cout << i << "- id: " << curr_employee.get_id() << " salary: " << curr_employee.get_salary() << " department: " << curr_employee.get_department() << "\n";
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

void add_employee(int salary, int department, Employee *employees, int size, Employee *new_employees, int last_id){ // copy old content, add last_id.
    Employee curr_employee;
    for(int i = 0; i < size; i++){
        curr_employee = employees[i];
        new_employees[i] = curr_employee;
    }
    Employee new_employee = Employee(last_id + 1, salary, department); // newly added employee.
    new_employees[size] = new_employee;
    delete[] employees; // after addition, our new employee array is new_employees, we need to delete the old one.
    // this function also deals with the case in which we have employees array with 0 element, new_employees[0] = new_employee is valid.
}

void update_employee(int id, int salary, int department, Employee *employees, int size){
    Employee curr_employee;
    int curr_id;
    bool found = false;
    for(int i = 0; i < size; i++){
        curr_employee = employees[i];
        curr_id = curr_employee.get_id();
        if(curr_id == id){
            found = true;
            employees[i].set_salary(salary);
            employees[i].set_department(department);
            return ; // if I found the desired id, no need to continue iterating employee array. This return optimize the complexity little bit.
        }
    }
    if(!found){ // if desired id does not exist in employee array, we should write an error message.
        cout << "ERROR: An invalid ID to update\n";
    }
}

bool delete_employee(int id, Employee *employees, int size, Employee *new_employees){ // it returns the last id as we may delete the last data.
    Employee curr_employee;
    int curr_id; 
    bool found = false; 
    int found_index; // where the desired id is found.
    for(int i = 0; i < size; i++){
        curr_employee = employees[i];
        curr_id = curr_employee.get_id();
        if(curr_id == id){
            found = true;
            found_index = i;
            break; 
            // when found, we need to exit this iteration and start to copy next element in to empty spaces that will be created by the deletion      
        }
    }
    if(!found){
        cout << "ERROR: An invalid ID to delete\n";
        return found; // if we iterate over the employee array and could not find the desired id, we need to write an error message and return.
    }
    int index = 0; // to copy old elements into new employee array.
    for(int i = 0; i < size; i++){ // starting from the next element of deleted id to copy the old array to new one. 
        if(i == found_index){
            continue; // do not take the employee that is asked to be deleted.
        }
        curr_employee = employees[i];
        new_employees[index] = curr_employee;
        index++;
    }
    delete[] employees;
    return found;
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
    //cout << "data file is opened succesfully\n";


    // getting datas from the file, clearing empty spaces. while doing these, declaring employees, largest size, employee_size, last_id
    int size = 500005; // ************ IMPORTANT NOTE ************ the largest size that employee array can be.
    Employee *temp_employees = new Employee[size];
    int employee_size = get_inputs(data_fptr, temp_employees);
    data_fptr.close();
    // to clear empty spaces in the employees array.
    
    Employee *employees = NULL; // to get clear (without datas initialized with constructor) employees array or getting empty employees array.
    Employee *employee_ptr = NULL; // we are going to use it to point new arrays created from delete and update functions.
    int last_id; // make easier to add new employee with an id of last_id + 1.
    
    if(employee_size == 0){ // if I did not get any employee value from the text, I should keep my employee array empty.
        employees = new Employee();
        employee_ptr = employees;
        last_id = 0; // ************** IMPORTANT NOTE ************** WHAT VALUE ID NEVER CAN BE ?? when add is called new id will be 1.
    }
    else{
        employees = new Employee[employee_size]; // this has the size which has been obtained how many employees we took from data file.
        employee_ptr = employees;
        last_id = clear_empty_spaces(temp_employees, employee_ptr, employee_size);
    }

    delete[] temp_employees; // we no longer need to keep temp_employees since we have the clear data obtained from data file.    

    // opening operations file.
    fstream operation_fptr;
    operation_fptr.open(operation_file, ios::in);
    if(!operation_fptr.is_open()){
        cout << "operation file cannot be opened\n";
        return 0;
    }
    //cout << "operation file is opened succesfully\n";    

    // reading operation file and responding them.
    // so we are going to equalize employee_ptr to new_employees resulted after update and delete operations in this part of the code.
    
    string line, operation, s_id, s_salary, s_department; // --COMPILER OPTIMIZATION LEVEL--
    int index, id, salary, department;

clock_t start = clock();
    while(getline(operation_fptr, line)){
        
        index = 0; // to iterate over each line obtained from operation file.
        operation = "";
        while(line[index] != ';'){
            operation += line[index];
            index++;
        }
        index++;

        if(is_equal_strings("ADD", operation)){ // add a given salary and department data to the end of the employee array with an id = last_id+1.
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

            Employee *new_employees = new Employee[employee_size+1];
            add_employee(salary, department, employee_ptr, employee_size, new_employees, last_id);
            // it is going to make last_id 1, if there is no element in employee array. Because my last_id = 0 in this case.
            employee_size++;
            last_id++;
            employee_ptr = new_employees;
            
            
            //clock_t end = clock();
            //cout << "Array ADD: " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }

        else if(is_equal_strings("UPDATE", operation)){ // update the desired id with the given data.
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

            update_employee(id, salary, department, employee_ptr, employee_size);

            //clock_t end = clock();
            //cout << "Array UPDATE: " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }

        else if(is_equal_strings("DELETE", operation)){ // delete the desired id and copy old data excluding the deleted data to new_employee array.
            //clock_t start = clock();

            s_id = "";
            while(line[index] != '\0'){
                s_id += line[index];
                index++;
            }         
            id = stoi(s_id);
            // We got the id data to delete from employee array.

            if(employee_size <= 0){
                cout << "ERROR: There is no Employee\n";
            }
            else if(employee_size == 1){
                Employee curr_employee = employee_ptr[0]; // take the only employee that employees array
                int curr_id = curr_employee.get_id();
                if(curr_id == id){
                    delete[] employee_ptr;
                    Employee *new_employees = new Employee[1]; // size == 1 && curr_id == id means that there will be an array with a size of 0.
                    employee_ptr = new_employees;
                    employee_size--;
                }
                else{
                    cout << "ERROR: An invalid ID to delete\n";
                }
            }
            else{
                Employee *new_employees = new Employee[employee_size - 1];
                bool is_deleted = delete_employee(id, employee_ptr, employee_size, new_employees);
                if(!is_deleted){ // it means invalid id. So we delete newly created array and continue with the old one.
                    delete[] new_employees;  // since id was not found, we did not use new_employees array.
                }else{
                    employee_ptr = new_employees;
                    employee_size--;
                }
            }

            //clock_t end = clock();
            //cout << "Array DELETE: " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }
        
        else{
            cout << "invalid operation has been obtained from operations file: " << operation << "\n";
        }
    }
    operation_fptr.close();
    
    clock_t end = clock();
    cout << "main: array solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";


    // openening output file named array_solution.csv
    fstream output_file;
    output_file.open("array_solution.csv", ios::out); // overwrites to the file, if does not exist it creates and writes.
    if(!output_file.is_open()){
        cout << "output file cannot be opened\n";
    }
    //cout << "output file opened succesfully\n";
    output_file << "Employee_ID;Salary;Department\n";
    
    // writing the csv file from our employee array.
    Employee curr_employee;
    int curr_id, curr_salary, curr_department;
    for(int i = 0; i < employee_size; i++){
        curr_employee = employee_ptr[i];
        curr_id = curr_employee.get_id();
        curr_salary = curr_employee.get_salary();
        curr_department = curr_employee.get_department();

        output_file << curr_id << ";" << curr_salary << ";" << curr_department << "\n";
    }
    delete[] employee_ptr;
    output_file.close();

    return 0;
}