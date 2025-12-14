#include <stdlib.h>
#include <string.h>
#include "../includes/structs.h"


//Yeni Düğüm Oluşturma
Node* createNode(const char* name, int id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) { /* Hata yönetimi */ exit(EXIT_FAILURE); }
    
    strncpy(newNode->name, name, MAX_NAME_LEN - 1);
    newNode->name[MAX_NAME_LEN - 1] = '\0';
    newNode->stop_id = id;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//Ekleme İşlevi
Node* insert_bst(Node* root, const char* name, int id) {
    if (root == NULL) {
        return createNode(name, id);
    }
    // String karşılaştırması
    int comparison = strcmp(name, root->name);

    if (comparison < 0) {
        root->left = insert_bst(root->left, name, id);
    } else if (comparison > 0) {
        root->right = insert_bst(root->right, name, id);
    }
    // Eşitlik durumunda (comparison == 0) bir şey yapmaya gerek yok.

    return root;
}

//Arama İşlevi
int search_bst(Node* root, const char* name) {
    if (root == NULL) {
        return -1; // Bulunamadı
    }

    int comparison = strcmp(name, root->name);

    if (comparison == 0) {
        return root->stop_id; // Durak bulundu
    } else if (comparison < 0) {
        return search_bst(root->left, name); // Sola git
    } else {
        return search_bst(root->right, name); // Sağa git
    }
}