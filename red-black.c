#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { RED, BLACK } Color;

typedef struct NodeRBT {
    int val;
    Color color;
    struct NodeRBT *left, *right, *parent;
} NodeRBT;

NodeRBT* new_NodeRBT(int val) {
    NodeRBT *node = (NodeRBT*)malloc(sizeof(NodeRBT));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = RED;
    return node;
}

void leftRotationRBT(NodeRBT **root, NodeRBT *node) {
    NodeRBT *y = node->right;
    node->right = y->left;       // moving Y left branch to node's right branch
    if (y->left != NULL) {       // fixing Y's parent
        y->left->parent = node;
    }

    if (node->parent == NULL) {     // checking if node was a root
        (*root) = y;
    } else if (node->parent->left == node) {       // fixing node's parent left or right child
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }
    y->left = node;     // final changes
    y->parent = node->parent;
    node->parent = y;
}

void rightRotationRBT(NodeRBT **root, NodeRBT *new) {
    NodeRBT *y = new->left;
    new->left = y->right;       // moving Y right branch to node's left branch
    if (y->right != NULL) {     // fixing Y's parent
        y->right->parent = new;
    }

    if (new->parent == NULL) {       // checking if node was a root
        (*root) = y;
    } else if (new->parent->right == new) {     // fixing node's parent left or right child
        new->parent->right = y;
    } else {
        new->parent->left = y;
    }
    y->right = new;         // final changes
    y->parent = new->parent;
    new->parent = y;
}

