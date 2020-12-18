#include <string>
#include <cstring> 
#include <map>  
#include <vector>  
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <windows.h>
#define CH_SIZE 2//汉字大小linux为3，win为2
using namespace std;  
  
struct trie_node  
{  
    trie_node()  
    {  
        count = 0;  
    }  
    int count;
    map<string, trie_node *> child;
};  
  
class trie  
{  
public:  
    trie();  
    ~trie();  
    void insert_str(string str); 
    trie_node *search_str(string str);
    trie_node *search_str_pre(string str_pre);
    void delete_str(string str);
    vector<string> get_str_pre(string str);
    vector<string> find_longest_str(string c_str); 
    void clear();
private:  
    void add_str(trie_node *root, string pre_str, vector<string> &ret); 
private:  
    struct trie_node *root;  
};  
  
trie::trie()  
{  
    root = new trie_node();  
}  
  
trie::~trie()  
{  
} 

void trie::insert_str(string str)  
{  
    if (root == NULL || str == "")  
    {  
        return ;  
    }  
    trie_node *cur_node = root;
    int size = str.size();
    for (int i = 0; i < size;)
    {
        string sub_str = str.substr(i, CH_SIZE);
        map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);

        if (iter == cur_node->child.end())
        {  
            trie_node *tmp_node = new trie_node();  
            cur_node->child.insert(pair<string, trie_node *>(sub_str, tmp_node));  
            cur_node = tmp_node;  
         }  
         else 
         {  
             cur_node = iter->second;  
         }  
         i = i + CH_SIZE ;  
     }  
  
     cur_node->count++;  
}  
   
void trie::delete_str(string str)  
{  
    trie_node *find_node = search_str(str);  
  
    if (find_node)  
    {  
        find_node->count--;  
    }  
}  

trie_node * trie::search_str_pre(string str)  
{  
    if (str == "")  
    {  
        return root;  
    }  
    if (NULL == root )  
    {  
        return NULL;  
    }  
  
    trie_node *cur_node = root;  
  
    int i;
    int size = str.size();
    for ( i = 0; i < size; )  
    {  
        string sub_str = str.substr(i, CH_SIZE);  
        map<string, trie_node *>::iterator iter = cur_node->child.find(sub_str);  
  
        if (iter == cur_node->child.end())  
        {  
            return NULL;  
        }  
        else  
        {  
             cur_node = iter->second;  
        }  
        i = i + CH_SIZE;  
    }  
  
    if (i == size)  
    {  
        return cur_node;  
    }  
    else  
    {  
        return NULL;  
    }  
}  

trie_node * trie::search_str(string str)  
{  
    trie_node * find_pre_node = search_str_pre(str);  
  
    if (find_pre_node != NULL)  
    {  
        if (find_pre_node->count == 0)  
        {  
            return NULL;  
        }  
        else  
        {  
            return find_pre_node;  
        }  
    }
    else
        return NULL;
}  
 
void trie::clear()  
{  
    vector<trie_node *> que;  
    que.push_back(root);  
  
    while (!que.empty())  
    {  
        for (map<string, trie_node *>::iterator iter = root->child.begin(); iter != root->child.end(); iter++)  
        {  
            que.push_back(iter->second);  
        }  
  
        trie_node *del_node = que.front();  
        que.pop_back();
 
        delete del_node;  
    }  
}  
 
void trie::add_str(trie_node *root, string pre_str, vector<string> &ret)  
{  
    for (map<string, trie_node *>::iterator iter = root->child.begin(); iter != root->child.end(); iter++)  
    {  
         add_str(iter->second, pre_str + iter->first, ret);  
    }  
  
    if (root->count != 0)  
    {  
         ret.push_back(pre_str);  
    }  
}  
  
vector<string> trie::get_str_pre(string str)  
{  
    vector<string> ret;  
    trie_node *find_node = search_str_pre(str);  
  
    if (find_node != NULL)  
    {  
        add_str(find_node, str, ret);  
    }  
  
    return ret;  
}

vector<string> trie::find_longest_str(string c_str)  
{  
    vector<string> ret;
    string first_char;
    int size = c_str.size();
    for (int i = 0; i < size;){
        string substr = c_str.substr(i, CH_SIZE);
        int word_len = CH_SIZE;
        if(search_str_pre(substr) == NULL){
            i += CH_SIZE;
        }
        else{
            while (search_str_pre(substr) != NULL && (i + word_len) <= size)
            {
                word_len += CH_SIZE;
                substr = c_str.substr(i, word_len);
            }
            substr = c_str.substr(i, word_len - CH_SIZE);
            ret.push_back(substr);
            i += word_len - CH_SIZE;
        }
        // cout << c_str.substr(i, CH_SIZE) << endl;
    }
    return ret;
}

string UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;
 
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);
 
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);
 
	result = szRes;
	delete[]strSrc;
	delete[]szRes;
 
	return result;
}

int main()
{
    trie t;
    // int n;
    string str;
    vector<string> ret;

    ifstream file("C:/vscode/CPE593FinalProject/words.txt");
	string temp;
	while (getline(file, temp)){
        string str = UTF8ToGB(temp.c_str()).c_str();
        stringstream input(str);
        string word;
        input >> word;
        t.insert_str(word);
    }

    ifstream split_str("C:/vscode/CPE593FinalProject/Split_words.txt");
    vector<string> ss;
    string s1;
    while (getline(split_str, s1)){
        string str = UTF8ToGB(s1.c_str()).c_str();
        stringstream input(str);
        string sen;
        input >> sen;
        ss.push_back(sen);
    }

    int ops = 1;
    switch (ops)
    {
    case 1:
        for (vector<string>::iterator i = ss.begin(); i != ss.end(); i++)
        {
            ret = t.find_longest_str(*i);
            for (vector<string>::iterator iter = ret.begin(); iter != ret.end(); iter++)
                cout << *iter << " / ";
            cout << endl;
        }
        break;
    
    case 2:
        cout << "please input the key word:" << endl;
        cin >> str;
 
        ret = t.get_str_pre(str);

        cout << "word include in dictionary:" << endl;
        for (vector<string>::iterator iter = ret.begin(); iter != ret.end(); iter++)
        {
            cout << *iter << endl;
        }
        break;
    }
    return 0;
}