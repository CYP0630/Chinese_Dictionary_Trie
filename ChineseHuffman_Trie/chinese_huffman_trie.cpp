/*************************************************************************
	> File Name: huffman.cpp
	> Author: sudingquan
	> Mail: 1151015256@qq.com
	> Created Time: 日  7/21 20:31:19 2019
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1000  //最大词长
#define MAX_N 1000 //最大词数
#define BASE 2
#define BASE_LETTER '0'
#define swap(a, b) { \
    __typeof(a) temp = a; \
    a = b, b = temp; \
}

typedef struct Node {
    unsigned char ch;
    double p;
    struct Node *next[2];
} Node;

typedef struct Code {
    unsigned char ch;
    char *str;
} Code;

typedef struct HuffmanTree {
    Node *root;
    int n;
    Code *codes;
} HuffmanTree;

typedef struct Data {
    unsigned char ch;
    double p;
} Data;

typedef struct trie_node {
    int flag;
    unsigned char *word;
    struct trie_node *next[BASE], *fail;
} trie_node;

Data arr[256];
int data[256];

struct word {
    unsigned char str[MAX_SIZE * 3];
    char huffman_code[10000];
} word[MAX_N];

int word_cnt = 0;
int character_cnt = 0;

//queue
typedef struct Queue {
    trie_node **data;
    int head, tail, size;
} Queue;

Queue *init_queue(int n) {
    Queue *q = (Queue *)calloc(sizeof(Queue), 1);
    q->data = (trie_node **)malloc(sizeof(trie_node *) * n);
    q->tail = q->head = 0;
    return q;
}

trie_node *front(Queue *q) {
    return q->data[q->head];
}

int empty(Queue *q) {
    return q->head == q->tail;
}

void push(Queue *q, trie_node *node) {
    q->data[q->tail++] = node;
    return ;
}

void pop(Queue *q) {
    if (empty(q)) return ;
    q->head++;
}

void clear_queue(Queue *q) {
    if (q == NULL) return ;
    free(q->data);
    free(q);
    return ;
}
//queue end

Node *getNewNode(Data *obj) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->ch = obj ? obj->ch : 0;
    p->p = obj ? obj->p : 0;
    p->next[0] = p->next[1] = NULL;
    return p;
}

HuffmanTree *getNewTree(int n) {
    HuffmanTree *tree = (HuffmanTree *)malloc(sizeof(HuffmanTree));
    tree->root = NULL;
    tree->n = n;
    tree->codes = (Code *)malloc(sizeof(Code) * n);
    return tree;
}

void insertOnce(Node **arr, int n) {
    for (int j = n; j >= 1; j--) {
        if (arr[j]->p > arr[j - 1]->p) {
            swap(arr[j], arr[j - 1]);
            continue;
        }
        break;
    }
    return ;
}

int extractCodes(Node *root, Code *arr, int k, int l, char *buff) {
    buff[l] = 0;
    if (root->next[0] == NULL && root->next[1] == NULL) {
        arr[k].ch = root->ch;
        arr[k].str = strdup(buff);
        return 1;
    }
    int delta = 0;
    buff[l] = '0';
    delta += extractCodes(root->next[0], arr, k + delta, l + 1, buff);
    buff[l] = '1';
    delta += extractCodes(root->next[1], arr, k + delta, l + 1, buff);
    return delta;
}

HuffmanTree *build(Data * arr, int n) {
    Node **nodes = (Node **)malloc(sizeof(Node *) * n);
    for (int i = 0; i < n; i++) {
        nodes[i] = getNewNode(arr + i);
    }
    for (int i = 1; i < n; i++) {
        insertOnce(nodes, i);
    }
    for (int i = n - 1; i >= 1; i--) {
        Node *p = getNewNode(NULL);
        p->next[1] = nodes[i];
        p->next[0] = nodes[i - 1];
        p->p = p->next[0]->p + p->next[1]->p;
        nodes[i - 1] = p;
        insertOnce(nodes, i - 1);
    }
    char *buff = (char *)malloc(sizeof(char) * n);
    HuffmanTree *tree = getNewTree(n);
    tree->root = nodes[0];
    extractCodes(tree->root, tree->codes, 0, 0, buff);
    free(nodes);
    free(buff);
    return tree;
}

trie_node *get_new_node() {
    trie_node *p = (trie_node *)calloc(sizeof(trie_node), 1);
    return p;
}

inline int code(char ch) { return ch - BASE_LETTER; }

void insert(trie_node *root, char *str, unsigned char *word) {
    trie_node *p = root;
    for (int i = 0; str[i]; i++) {
        if (p->next[code(str[i])] == NULL) p->next[code(str[i])] = get_new_node();
        p = p->next[code(str[i])];
    }
    p->flag = 1;
    p->word = (unsigned char *)malloc(sizeof(unsigned char) * MAX_SIZE * 3);
    memcpy(p->word, word, sizeof(unsigned char) * MAX_SIZE * 3);
    return ;
}

void build_ac(trie_node *root, int n) {
    Queue *q = init_queue(n + 10);
    root->fail = NULL;
    push(q, root);
    while (!empty(q)) {
        trie_node *now_node = front(q);
        pop(q);
        for (int i = 0; i < BASE; i++) {
            if (now_node->next[i] == NULL) continue;
            trie_node *p = now_node->fail;
            while (p && p->next[i] == NULL) p = p->fail;
            if (p == NULL) now_node->next[i]->fail = root;
            else now_node->next[i]->fail = p->next[i];
            push(q, now_node->next[i]);
        }
    }
    return ;
}

void match(trie_node *root, const char *str) {
    trie_node *p = root;
    for (int i = 0; str[i]; i++) {
        int ind = str[i] - BASE_LETTER;
        while (p && p->next[ind] == NULL) p = p->fail;
        if (p == NULL) p = root;
        else p = p->next[ind];
        trie_node *q = p;
        while (q) {
            if (q->flag) {
                printf("匹配到 : %s\n", q->word);
            }
            q = q->fail;
        }
    }
    return ;
}

trie_node *build_trie(int word_cnt) {
    trie_node *root = get_new_node();
    for (int i = 0; i < word_cnt; i++) {
        insert(root, word[i].huffman_code, word[i].str);
    }
    build_ac(root, word_cnt);
    return root;
}

int main() {
    while (1) {
        scanf("%s", word[word_cnt].str);
        if (strcmp((char *)word[word_cnt].str, "END") == 0) {
            break;
        }
        word_cnt++;
    }
    printf("输入了%d个词\n", word_cnt);
    for (int i = 0; i < word_cnt; i++) {
        for (int j = 0; word[i].str[j]; j++) {
            data[word[i].str[j]] += 1;
        }
    }
    printf("字节频次表:\n");
    for (int i = 0; i < 256; i++) {
        if (data[i] == 0) continue;
        printf("%d : %d\n", i, data[i]);
        character_cnt += 1;
    }
    printf("总字节个数:%d\n", character_cnt);
    for (int i = 0, j = 0; i < 256; i++) {
        if (data[i] == 0) continue;
        arr[j].ch = i;
        arr[j].p = data[i];
        j++;
    }
    HuffmanTree *tree = build(arr, character_cnt);
    printf("字节编码表:\n");
    for (int i = 0; i < tree->n; i++) {
        printf("%d : %s\n", tree->codes[i].ch, tree->codes[i].str);
    }
    printf("词语编码表:\n");
    for (int i = 0; i < word_cnt; i++) {
        int len = strlen((char *)word[i].str);
        for (int j = 0; j < len; j++) {
            int x = 0;
            while (word[i].str[j] != tree->codes[x].ch) x++;
            strcat(word[i].huffman_code, tree->codes[x].str);
        }
        printf("%s : %s\n", word[i].str, word[i].huffman_code);
    }
    trie_node *root = build_trie(word_cnt);
    printf("输入母串开始匹配\n");
    unsigned char *str = (unsigned char *)malloc(sizeof(unsigned char) * 100000);
    char *str_huffman_code = (char *)malloc(sizeof(char) * 1000000);
    scanf("%s", str);
    for (int i = 0; str[i]; i++) {
        int flag = 0, j = 0;
        for (; j < tree->n; j++) {
            if (str[i] != tree->codes[j].ch) continue;
            flag = 1;
            break;
        }
        if (flag) {
            strcat(str_huffman_code, tree->codes[j].str);
        }
    }
    printf("%s\n", str_huffman_code);
    match(root, str_huffman_code);
    return 0;
}
