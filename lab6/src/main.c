#include "tree.h"

int main(void){
    int numberOfVertexes = 0;
    if (!scanf("%d", &numberOfVertexes)) return EXIT_SUCCESS_;
    
    if (numberOfVertexes <= 0){
        printf("0");
        return EXIT_SUCCESS_;
    } 
  
    struct AllocTree *treeAlloc = NodeAlloc(numberOfVertexes + 1);
    if(!treeAlloc) return EXIT_SUCCESS_;

    struct Node *root = NULL;
    int vertex;

    for (int i = 0; i < numberOfVertexes; i++){
        if (!scanf("%d", &vertex)){
            Dealloc(treeAlloc);
            return EXIT_SUCCESS;
        }
        root = Insert(root, vertex, treeAlloc);
    }

    int height = IsHeight(root);
    printf("%d", height);
    Dealloc(treeAlloc);
    return EXIT_SUCCESS_;
}
