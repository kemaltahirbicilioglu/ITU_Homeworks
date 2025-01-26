#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]){
    string file1_name, file2_name;
    if(argc == 3){
        file1_name = argv[1];
        file2_name = argv[2];
    }else{
        cout << "invalid argument count\n";
        return 1;
    }

    fstream fptr1;
    fstream fptr2;
    fptr1.open(file1_name, ios_base::in);
    if(!fptr1.is_open()){
        cout << "fptr1 cannot be opened\n";
        return 1;
    }      

    fptr2.open(file2_name, ios_base::in);

    if(!fptr2.is_open()){
        cout << "fptr2 cannot be opened\n";
        return 1;
    }

    string line1, line2;
    bool is_equal = true;
    int line_number = 0;
    while(getline(fptr1, line1)){

        getline(fptr2, line2);
        if(line1 != line2){
            is_equal = false;
            cout << "lines " << line_number << " are not equal \n";
            cout << "line1 : " << line1 << "\n";
            cout << "line2 : " << line2 << "\n";
            break;
        }
        line_number++;
    }
    if(is_equal){
        cout << "files are equal\n";
    }

    fptr1.close();
    fptr2.close();
    
    return 0;
}