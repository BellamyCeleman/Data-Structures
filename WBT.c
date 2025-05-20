#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define ALPHA 0.5 // balance threshold

typedef struct NodeWBT {
    int val;
    int size;               // size of subtree
    struct NodeWBT *left;
    struct NodeWBT *right;
} NodeWBT;

NodeWBT* createNodeWBT(int val) {
    NodeWBT* newNode = (NodeWBT*)malloc(sizeof(NodeWBT));
    newNode->val = val;
    newNode->size = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int size(NodeWBT *node) {
    return node ? node->size : 0;
}

void rewriteSize(NodeWBT *node) {
    if (node) {
        node->size = size(node->left) + size(node->right) + 1; // update size of subtree
    }
}

void leftRotationWBT(NodeWBT** root) {
    NodeWBT* newRoot = (*root)->right;
    (*root)->right = newRoot->left;
    newRoot->left = *root;
    rewriteSize(*root);                 // change sizes of subtree
    rewriteSize(newRoot);               
    *root = newRoot;
}

void rightRotationWBT(NodeWBT** root) {
    NodeWBT* newRoot = (*root)->left;
    (*root)->left = newRoot->right;
    newRoot->right = *root;
    rewriteSize(*root);
    rewriteSize(newRoot);
    *root = newRoot;
}

void balance(NodeWBT** root) {
    if (!(*root)) return;
    int leftSize = size((*root)->left);
    int rightSize = size((*root)->right);
    int totalSize = size(*root);

    if (leftSize > ALPHA * totalSize) {
        if (size((*root)->left->left) < size((*root)->left->right)) {
            leftRotationWBT(&((*root)->left)); // convert zig-zag to straight
        }
        rightRotationWBT(root);
    } else if (rightSize > ALPHA * totalSize) {
        if (size((*root)->right->right) < size((*root)->right->left)) {
            rightRotationWBT(&((*root)->right)); // convert zig-zag to straight
        }
        leftRotationWBT(root);
    }
}

NodeWBT* insertRec(NodeWBT* root, NodeWBT* newNode) {
    if (!root) return newNode;

    if (newNode->val < root->val)
        root->left = insertRec(root->left, newNode);
    else
        root->right = insertRec(root->right, newNode);

    rewriteSize(root);          // this code will rewrite size and balance our tree after all the insertRec functions
    balance(&root); // balance after insertWBT
    return root;
}

void insertWBT(NodeWBT **root, NodeWBT *newNode) {
    *root = insertRec(*root, newNode);
}

NodeWBT* deleteRec(NodeWBT* root, int val) {
    if (!root) return NULL;

    if (val < root->val) {
        root->left = deleteRec(root->left, val);
    } else if (val > root->val) {
        root->right = deleteRec(root->right, val);
    } else {                // if val == root.val
        if (!root->left || !root->right) {          // if we have one child
            NodeWBT* child = root->left ? root->left : root->right;
            free(root);
            return child;               // replace the root with the child
        } else {
            NodeWBT* successor = root->right;
            while (successor->left) successor = successor->left;    // smallest in right subtree
            root->val = successor->val;                             // replace root's val with the leftmost val in the right subtree
            root->right = deleteRec(root->right, successor->val);   // then we delete the leftmost node
        }
    }

    rewriteSize(root);
    balance(&root); // balance after deleteWBT
    return root;
}

void deleteWBT(NodeWBT **root, int val) {
    *root = deleteRec(*root, val);
}

NodeWBT *searchWBT(NodeWBT *root, int val) {
    while (root) {
        if (root->val == val) return root;
        else if (root->val < val) root = root->right;
        else root = root->left;
    }
    return NULL;
}

void printTreeWBT(NodeWBT* root, int size) {
    if (!root) return;
    printTreeWBT(root->right, size + 1);
    for (int i = 0; i < size; i++) printf("   ");
    printf("%d (%d)\n", root->val, root->size);
    printTreeWBT(root->left, size + 1);
}

void freeTreeWBT(NodeWBT** root) {
    if (!(*root)) return;
    NodeWBT** stack = (NodeWBT**)malloc(sizeof(NodeWBT*) * size(*root));
    int top = -1;                   // index of the stack
    NodeWBT* curr = *root;

    while (curr || top != -1) {     // while we are not on the end of the tree
        while (curr) {
            stack[++top] = curr;    // we put curr node in the stack
            curr = curr->left;
        }
        curr = stack[top--];
        NodeWBT* temp = curr;
        curr = curr->right;
        free(temp);
    }

    free(stack);
    *root = NULL;
}

