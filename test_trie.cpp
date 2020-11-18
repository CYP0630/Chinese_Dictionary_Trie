#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class Trie {
   private:
    class Node {
       public:
        Node* next[26];
        bool isWord;
        Node() {
            // make sure all pointers point to nullptr
            for (int i = 0; i < 26; ++i) {
                next[i] = nullptr;
            }
            isWord = false;
        }
    };
    Node root;

   public:
    Trie() : root() {}
    ~Trie() {
        for (int i = 0; i < 26; ++i) {
            if (root.next[i]) {
                deleteHelper(root.next[i]);
            }
        }
    }
    // load the trie from a file
    bool load(const char dictionary[]) {
        ifstream dict(dictionary);
        if (!dict.is_open()) {
            return false;
        }
        string word;
        while (dict >> word) {
            add(word.c_str());
        }
        return true;
    }
    
    Trie(const Trie& orig) = delete;
    Trie& operator=(const Trie& orig) = delete;

    void add(const char word[]) {
        if (!isValid(word)) return;

        Node* node = &root;
        while (*word) {
            if (node->next[tolower(*word) - 'a'] == nullptr) {
                node->next[tolower(*word) - 'a'] = new Node();
            }
            node = node->next[tolower(*word) - 'a'];
            ++word;
        }
        node->isWord = true;
    }
    
    bool contains(const char word[]) const {
        if (!isValid(word)) return false;

        const Node* node = &root;
        while (*word) {
            node = node->next[tolower(*word) - 'a'];
            if (node == nullptr) {
                return false;
            }
            ++word;
        }
        return node->isWord;
    }

    bool containsPrefix(const char word[]) const;
    int printByPrefix(const char prefix[]) const {
        const char* word = prefix;
        if (!isValid(word)) return -1;

        const Node* node = &root;
        while (*word) {
            node = node->next[tolower(*word) - 'a'];
            if (node == nullptr) {
                return 0;
            }
            ++word;
        }

        string _prefix = string(prefix);
        return printHelper(_prefix, node);
    }
    void print() const {
        string prefix;
        printHelper(prefix, &root);
    }

   private:
    bool isValid(const char word[]) const {
        int len = 0;
        while (*word) {
            if (!islower(*word)) {
                return false;
            }
            ++len;
            ++word;
        }
        return len > 0;
    }

    int printHelper(string& prefix, const Node* node) const {
        int cnt = 0;

        if (node->isWord) {
            cout << prefix << endl;
            ++cnt;
        }
        for (int i = 0; i < 26; ++i) {
            if (node->next[i]) {
                prefix += (i + 'a');
                cnt += printHelper(prefix, node->next[i]);
                prefix.pop_back();
            }
        }

        return cnt;
    }
    void deleteHelper(Node* node) {
        for (int i = 0; i < 26; ++i) {
            if (node->next[i]) {
                deleteHelper(node->next[i]);
            }
        }
        delete node;
    }
};

int main() {
  char tmp[36];
  Trie dict;
  ifstream myfile("dictionary.txt");
  while (!myfile.eof()) {
    myfile.getline(tmp, 36);
    dict.add(tmp);
  }
  /*myfile.close();
  ifstream game("G:\\cppWorkSpace\\CPE593\\Assignment7\\boggle.dat");
  string size;
  game.getline(size, 1);
  char* bog = new char[size][size];*/
/*
  int size = 4;
  int size2 = 16;
  char bog[4][4] = {'a', 'c', 'e', 's', 't', 'b', 'd', 'r',
                    'y', 'k', 'e', 'p', 'l', 'g', 'n', 'm'};
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      const int size2 = size * size;
      char word[16];
      word[0] = bog[i][j];
      bool used[4][4] = {false};
      used[i][j] = true;
      int k = 1;
      printIsWord(bog, size, dict, i, j, word, size2, k, used);
    }
  }
*/

return 0;
}