void insertFixupRBT(NodeRBT **root, NodeRBT *new) {
    while (new != (*root) && new->parent->color == RED) {
        if (new->parent == new->parent->parent->left) {     // to understand in which branch our parent is
            NodeRBT *uncle = new->parent->parent->right;

            Color uncle_color = (uncle == NULL || uncle->color == BLACK) ? BLACK : RED;
            
            if (new->parent->color == RED && uncle_color == RED) {          // Case 1: if parent and uncle of our node are red
                new->parent->color = BLACK;                                 // and our node is also red because it is a new one
                uncle->color = BLACK;                                       // we recolor parent and uncle in black and grandparent 
                new->parent->parent->color = RED;                           // in red
                new = new->parent->parent;
            } else if (new->parent->color == RED) {                         // Case 2: if parent is red and uncle is black, and new 
                if (new == new->parent->right) {                            // is a right child, parent is a left child, then we rotate 
                    new = new->parent;                                      // to left, and then to right
                    leftRotationRBT(root, new);
                }
                new->parent->color = BLACK;                                 // Case 3: if parent is red and uncle is black, and new
                new->parent->parent->color = RED;                           // is a left child, parent is a left child, then we rotate
                rightRotationRBT(root, new->parent->parent);                   // to right
            } 
        } else {
            NodeRBT *uncle = new->parent->parent->left;

            Color uncle_color = (uncle == NULL || uncle->color == BLACK) ? BLACK : RED;

            if (new->parent->color == RED && uncle_color == RED) {
                new->parent->color = BLACK;
                uncle->color = BLACK;
                new->parent->parent->color = RED;
                new = new->parent->parent;
            } else if (new->parent->color == RED) {
                if (new == new->parent->left) {
                    new = new->parent;
                    rightRotationRBT(root, new);
                }
                new->parent->color = BLACK;
                new->parent->parent->color = RED;
                leftRotationRBT(root, new->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;
}

void insertRBT(NodeRBT **root, NodeRBT *new) {
    if ((*root) == NULL) {      // assign node if root is NULL
        new->color = BLACK;
        (*root) = new;
        return;
    }

    NodeRBT *temp = (*root);
    NodeRBT *parent = NULL;        // to find a parent for a new node

    while (temp != NULL) {      // searchRBTing for place to put the new node
        parent = temp;
        if (new->val > temp->val) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }


    if (new->val >= parent->val) {      //adding do a parent in the right branch 
        parent->right = new;
    } else {
        parent->left = new;
    }
    new->parent = parent;

    insertFixupRBT(root, new);     // fixing violations 
}

void deleteFixupRBT(NodeRBT **root, NodeRBT *node) {
    while (node != *root && node != NULL && node->color == BLACK) {     // while  node != root (that means we came back to the top of the tree)
        NodeRBT *sibling;                                               // we correct only black nodes

        if (node == node->parent->left) {
            sibling = node->parent->right;                          // we find the uncle

            if (sibling != NULL && sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                leftRotationRBT(root, node->parent);
                sibling = node->parent->right;
            }

            if (sibling != NULL &&                                                // First case: if the sibling is red
                (sibling->left == NULL || sibling->left->color == BLACK) &&         // Check if the sibling' he left child is black
                (sibling->right == NULL || sibling->right->color == BLACK)) {       
                sibling->color = RED;                                               // color the sibling red, move up to the parent node
                node = node->parent;                                                 
            }
            else {                                                                  
                if (sibling != NULL && (sibling->right == NULL || sibling->right->color == BLACK)) { // the right child of the sibling is black
                    if (sibling->left != NULL) sibling->left->color = BLACK;        // color sibling's left child black, sibling red
                    sibling->color = RED;                                           
                    rightRotationRBT(root, sibling);                                 
                    sibling = node->parent->right;                                   
                }
                if (sibling != NULL) {                                               
                    sibling->color = node->parent->color;                            // Set the sibling's color to the parent's color, parent black
                    node->parent->color = BLACK;                                    
                    if (sibling->right != NULL) sibling->right->color = BLACK;       // sibling's right child color black
                    leftRotationRBT(root, node->parent);                             
                }
                node = *root;                                                        // If the balance is not restored, continue fixing the tree from the root
            }
        } else {                // All the same if node is a right branch of parent
            sibling = node->parent->left;

            if (sibling != NULL && sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rightRotationRBT(root, node->parent);
                sibling = node->parent->left;
            }

            if (sibling != NULL &&
                (sibling->right == NULL || sibling->right->color == BLACK) && 
                (sibling->left == NULL || sibling->left->color == BLACK)) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling != NULL && (sibling->left == NULL || sibling->left->color == BLACK)) {
                    if (sibling->right != NULL) sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotationRBT(root, sibling);
                    sibling = node->parent->left;
                }
                if (sibling != NULL) {
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != NULL) sibling->left->color = BLACK;
                    rightRotationRBT(root, node->parent);
                }
                node = *root;
            }
        }
    }
    if (node != NULL) node->color = BLACK;          // color the root node Black
}

void deleteRBT(NodeRBT **root, NodeRBT *del) {
    if (del == NULL || *root == NULL) return;

    Color deleteRBTdColor = RED;       // to check if we need a deleteFixupRBT
    NodeRBT *temp = (*root);
    NodeRBT *parent = NULL;
    NodeRBT *fixupNode;

    while (temp != NULL && temp->val != del->val) {         // searchRBTing for a node
        if (temp->val > del->val) {         
            temp = temp->left;
        } else if (temp->val < del->val) {
            temp = temp->right;
        }
    }

    if (temp == NULL) return;       // node isn't found

    parent = temp->parent;

    
    if (temp->left == NULL && temp->right == NULL) {        // Node has no children
        if (temp == (*root)) {
            (*root) = NULL;
        } else if (parent->left == temp) {
            temp = NULL;
            parent->left = NULL;
        } else {
            temp = NULL;
            parent->right = NULL;
        }
        free(temp);
    } else if (temp->left == NULL || temp->right == NULL) {     // Has exactly one child
        NodeRBT *child = (temp->left) ? temp->left : temp->right;

        if (parent == NULL) {
            (*root) = child;
        } else if (parent->left == temp) {
            parent->left = child;
        } else if(parent->right == temp) {
            parent->right = child;
        }

        if (child != NULL) {
            child->parent = parent;
        }
        free(temp);

    } else {                                               // has both children
        NodeRBT *succesor = temp->right;
        NodeRBT *succesorParent = temp;

        while (succesor->left != NULL) {        // moving to the leftmost element in the right branch of the node we want ot deleteRBT
            succesorParent = succesor;
            succesor = succesor->left;
        }
        temp->val = succesor->val;

        if (succesorParent->left == succesor) {
            succesorParent->left = succesor->right;
        } else {
            succesorParent->right = succesor->right;
        }

        deleteRBTdColor = succesor->color;         // to check if we need a fixUp

        free(succesor);
    }
    
    if (deleteRBTdColor == BLACK) {
        deleteFixupRBT(root, del);
    }
}

void inorder(NodeRBT *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d (%s) ", root->val, root->color == RED ? "R" : "B");
        inorder(root->right);
    }
}

NodeRBT* searchRBT(NodeRBT* root, int val) {
    while (root != NULL) {
        if (val == root->val) {
            return root;
        } else if (val < root->val) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL;
}

void printTreeRBT(NodeRBT *root, int depth) {
    if (root == NULL) {
        return;
    }
    printTreeRBT(root->right, depth + 1);
    for (int i = 0; i < depth; i++) {
        printf("   ");
    }
    printf("(%d %s)\n", root->val, root->color == RED ? "R" : "B");
    printTreeRBT(root->left, depth + 1);
}

void clearRBTree(NodeRBT **root) {
    if (*root == NULL) return;

    clearRBTree(&(*root)->left);  
    clearRBTree(&(*root)->right);

    free(*root);                  
    *root = NULL;                
}
