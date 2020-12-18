
// C++ implementation of the approach 
#include <bits/stdc++.h> 
using namespace std; 
  
// Structure for Trie 
struct Trie { 
    bool isEndOfWord; 
    unordered_map<char, Trie*> map; 
    string meaning; 
}; 
  
// Function to create a new Trie node 
Trie* getNewTrieNode() 
{ 
    Trie* node = new Trie; 
    node->isEndOfWord = false; 
    return node; 
} 
  
// Function to insert a word with its meaning 
// in the dictionary built using a Trie 
void insert(Trie*& root, const string& str, 
            const string& meaning) 
{ 
  
    // If root is null 
    if (root == NULL) 
        root = getNewTrieNode(); 
  
    Trie* temp = root;
    int len = str.length();
    for (int i = 0; i < len; i++) { 
        char x = str[i]; 
  
        // Make a new node if there is no path 
        if (temp->map.find(x) == temp->map.end()) 
            temp->map[x] = getNewTrieNode(); 
  
        temp = temp->map[x]; 
    } 
  
    // Mark end of word and store the meaning 
    temp->isEndOfWord = true; 
    temp->meaning = meaning; 
} 
  
// Function to search a word in the Trie 
// and return its meaning if the word exists 
string getMeaning(Trie* root, const string& word) 
{ 
  
    // If root is null i.e. the dictionary is empty 
    if (root == NULL) 
        return ""; 
  
    Trie* temp = root;

    int word_length = word.length();
    // Search a word in the Trie 
    for (int i = 0; i < word_length; i++) { 
        temp = temp->map[word[i]]; 
        if (temp == NULL) 
            return ""; 
    } 
  
    // If it is the end of a valid word stored 
    // before then return its meaning 
    if (temp->isEndOfWord) 
        return temp->meaning; 
    return ""; 
} 
  
// Driver code 
int main() 
{ 
    Trie* root = NULL; 
  
    // Build the dictionary 
    insert(root, "language", "the method of human communication"); 
    insert(root, "computer", "A computer is a machine that can be instructed to carry out sequences of arithmetic or logical operations automatically via computer programming"); 
    insert(root, "map", "a diagrammatic representation of an area"); 
    insert(root, "book", "a written or printed work consisting of pages glued or sewn together along one side and bound in covers."); 
    insert(root, "science", "the intellectual and practical activity encompassing the systematic study of the structure and behaviour of the physical and natural world through observation and experiment."); 
  
    string str = "map"; 
    cout << getMeaning(root, str); 
  
    return 0; 
} 