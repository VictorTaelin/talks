#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* buildTree(int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    Node* root = createNode(mid);
    root->left = buildTree(start, mid - 1);
    root->right = buildTree(mid + 1, end);
    return root;
}

long long sumTree(Node* root) {
    if (root == NULL) return 0;
    return root->data + sumTree(root->left) + sumTree(root->right);
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root = buildTree(0, (1 << 24) - 1);
    long long sum = sumTree(root);
    printf("Sum of all elements in the tree: %lld\n", sum);
    freeTree(root);
    return 0;
}
