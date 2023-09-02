#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cctype>

using std::ifstream;
using std::ostream;
using std::cout;
using std::endl;
using std::string;

#ifndef Hash_H
#define Hash_H

struct player{
    int rank;
    string name;
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

    friend ostream &operator<<(ostream &out, const player &p) {
        out << "Rank: " << p.rank
        << ", Name: " << p.name
        << ", Team: " << p.team
        << ", Position: " << p.position
        << ", Age: " << p.age
        << ", Games Played: " << p.gamesPlayed
        << ", Games Started: " << p.gamesStarted
        << ", Passes Completed: " << p.passesCompleted
        << ", Passes Attempted: " << p.passesAttempted
        << ", Pass Yards: " << p.passYards
        << ", Pass TDs: " << p.passTDS
        << ", Interceptions: " << p.interceptions
        << ", Rush Attempts: " << p.rushAttempts
        << ", Rush Yards: " << p.rushYards
        << ", Rush Yards Per Attempt: " << p.rushYardsPerAttempt
        << ", Rush TDs: " << p.rushTDS
        << ", Targets: " << p.targets
        << ", Receptions: " << p.receptions
        << ", Reception Yards: " << p.recYards
        << ", Reception Yards Per Attempt: " << p.recYardsPerAttempt
        << ", Reception TDs: " << p.recTDS
        << ", Fumbles: " << p.fumbles
        << ", Fumbles Lost: " << p.fumblesLost
        << ", Total TDs: " << p.totalTDS
        << ", Fantasy Points: " << p.fantasyPoints
        << ", PPR Points: " << p.PPRpoints
        << ", VBD: " << p.VBD
        << ", Position Rank: " << p.posRank;
    return out;
  }

};

class LinkedList {
  private:
    struct Node {
        player data;
        Node *next;

        Node(): data{}, next{nullptr} {}
	      Node(player data, Node *p = nullptr): data{data}, next{p}{}
    };
    Node *head;

  public:
    LinkedList(); // constructor
    ~LinkedList(); // destructor

    void push_front(player ); // adds item to the front
    void push_back(player ); // adds item to the back
    bool search(string ); // searches list for item
    player find(string ); // returns player in list

    player remove_front(); // removes front item

    void print(); // prints contents of list
};

class Hash{
    private:
        //unique_ptr<LinkedList<string> []> arr = nullptr;
        LinkedList *arr;
        unsigned int load;

    public:
        Hash(); // constructor
        Hash (unsigned int ); // overloaded constructor
        ~Hash(); // destructor


        unsigned int hash(const string & ); // hash function
        void add(player ); // adds item to hash table
        bool search(string ); // finds string in table
        player find(string ); // returns a player in the hashtable

        void print(); // prints contents of table

};

void convert(string & );

#endif