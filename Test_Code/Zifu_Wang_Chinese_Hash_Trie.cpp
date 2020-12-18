#include <iostream>
#include <map>
#include <string>
#include <vector>
#define CH_SIZE 3  //汉字大小linux为3，win为2
using namespace std;

struct trie_node {
  trie_node() { count = 0; }
  int count;  // number of strings that end with this character
  map<string, trie_node *> child;  // key is the current character, value is the
                                   // pointer of the following character
};

class trie {
 public:
  trie();
  ~trie();
  void insert_str(string str);                // Inser strings
  trie_node *search_str(string str);          // Look up strings
  trie_node *search_str_pre(string str_pre);  // Look up prefix of strings
  void delete_str(string str);                // Delete strings
  vector<string> get_str_pre(
      string str);  // Return strings with str to be its prefix
  void clear();     // Clear up
 private:
  void add_str(trie_node *root, string pre_str,
               vector<string>
                   &ret);  // Add strings with pre_str to be its prefix into ret
 private:
  struct trie_node *root;
};

trie::trie() { root = new trie_node(); }

trie::~trie() {}

// Insert strings
void trie::insert_str(string str) {
  if (root == NULL || str == "") {
    return;
  }
  trie_node *cur_node = root;

  for (int i = 0; i < str.size();) {
    string sub_str = str.substr(i, CH_SIZE);
    map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);

    if (iter ==
        cur_node->child.end())  // If not found in the map, then insert new node
    {
      trie_node *tmp_node = new trie_node();
      cur_node->child.insert(pair<string, trie_node *>(sub_str, tmp_node));
      cur_node = tmp_node;
    } else  // If found, value will be the pointer that points to the next node
    {
      cur_node = iter->second;
    }
    i = i + CH_SIZE;
  }

  cur_node->count++;
}

// Delete strings
void trie::delete_str(string str) {
  trie_node *find_node = search_str(str);

  if (find_node) {
    find_node->count--;
  }
}

// Look up prefix
trie_node *trie::search_str_pre(string str) {
  if (str == "") {
    return root;
  }
  if (NULL == root) {
    return NULL;
  }

  trie_node *cur_node = root;

  int i;
  for (i = 0; i < str.size();) {
    string sub_str = str.substr(i, CH_SIZE);
    map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);

    if (iter == cur_node->child.end()) {
      return NULL;
    } else {
      cur_node = iter->second;
    }
    i = i + CH_SIZE;
  }

  if (i == str.size()) {
    return cur_node;
  } else {
    return NULL;
  }
}

// Look up strings
trie_node *trie::search_str(string str) {
  trie_node *find_pre_node = search_str_pre(str);

  if (find_pre_node != NULL) {
    if (find_pre_node->count == 0) {
      return NULL;
    } else {
      return find_pre_node;
    }
  }
}

// Clear up
void trie::clear() {
  vector<trie_node *> que;
  que.push_back(root);

  while (!que.empty()) {
    for (map<string, trie_node *>::iterator iter = root->child.begin();
         iter != root->child.end(); iter++) {
      que.push_back(iter->second);
    }

    trie_node *del_node = que.front();
    que.pop_back();

    delete del_node;
  }
}

// Add strings with pre_str to be its prefix into ret
void trie::add_str(trie_node *root, string pre_str, vector<string> &ret) {
  for (map<string, trie_node *>::iterator iter = root->child.begin();
       iter != root->child.end(); iter++) {
    add_str(iter->second, pre_str + iter->first, ret);
  }

  if (root->count != 0) {
    ret.push_back(pre_str);
  }
}

// Return strings with str to be its prefix
vector<string> trie::get_str_pre(string str) {
  vector<string> ret;
  trie_node *find_node = search_str_pre(str);

  if (find_node != NULL) {
    add_str(find_node, str, ret);
  }

  return ret;
}

int main() {
  trie t;
  int n;
  string str;
  vector<string> ret;

  cout << "please input the num of the dictionary:" << endl;
  cin >> n;

  for (int i = 0; i < n; i++) {
    cin >> str;
    t.insert_str(str);
  }

  cout << "please input the key word:" << endl;
  cin >> str;

  ret = t.get_str_pre(str);

  for (vector<string>::iterator iter = ret.begin(); iter != ret.end(); iter++) {
    cout << *iter << endl;
  }

  return 0;
}