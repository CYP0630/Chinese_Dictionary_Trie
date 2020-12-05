typedef unsigned short US;
typedef unsigned char UC;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"
#include "huffman_tree.h"
#define MAX_N 256
int mystrlen(UC* str) {
    int cnt = 0;
    while (str[cnt] != '\0') cnt++;
    return cnt / 2;
}

US **convertToShortArr(UC (*str)[64], int str_cnt) {
    US **short_arr = (US **)calloc(str_cnt, sizeof(US *));
    for (int i = 0; i < str_cnt; ++i) {
        int len = mystrlen(str[i]);
        short_arr[i] = (US *)calloc(len, sizeof(US));
        for (int j = 0; str[i][j]; j += 2) {
            short_arr[i][j / 2] |= str[i][j];
            short_arr[i][j / 2] <<= 8;
            short_arr[i][j / 2] |= str[i][j + 1];
        }
    }
    return short_arr;
}
char huffman_code[65536][100] = {0};
int main() {
    UC str[128][64] = {0};
    printf("输入模式串(END结束):\n");
    int str_cnt = 0;
    while (true) {
        scanf("%s", str[str_cnt]);
        if (strcmp((const char *)str[str_cnt], "END") == 0) {
            printf("开始分析....\n\n");
            break;
        }
        str_cnt++;
    }
    US** short_arr = convertToShortArr(str, str_cnt);

    int* visit = (int *)calloc(100000, sizeof(int));
    int* maps = (int *)calloc(100000, sizeof(int));
    int maps_cnt = 0;
    for (int i = 0 ; i < str_cnt; ++i) {
        int len = mystrlen(str[i]);
        for (int j = 0; j < len; j++) {
            if (!visit[short_arr[i][j]]) {
               maps[maps_cnt++] = short_arr[i][j]; 
            }
            visit[short_arr[i][j]]++;
        }
    }
    printf("词频统计表：\n");
    for (int i = 0; i < maps_cnt; ++i) {
        unsigned short ind = maps[i];
        char a = ind >> 8;
        char b = ind;
        printf("[%c%c] : %d\n", a, b, visit[ind]);
    }
    printf("\n");
    HFNode **arr = (HFNode **)calloc(maps_cnt, sizeof(HFNode));
    for (int i = 0; i < maps_cnt; ++i) {
        HFNode *node = getNode();
        node->ch = maps[i];
        node->freq = visit[maps[i]];
        arr[i] = node;
    }
    char buff[100];
    build(maps_cnt, arr);
    extract(arr[0], buff, huffman_code, 0);
    printf("哈夫曼编码表：\n");
    for (int i = 0; i < maps_cnt; i++) {
        US ind = maps[i];
        char a = ind >> 8;
        char b = ind;
        printf("%c%c : %s\n", a, b, huffman_code[ind]);
    }
    Trie root = NULL;
    for (int i = 0; i < str_cnt; ++i) {
        int len = mystrlen(str[i]);
        char new_str[1024] = {0};
        for (int j = 0; j < len; ++j) {
            sprintf(new_str, "%s%s", new_str, huffman_code[short_arr[i][j]]);
        }
        root = insert(root, new_str, str[i]);
    }
    
    UC quesion[128] = {0};
    printf("开始询问(END结束)：\n");
    while (true) {
        scanf("%s", quesion);
        if (strcmp((const char *)quesion, "END") == 0) {
            printf("程序结束....\n\n");
            break;
        }
        int len = mystrlen(quesion);
        int flag = 0;
        char temp_ch[128] = {0};
        for (int i = 0; i < len * 2; i += 2) {
            US temp = 0;
            temp |= quesion[i];
            temp <<= 8;
            temp |= quesion[i + 1];
            if (huffman_code[temp] == '\0') continue;
            sprintf(temp_ch, "%s%s", temp_ch, huffman_code[temp]);
        }
        /*if (flag) {
            printf("--404 NO RESULT--\n");
            continue;
        }*/
        match(root, temp_ch);
        /*if (search(root, temp_ch)) {
            printf("%s TAKE FOUND!\n",quesion);
        }else {
            printf("--404 NO RESULT--\n");
        }*/
    }
    system("pause");
    return 0;
}