/*@Author
StudentName: Kemal Tahir Bıcılıoğlu
StudentID: 150210083
Date: 25.10.23
*/
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

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

void print_file(fstream &file){
    file.clear();
    file.seekg(0, ios::beg);
    string line;
    while(getline(file, line)){
        cout << line << "\n";
    }
}

int find_last_id(fstream &file, int *file_size){
    file.clear();
    file.seekg(0, ios::beg); // make sure it points to the beginning of the file
    
    string line, s_id;
    int index;
    int id = 0;
    int count = 0;
    getline(file, line); // discard the first line.
    while(getline(file, line)){
        count++;
        index = 0;
        s_id = "";
        while(line[index] != ';'){
            s_id += line[index];
            index++;
        }
        id = stoi(s_id);        
    }
    *file_size = count;
    return id; // last taken id is the last id of the employees file.
}

bool valid_id_check(fstream &file, int id){
    file.clear();
    file.seekg(0, ios::beg); // make sure it is pointing the beginning of the file.
    
    string line, s_id;
    int curr_id;
    getline(file, line); // discard the first line.
    while(getline(file, line)){
        int index = 0;
        s_id = "";
        while(line[index] != ';'){
            s_id += line[index];
            index++;
        }
        curr_id = stoi(s_id);
        // we got id.
        if(curr_id == id){
            return true;
        }
    }
    return false;
}

void copy_back_files(fstream &file1, fstream &file2){
    file1.clear();
    file2.clear();
    file1.seekg(0, ios::beg);
    file2.seekp(0,ios::beg);
    string line;
    while(getline(file1, line)){
        file2 << line << "\n";
    }
}

void add_employe_to_file(fstream &file, int id, int salary, int department){
    file.clear();
    file.seekp(0, ios::end);
    file << id << ";" << salary << ";" << department << "\n";
}

void update_employee(fstream &file, fstream &new_file, int id, int salary, int department){
    file.clear();
    file.seekg(0, ios::beg);
    string line, s_id;
    getline(file, line); // first line of the file.
    new_file << line << "\n";

    while(getline(file, line)){
        int index = 0;
        s_id = "";
        while(line[index] != ';'){
            s_id += line[index];
            index++;
        }
        int curr_id = stoi(s_id);
        if(curr_id == id){
            new_file << id << ";" << salary << ";" << department << "\n";
            continue;
        }
        new_file << line << "\n";
    }
}

void delete_employee_from_file(fstream &file, fstream &new_file, int id){
    file.clear();
    file.seekg(0, ios::beg);

    string line, s_id;
    getline(file, line);
    new_file << line << "\n"; // first line of the file.

    while(getline(file, line)){
        int index = 0;
        s_id = "";
        while(line[index] != ';'){
            s_id += line[index];
            index++;
        }
        int curr_id = stoi(s_id);
        // we got id
        if(curr_id == id){
            continue;  // do not write the id that will be deleted to the new_file.
        }
        new_file << line << "\n";
    }
}

int main(int argc, char *argv[]){
//clock_t start = clock();

    // argc argv...
    string data_file, operation_file;
    if(argc == 3){
        data_file = argv[1];
        operation_file = argv[2];
    }else{
        cout << "invalid argument count\n";
        return 1;
    }
    
    
    // opening files. finding last id and the size of the file.
    fstream data_fptr, operation_fptr;
    data_fptr.open(data_file, ios::in);
    operation_fptr.open(operation_file, ios::in);
    int file_size = 0;
    int last_id = find_last_id(data_fptr, &file_size);
    data_fptr.close(); // we are going to open it according to our purposes.
    

    string line, operation;
    int index = 0;
    while(getline(operation_fptr, line)){
        index = 0;
        operation = "";
        while(line[index] != ';'){
            operation += line[index];
            index++;
        }
        index++;

        if(is_equal_strings("ADD", operation)){
            //clock_t start = clock();

            data_fptr.open(data_file, ios::app);
            string s_salary, s_department;
            while(line[index] != ';'){
                s_salary += line[index];
                index++;
            }
            index++;
            while(line[index] != '\0'){
                s_department += line[index];
                index++;
            }
            int salary = stoi(s_salary);
            int department = stoi(s_department);
            // we got salary and department.
            last_id++;
            file_size++;
            add_employe_to_file(data_fptr, last_id, salary, department);
            data_fptr.close();
            
            //clock_t end = clock();
            //cout << "ADD: file solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }

        else if(is_equal_strings("UPDATE", operation)){
            //clock_t start = clock();

            string s_id, s_salary, s_department;
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
            int id = stoi(s_id);
            int salary = stoi(s_salary);
            int department = stoi(s_department);
            // we got datas.
            data_fptr.open(data_file, ios::in);
            bool is_id_valid = valid_id_check(data_fptr, id);
            if(!is_id_valid){
                cout << "ERROR: An invalid ID to update\n";
                data_fptr.close();
                continue;
            }
            fstream temp_fptr;
            string temp_file = "temp_file.csv";
            temp_fptr.open(temp_file, ios::out);

            update_employee(data_fptr, temp_fptr, id, salary, department);
            temp_fptr.close();
            data_fptr.close();
            
            data_fptr.open(data_file, ios::out); // to get rid of old file. when opening with an out mode, it will automatically delete the old datas.
            temp_fptr.open(temp_file, ios::in);

            copy_back_files(temp_fptr, data_fptr);
            temp_fptr.close();
            data_fptr.close();
            remove(temp_file.c_str());

            //clock_t end = clock();
            //cout << "UPDATE: file solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }

        else if(is_equal_strings("DELETE", operation)){
            //clock_t start = clock();

            if(file_size == 0){
                cout << "ERROR: There is no Employee\n";
                continue;
            }
            
            string s_id;
            int id;
            while(line[index] != '\0'){
                s_id += line[index];
                index++;
            }
            id = stoi(s_id);
            // we got id.

            data_fptr.open(data_file, ios::in);
            bool is_valid_id = valid_id_check(data_fptr, id);
            if(!is_valid_id){
                cout << "ERROR: An invalid ID to delete\n";
                data_fptr.close();
            }
            else{            
            fstream temp_fptr;
            string temp_file = "temp_file.csv";
            temp_fptr.open(temp_file, ios::out);

            delete_employee_from_file(data_fptr, temp_fptr, id);
            temp_fptr.close();
            data_fptr.close();
            
            data_fptr.open(data_file, ios::out); // to get rid of old file. when opening with an out mode, it will automatically delete the old datas.
            temp_fptr.open(temp_file, ios::in);

            copy_back_files(temp_fptr, data_fptr);
            temp_fptr.close();
            data_fptr.close();
            remove(temp_file.c_str());
            file_size--;
            }
            //clock_t end = clock();
            //cout << "DELETE: file solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";
        }else{
            cout << "invalid operation\n";
            return 1;
        }
        
    }
    
    operation_fptr.close();

//clock_t end = clock();
//cout << "main: file solution " << (double)(end-start)*1000 / CLOCKS_PER_SEC  << " miliseconds\n";

    return 0;
}