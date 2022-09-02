#include "Prim.h"

int main(void) {
    const unsigned int vertexNum = IsCorrectInputVertex();
    const unsigned int edgeNum = IsCorrectInputEdge(vertexNum);

    if (!haveMST(vertexNum, edgeNum)) return EXIT_SUCCESS_;

    unsigned int matrixLength = (vertexNum) * (vertexNum);
    unsigned int *matrix = malloc(sizeof(unsigned int) * matrixLength);
    if (!matrix) return EXIT_SUCCESS_;

    InitCellsInMatrix(matrix, vertexNum);
    const int resOfFilling = FillValuesInMatrix(matrix, edgeNum, vertexNum);
    if (!resOfFilling) {
        free(matrix);
        return EXIT_SUCCESS_;
    }

    size_t sizeEdgeConstructor = CalcSizeEdgeConstructor();
    struct EdgeConstructor *edge = malloc(sizeEdgeConstructor * (vertexNum));
    if (!edge) {
        free(matrix);
        return EXIT_SUCCESS_;
    }
    
    size_t sizeEdgeAns = CalcEdgeAns();
    struct EdgeAns *ans = malloc(sizeEdgeAns * (vertexNum - 1) * 2);
    if (!ans) {
        free(matrix);
        free(edge);
        return EXIT_SUCCESS_;
    }

    if (!PrimMST(edge, matrix, vertexNum, ans)) {
        PrintError(NO_SPANNING_TREE);
        FreeStructures(matrix, edge, ans);
        return EXIT_SUCCESS_;
    }

    PrintMST(ans, vertexNum);
    FreeStructures(matrix, edge, ans);
    return EXIT_SUCCESS_;   
}
