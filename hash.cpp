#include "hash.h"

LinkedList::LinkedList() {
    head = nullptr;
}

LinkedList::~LinkedList() {
    Node *curr = head;
    while (head != nullptr) {
        head = head->next;
        delete curr;
        curr = head;
    }
    delete head;
}

void LinkedList::push_front(player data) {
      Node *new_node = new Node(data);
      new_node->next = head;
      head = new_node;
}

void LinkedList::push_back(player data) {
    Node *new_node = new Node(data);
    new_node->next = nullptr;
    if (head == nullptr) {
        head = new_node;
    } else {
        Node *curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}

bool LinkedList::search(string item) {
    Node *curr = head;
	while (curr != nullptr) {
	    if (curr->data.name == item){
            return true;
        }
		curr = curr->next; // update curr
	}
    return false;
}

player LinkedList::find(string item){
    Node *curr = head;
	while (curr != nullptr) {
	    if (curr->data.name == item){
            return curr->data;
        }
		curr = curr->next; // update curr
	}
    player p;
    p.name = "NOT FOUND";
    return p;
}

player LinkedList::remove_front() {
    if (head == nullptr) {
        throw std::runtime_error("LinkedList is empty");
    }
    player data = head->data;
    Node *temp = head;
    head = head->next;
    delete temp;
    return data;
}

void LinkedList::print() {
    Node *curr = head;
	while (curr != nullptr) {
	    cout << curr->data.name << " ";
		curr = curr->next; // update curr
	}
	cout << endl;
	return;
}

// constructor
Hash::Hash(): load{10}{
    arr = new LinkedList [load];
}

// overloaded constructor
Hash::Hash (unsigned int s): load{s}{
    arr = new LinkedList [load];
} 

// destructor
Hash::~Hash(){
    delete[] arr;
}

// hash function
unsigned int Hash::hash(const string & key){
    unsigned int hashVal = 0;
    for (char ch: key){
        hashVal = 37*hashVal + ch;
    }
    return hashVal % load;
} 

// adds item to hash table
void Hash::add(player p){
    string s = p.name;
    convert(s);
    unsigned int location = hash(s);
    arr[location].push_front(p);
    return;
} 

// finds string in dictionary, returns true if found
bool Hash::search(string s){
    convert(s);
    unsigned int location = hash(s);
    return (arr[location]).search(s); // search linked list at location
}

// converts string to only alphanum
void convert(string& s){
    // remove non '-' punctuation
    for (int i = 0; i < s.length(); i++){
        if (!isalnum(s[i])){
            s.erase(i, 1);
            i--;
        }
    }
    // if first and second characters are upper make everything lower
    if (std::isupper(s[0])){
        for (char& c : s){
            c = std::tolower(c);
        }
    }
    return;
}

// returns a player in the hashtable
player Hash::find(string s){
    convert(s);
    unsigned int location = hash(s);
    return (arr[location]).find(s);
}

// prints contents of table
void Hash::print(){
    for (int i = 0; i < load; i++){
        cout << i << "| ";
	    //arr[i].print();
    }
} 