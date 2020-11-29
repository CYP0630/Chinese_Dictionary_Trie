
#define BASE 2
#define BEGIN_LETTER '0'
typedef struct Node {
    int flag;
    unsigned char *str;
    struct Node *next[BASE];
} Node, *Trie;

#define BASE 2
#define BEGIN_LETTER '0'
Node *get_new_node() {
    Node *p = (Node *)calloc(sizeof(Node), 1);
    p->flag = 0;
    return p;
}

void clear(Trie root) {
    if (root == NULL) return ;
    for (int i = 0; i < BASE; i++) {
        clear(root->next[i]);
    }
    free(root);
    return ;
}

Node *insert(Trie root, const char *str, UC *org) {
    if (root == NULL) root = get_new_node();
    Node *p = root;
    for (int i = 0; str[i]; i++) {
        int ind = str[i] - BEGIN_LETTER;
        if (p->next[ind] == NULL) p->next[ind] = get_new_node();
        p = p->next[ind];
    }
    p->flag = 1;
    p->str = (unsigned char *)calloc(512, sizeof(unsigned char));
    memcpy(p->str, org, sizeof(org) * 2);
    return root;
}

int search(Trie root, const char *str) {
    Node *p = root;
    int i = 0;
    while (p && str[i]) {
        int ind = str[i++] - BEGIN_LETTER;
        p = p->next[ind];
    }
    return (p && p->flag);
}

void get_random_string(char *str) {
    int len = rand() % 10;
    for (int i = 0; i < len; i++) str[i] = BEGIN_LETTER + rand() % BASE;
    str[len] = 0;
    return ;
}

void output(Trie root, char *str, int level) {
    if (root == NULL) return ;
    str[level] = 0;
    if (root->flag) {
        printf("find word : %s\n", str);
    }
    for (int i = 0; i < BASE; i++) {
        if (root->next[i] == NULL) continue;
        str[level] = i + BEGIN_LETTER;
        output(root->next[i], str, level + 1);
    }
    return ;
}

void match(Trie root, char *text){
    Trie p = root;
    for (int i = 0; text[i]; ++i) {
        int j = i;
        p = root;
        while (p && p->next[text[j] - BEGIN_LETTER]) {
            p = p->next[text[j] - BEGIN_LETTER]; 
            if(p && p->flag) {
                printf("FIND WORD : %s\n", p->str);
                break;
            }
            j++;
            if (text[j] == '\0') return;
        }
    }
}