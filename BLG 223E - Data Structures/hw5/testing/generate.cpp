#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

class Employee {
public:
  Employee(int id = 0, int salary = 0, int department = 0) {
    this->id = id;
    this->salary = salary;
    this->department = department;
  }
  int id;
  int salary;
  int department;
};

int generate_id(int size){
  return (rand() % size) + 1;
}

int generate_salary() {
  return rand() % 9000 + 1000;
}

int generate_department() {
  return rand() % 9 + 1;
}

void shuffle_array(vector<Employee>& dataset) {
  int idx1, idx2;
  for (unsigned int i = 0; i < dataset.size(); i++) {
    idx1 = (rand() * RAND_MAX + rand()) % dataset.size();
    idx2 = i;
    swap(dataset[idx1], dataset[idx2]);
  }
}

string get_file_name(int size, string name){
  return "./dataset/" + to_string(size/1000) + "k_" + name; 
}

void create_operation(int size, int count){
  string add_file = get_file_name(size, "add_file.txt");
  string update_file = get_file_name(size, "update_file.txt");
  string delete_file = get_file_name(size, "delete_file.txt");
  
  ofstream add_stream(add_file);
  ofstream update_stream(update_file);
  ofstream delete_stream(delete_file);
 
  for(int i = 0; i < count; i++){
    add_stream << "ADD" << ";" << generate_salary() << ";" << generate_department() << "\n";  
  }

  for(int i = 0; i < count; i++){
    update_stream << "UPDATE" << ";" << generate_id(size) << ";" << generate_salary() << ";" << generate_department() << "\n";  
  }

  for(int i = 0; i < count; i++){
    delete_stream << "DELETE" << ";" << generate_id(size) << "\n";  
  }  

  add_stream.close();
  update_stream.close();
  delete_stream.close();

}

void create_employees(int size){
  vector<Employee> dataset(size);
  
  for (int i = 0; i < size; i++) {
    dataset[i] = Employee(i+1, generate_salary(), generate_department());
  }
  shuffle_array(dataset);

  string output_file = get_file_name(dataset.size(), "employee.csv");

  ofstream output_stream(output_file);
  output_stream << "Employee_ID;Salary;Department\n";
  for (auto& e : dataset) {
    output_stream << e.id << ";" << e.salary << ";" << e.department << '\n';
  }
  output_stream.close();
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc < 2) return 1;

  int dataset_size = stoi(argv[1]);
  
  create_employees(dataset_size);
  create_operation(dataset_size, 100);
  
  return 0;
}
