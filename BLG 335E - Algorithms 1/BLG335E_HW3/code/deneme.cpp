#include <bits/stdc++.h>
using namespace std;

// Publisher class to hold sales data
class Publisher {
public:
    string name;
    float na_sales = 0;
    float eu_sales = 0;
    float others_sales = 0;
};

// Function to extract publisher data from a CSV line
vector<string> extractPublisherData(const string& line) {
    stringstream ss(line);
    string temp;
    vector<string> publisher_data;

    getline(ss, temp, ','); // Name
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // Platform
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // Year_of_Release
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // Publisher name
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // NA Sales
    publisher_data.push_back(temp);

    getline(ss, temp, ','); // EU Sales
    publisher_data.push_back(temp);

    getline(ss, temp); // Other Sales
    publisher_data.push_back(temp);

    return publisher_data;
}

// Function to sum sales for a specific publisher up to a certain year
Publisher sumSalesUpToYear(const string& filename, const string& targetPublisher, int targetYear) {
    ifstream file(filename);
    string line;
    Publisher result;

    // Skip the header
    getline(file, line);

    // Loop through each line in the file
    while (getline(file, line)) {
        vector<string> data = extractPublisherData(line);

        string publisherName = data[3]; // Publisher name
        int year = stoi(data[2]);      // Year of release
        float na_sales = stof(data[4]); // NA Sales
        float eu_sales = stof(data[5]); // EU Sales
        float other_sales = stof(data[6]); // Other Sales

        // Only process if the publisher matches and year is <= target year
        if (publisherName == targetPublisher && year < targetYear) {
            result.na_sales += na_sales;
            result.eu_sales += eu_sales;
            result.others_sales += other_sales;
        }
    }

    file.close();
    return result;
}

// Function to print the sales data of a publisher
void printPublisherSales(const Publisher& publisher) {
    cout << "Total NA Sales: " << publisher.na_sales << " million" << endl;
    cout << "Total EU Sales: " << publisher.eu_sales << " million" << endl;
    cout << "Total Other Sales: " << publisher.others_sales << " million" << endl;
}


// Function to normalize line endings (removes '\r' if present)
std::string normalizeLineEndings(const std::string& line) {
    std::string normalized = line;
    if (!normalized.empty() && normalized.back() == '\r') {
        normalized.pop_back();
    }
    return normalized;
}

// Function to trim leading and trailing whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to remove BOM if present
void removeBOM(std::ifstream& file) {
    char bom[3];
    file.read(bom, 3);
    if (!(bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF')) {
        file.seekg(0); // Rewind if no BOM
    }
}

// Main function to check if two files are identical
bool areFilesIdentical(const std::string& file1, const std::string& file2) {
    // Open both files
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    // Check if both files were opened successfully
    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Error opening one or both files." << std::endl;
        return false;
    }

    // Remove BOM from both files if present
    removeBOM(f1);
    removeBOM(f2);

    std::string line1, line2;
    // Compare line by line
    while (std::getline(f1, line1) && std::getline(f2, line2)) {
        // Normalize line endings and trim whitespace before comparing
        if (trim(normalizeLineEndings(line1)) != trim(normalizeLineEndings(line2))) {
            std::cout << file1 << ":\n" << line1 << "\n";
            std::cout << file2 << ":\n" << line2 << "\n";
            return false;
        }
    }

    // Check for any remaining lines in either file
    if (std::getline(f1, line1) || std::getline(f2, line2)) {
        std::cerr << "Files differ in length." << std::endl;
        return false;
    }

    // Files are identical
    return true;
}

int main() {
    string file_name1 = "aliemrepreorder.txt";
    string file_name2 = "benimpreorder.txt";
    
    if(areFilesIdentical(file_name1, file_name2)){
        cout << "files are identical\n";
    }else{
        cout << "not identical\n";
    }

    return 0;
}
