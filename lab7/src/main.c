#include "topsort.h"

int main(void) {
    const int vertexNum = ReadAndCheckVertex();
    const int edgeNum = ReadAndCheckEdge(vertexNum);

    char colours[vertexNum + 1];
    InitColours(colours, vertexNum);   
    unsigned int stackOfSortedVertexes[vertexNum + 1];

    const int matrixLenght = (vertexNum + 1) * (vertexNum + 1);
    unsigned char *matrix = calloc(matrixLenght, sizeof(unsigned char));
    if (!matrix) return EXIT_SUCCESS_;

    const int resultOfFilling = FillEdgesToMatrix(edgeNum, vertexNum, matrix);
    if(resultOfFilling != EXIT_SUCCESS_){
        FillRes(resultOfFilling);
        free(matrix);
        return EXIT_SUCCESS_;
    }

    FinishProgram(vertexNum, matrix, stackOfSortedVertexes, colours);
    
    free(matrix);
    return EXIT_SUCCESS_;
}
