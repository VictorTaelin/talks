#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 16
#define MAX_VALUE (1 << 17)

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct ThreadArgs {
    Node* root;
    long long sum;
} ThreadArgs;

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

void* sumTree(void* args) {
    ThreadArgs* threadArgs = (ThreadArgs*)args;
    Node* root = threadArgs->root;

    if (root == NULL) {
        threadArgs->sum = 0;
        return NULL;
    }

    ThreadArgs leftArgs, rightArgs;
    pthread_t leftThread, rightThread;

    leftArgs.root = root->left;
    rightArgs.root = root->right;

    pthread_create(&leftThread, NULL, sumTree, &leftArgs);
    pthread_create(&rightThread, NULL, sumTree, &rightArgs);

    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);

    threadArgs->sum = root->data + leftArgs.sum + rightArgs.sum;
    return NULL;
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root = buildTree(0, MAX_VALUE - 1);

    ThreadArgs threadArgs[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    int range = MAX_VALUE / NUM_THREADS;
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadArgs[i].root = buildTree(i * range, (i + 1) * range - 1);
        pthread_create(&threads[i], NULL, sumTree, &threadArgs[i]);
    }

    long long totalSum = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        totalSum += threadArgs[i].sum;
        freeTree(threadArgs[i].root);
    }

    printf("Sum of all elements in the tree: %lld\n", totalSum);
    return 0;
}

// f(10) = 
// f(11) =  0.209s
// f(12) =  0.672s
// f(13) =  1.724s
// f(14) =  4.420s
// f(15) = 17.067s
// f(16) = 73.380s
