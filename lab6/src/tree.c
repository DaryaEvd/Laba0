#include "tree.h"

struct Node{
    int key;
    int height;
    struct Node *left;
    struct Node *right;    
} Node;

struct AllocTree{
    struct Node *data;
    int index;
} AllocTree;

struct AllocTree *NodeAlloc(const int size){
    struct AllocTree *result = malloc(sizeof(struct AllocTree));
    if(!result) return NULL;

    result->data = malloc((size + 1) * sizeof(struct Node));
    if(!result->data){
        free(result);
        return NULL;
    }
  
    result->index = 0;
    result->data[0].left = NULL;
    result->data[0].right = NULL;
    result->data[0].height = 1;
    return result;
}

void Dealloc (struct AllocTree *treeNode){
    free(treeNode->data);
    free(treeNode);
}

static int MaxElement(int const a, int const b){
    return (a > b)? a : b;
}

int IsHeight(struct Node const *n){
    return n ? n->height : EXIT_SUCCESS_;
}

static int FixHeightValue(struct Node const *n){
    if (n != NULL)
        return 1 + MaxElement(IsHeight(n->left), IsHeight(n->right));
    return EXIT_SUCCESS_;
} 

static int GetBalanceFactor(struct Node const *n){
    if (n != NULL) 
        return IsHeight(n->left) - IsHeight(n->right);
    return EXIT_SUCCESS_;    
} 

static struct Node *RightRotate(struct Node *const root){
    struct Node *newRoot = root->left;
    struct Node *leaf = newRoot->right;
 
    newRoot->right = root;
    root->left = leaf;
 
    root->height = FixHeightValue(root);
    newRoot->height = FixHeightValue(newRoot);

    return newRoot;
}
 
static struct Node *LeftRotate(struct Node *const root){
    struct Node *newRoot = root->right;
    struct Node *leaf = newRoot->left;
 
    newRoot->left = root;
    root->right = leaf;
 
    root->height = FixHeightValue(root);
    newRoot->height = FixHeightValue(newRoot);
 
    return newRoot;
}

static struct Node *BalanceTree(struct Node *const root){
    root->height = FixHeightValue(root);

    if(GetBalanceFactor(root) == -2){
        if (root->height && GetBalanceFactor(root->right) > 0) 
            root->right = RightRotate(root->right);         
        return LeftRotate(root);
    }

    if(GetBalanceFactor(root) == 2){
        if(root->left && GetBalanceFactor(root->left) < 0)
            root->left = LeftRotate(root->left);
        return RightRotate(root);
    }
    return root;
} 

static struct Node *CreateNode(int const key, struct AllocTree *allocMem){
    allocMem->data[allocMem->index].key = key;
    allocMem->data[allocMem->index].height = 1;
    allocMem->data[allocMem->index].left = NULL;
    allocMem->data[allocMem->index].right = NULL;
    return &(allocMem->data[allocMem->index++]);
}
 
struct Node *Insert(struct Node *const node, int key, struct AllocTree *const allocTree){
    if (node == NULL)
        return(CreateNode(key, allocTree)); 
  
    if (key > node->key)
        node->right = Insert(node->right, key, allocTree);        
    else 
        node->left  = Insert(node->left, key, allocTree);
 
    node->height = FixHeightValue(node);
 
    return BalanceTree(node);
}
