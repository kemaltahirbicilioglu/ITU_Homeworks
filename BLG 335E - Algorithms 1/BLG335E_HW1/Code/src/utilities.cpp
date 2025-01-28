#include "tweet.h"

// creates a Tweet struct from the read line, 
// input format: tweetID,retweet_count,favorite_count
void extractData(Tweet &extract, std::string data)
{
    std::stringstream ss(data);
    std::string temp;

    std::getline(ss, temp, ','); // Extract the tweetID
    extract.tweetID = std::stoll(temp);

    std::getline(ss, temp, ','); // Extract the retweetCount
    extract.retweetCount = std::stoi(temp);

    std::getline(ss, temp); // Extract the favoriteCount
    extract.favoriteCount = std::stoi(temp);
}


std::vector<Tweet> readTweetsFromFile(const std::string& filename) 
{
    std::vector<Tweet> tweets;
    std::ifstream inputStream(filename, std::ios_base::in);
    if(!inputStream.is_open()){
        std::cerr << "input file " << filename << " cannot be opened\n";
        return tweets;
    }
    std::cerr << "input file " << filename << " is opened " << "\n";

    std::string line;
    getline(inputStream, line); // initial line

    Tweet currentData;
    while(getline(inputStream, line)){
        extractData(currentData, line);
        tweets.push_back(currentData);
    } 
    inputStream.close();

    return tweets;
}


void writeTweetsToFile(const std::string& filename, const std::vector<Tweet>& tweets) 
{
    std::ofstream outputStream(filename, std::ios_base::out);
    if(!outputStream.is_open()){
        std::cerr << "output file " << filename << " cannot be opened\n";
        return;
    }
    std::cerr << "output file " << filename << " is opened " << "\n";

    outputStream << "tweetID,retweet_count,favorite_count\n"; // initial line
    for(const auto &tweet : tweets){
        outputStream << tweet.tweetID << "," << tweet.retweetCount << "," << tweet.favoriteCount << "\n";
    }
    outputStream.close();
}
