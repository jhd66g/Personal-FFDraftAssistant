#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include "hash.h"
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <cstdlib>

using std::ifstream;
using std::cout;
using std::endl;
using std::cin;
using json = nlohmann::json;

void populateHash(Hash& ); // populates hash table w data from file
size_t WriteCallback(void* , size_t , size_t , string* ); // handle data received from server
void analyzeAndRankPlayers(player , player , player , player ); // outputs API response
string playerToString(const player& p); // converts player p to string


int main(int argc, char* argv[]){
    // open file and populate hash table
    Hash h;
    populateHash(h);
    bool again = true;
    string q1, q2, q3, q4, quit;
    player p1, p2, p3, p4;

    while (again){
        cout << "Player 1: ";
        std::getline(cin, q1);

        cout << "Player 2: ";
        std::getline(cin, q2);

        cout << "Player 3: ";
        std::getline(cin, q3);

        cout << "Player 4: ";
        std::getline(cin, q4);
        cout << endl;

        p1 = h.find(q1);
        p2 = h.find(q2);
        p3 = h.find(q3);
        p4 = h.find(q4);

        analyzeAndRankPlayers(p1, p2, p3, p4);

        cout << "Press q to quit or c to continue ";
        std::getline(cin, quit);
        if ((quit == "q") || (quit == "Q")){
            again = false;
        }
    }

    return 0;
}

