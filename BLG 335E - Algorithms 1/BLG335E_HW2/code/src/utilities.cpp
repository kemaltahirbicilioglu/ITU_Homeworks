#include "methods.h"


// creates a Item struct from the read line, 
// input format: age, type, origin, rarity
void extractData(Item &extract, std::string data)
{
    std::stringstream ss(data);
    std::string temp;

    std::getline(ss, temp, ','); // Extract the age
    extract.age = std::stoi(temp);

    std::getline(ss, temp, ','); // Extract the type
    extract.type = std::stoi(temp);

    std::getline(ss, temp, ','); // Extract the origin
    extract.origin = std::stoi(temp);

    std::getline(ss, temp); // Extract the rarity score
    extract.rarityScore = std::stod(temp);
}


std::vector<Item> readItemsFromFile(const std::string& filename) 
{
    std::vector<Item> Items;
    std::ifstream inputStream(filename, std::ios_base::in);
    if(!inputStream.is_open()){
        std::cerr << "input file " << filename << " cannot be opened\n";
        return Items;
    }
    std::cerr << "input file " << filename << " is opened " << "\n";

    std::string line;
    getline(inputStream, line); // initial line

    Item currentData;
    while(getline(inputStream, line)){
        extractData(currentData, line);
        Items.push_back(currentData);
    } 
    inputStream.close();

    return Items;
}

void writeItemsToFile(const std::string& filename, const std::vector<Item>& items) 
{
    std::ofstream outputStream(filename, std::ios_base::out);
    if(!outputStream.is_open()){
        std::cerr << "output file " << filename << " cannot be opened\n";
        return;
    }
    std::cerr << "output file " << filename << " is opened " << "\n";

    outputStream << "age,type,origin,rarity\n"; // initial line
    for(const auto &Item : items){
        outputStream << Item.age << "," << Item.type << "," << Item.origin << "," << Item.rarityScore << "\n"; // precision of the rarity ??
    }
    outputStream.close();
}
