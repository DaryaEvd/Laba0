#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum{
    EXIT_SUCCESS_,
    EXIT_FAILURE_
} ExitStatus;

struct Node;
struct AllocTree;
struct AllocTree *NodeAlloc(const int size);
void Dealloc (struct AllocTree *treeNode);

struct Node *Insert(struct Node *const node, int key, struct AllocTree *const allocTree);
int IsHeight(const struct Node *n);

#endif // TREE_H_