void populateHash(Hash& h){
    ifstream file;
    string fileName = "FFStats2022.txt";
    file.open(fileName);
    if (file.fail()){
        throw std::runtime_error("file broken :(");
    }
    int size {499};

    // player attributes
    int rank;
    string fName;
    string lName;
    string team;
    string position;
    int age;
    int gamesPlayed;
    int gamesStarted;
    int passesCompleted;
    int passesAttempted;
    int passYards;
    int passTDS;
    int interceptions;
    int rushAttempts;
    int rushYards;
    double rushYardsPerAttempt;
    int rushTDS;
    int targets;
    int receptions;
    int recYards;
    double recYardsPerAttempt;
    int recTDS;
    int fumbles;
    int fumblesLost;
    int totalTDS;
    int fantasyPoints;
    double PPRpoints;
    int VBD;
    int posRank;
    player p;
    while (file >> rank >> fName >> lName >> team >> position >> age >> gamesPlayed >> gamesStarted
    >> passesCompleted >> passesAttempted >>passYards >> passTDS >> interceptions >> rushAttempts >> rushYards 
    >> rushYardsPerAttempt >> rushTDS >> targets >> receptions >> recYards >> recYardsPerAttempt >> recTDS
    >> fumbles >> fumblesLost >>totalTDS >> fantasyPoints >> PPRpoints >> VBD >> posRank){
        p.rank = rank;
        string name = fName + lName;
        convert(name);
        p.name = name;
        p.team = team;
        p.position = position;
        p.age = age;
        p.gamesPlayed = gamesPlayed;
        p.gamesStarted = gamesStarted;
        p.passesCompleted = passesCompleted;
        p.passesAttempted = passesAttempted;
        p.passYards = passYards;
        p.passTDS = passTDS;
        p.interceptions = interceptions;
        p.rushAttempts = rushAttempts;
        p.rushYards = rushYards;
        p.rushYardsPerAttempt = rushYardsPerAttempt;
        p.rushTDS = rushTDS;
        p.targets = targets;
        p.receptions = receptions;
        p.recYards = recYards;
        p.recYardsPerAttempt = recYardsPerAttempt;
        p.recTDS = recTDS;
        p.fumbles = fumbles;
        p.fumblesLost = fumblesLost;
        p.totalTDS = totalTDS;
        p.fantasyPoints = fantasyPoints;
        p.PPRpoints = PPRpoints;
        p.VBD = VBD;
        p.posRank = posRank;
        h.add(p);
    }
    return;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

void analyzeAndRankPlayers(player p1, player p2, player p3, player p4) {
    const char* key = std::getenv("OPENAI_API_KEY");
    if (!key) {
        std::cerr << "Error: OPENAI_API_KEY environment variable not set!" << std::endl;
        exit(1);
    }
    string apiKey = key;
    string baseUrl = "https://api.openai.com/v1/chat/completions";
    string response;

    CURL* curl = curl_easy_init();
    if (curl) {
        json requestData;

        requestData["model"] = "gpt-3.5-turbo";
        requestData["messages"] = {
            {
                {"role", "system"},
                {"content", "You are a helpful assistant that analyzes NFL players' stats in order to predict season-long fantasy football outcomes. Given 2022-2023 statistics you formulate a player's value for a half-ppr ESPN league so that the user can gain a competitive edge on traditional rankings. Please value positional value in addition to the key statistics that offer insight into fantasy scoring outside of just yard, tds, and receptions. In addition to the rankings, please provide a 2-3 sentence explanation on your thought process."}
            },
            {
                {"role", "user"},
                {"content", 
                 "Rank these players in terms of their draft value for the 2023-2024 NFL fantasy football season: \n" 
                 + playerToString(p1) + "\n" + playerToString(p2) + "\n" + playerToString(p3) + "\n" + playerToString(p4)
                }
            }
        };
        requestData["temperature"] = 0;

        string requestDataStr = requestData.dump().c_str();

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestDataStr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestDataStr.length());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);

        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        cout << "HTTP Response Code: " << response_code << endl;

        if (res != CURLE_OK) {
            std::cerr << "Curl request failed: " << curl_easy_strerror(res) << endl;
        } 
        else {
            json jresponse;
            try {
                jresponse = json::parse(response);

                // Check for API error
                if (jresponse.contains("error")) {
                    string errorMessage = jresponse["error"]["message"].get<string>();
                    std::cerr << "API Error: " << errorMessage << endl;
                }
                // Check for the expected response format
                else if (jresponse.contains("choices") && jresponse["choices"].is_array() && !jresponse["choices"].empty() && 
                        jresponse["choices"][0].contains("message") && jresponse["choices"][0]["message"].contains("content")){
                    cout << jresponse["choices"][0]["message"]["content"].get<string>() << endl;
                } else {
                    std::cerr << "Unexpected response format!" << endl;
                }

            } catch(const json::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << endl;
            }
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

string playerToString(const player& p) {
    std::ostringstream oss;
    oss << "Rank: " << p.rank << "\n";
    oss << "Name: " << p.name << "\n";
    oss << "Team: " << p.team << "\n";
    oss << "Position: " << p.position << "\n";
    oss << "Age: " << p.age << "\n";
    oss << "Games Played: " << p.gamesPlayed << "\n";
    oss << "Games Started: " << p.gamesStarted << "\n";
    oss << "Passes Completed: " << p.passesCompleted << "\n";
    oss << "Passes Attempted: " << p.passesAttempted << "\n";
    oss << "Pass Yards: " << p.passYards << "\n";
    oss << "Pass TDs: " << p.passTDS << "\n";
    oss << "Interceptions: " << p.interceptions << "\n";
    oss << "Rush Attempts: " << p.rushAttempts << "\n";
    oss << "Rush Yards: " << p.rushYards << "\n";
    oss << "Rush Yards Per Attempt: " << p.rushYardsPerAttempt << "\n";
    oss << "Rush TDs: " << p.rushTDS << "\n";
    oss << "Targets: " << p.targets << "\n";
    oss << "Receptions: " << p.receptions << "\n";
    oss << "Reception Yards: " << p.recYards << "\n";
    oss << "Yards Per Reception Attempt: " << p.recYardsPerAttempt << "\n";
    oss << "Reception TDs: " << p.recTDS << "\n";
    oss << "Fumbles: " << p.fumbles << "\n";
    oss << "Fumbles Lost: " << p.fumblesLost << "\n";
    oss << "Total TDs: " << p.totalTDS << "\n";
    oss << "Standard Fantasy Points: " << p.fantasyPoints << "\n";
    oss << "PPR Points: " << p.PPRpoints << "\n";
    oss << "VBD: " << p.VBD << "\n";
    oss << "Position Rank: " << p.posRank << "\n";
    
    return oss.str();
}
