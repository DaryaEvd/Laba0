#include "dijkstra.h"

int main(void){
    int vertexNum, startNode, endNode, edgeNum;
    if(scanf("%d %d %d %d", &vertexNum, &startNode, &endNode, &edgeNum) != 4)
        PrintError(BAD_NUMBER_OF_LINES);
    
    typeOfError resultOfFilling = IsOKInput(vertexNum, startNode, endNode, edgeNum);
    if(resultOfFilling != EXIT_SUCCESS_){
        PrintRes(resultOfFilling);
        return EXIT_SUCCESS_;
    }
    
    const unsigned int lengthMatrix = vertexNum * vertexNum;    
    int *matrix = malloc(sizeof(unsigned int) * lengthMatrix);
    if(!matrix) return EXIT_SUCCESS_;

    const size_t resOfInit = InitMatrixValues(matrix, vertexNum, edgeNum);    
    if(!resOfInit){
        free(matrix);
        return EXIT_SUCCESS_;
    } 

    const size_t edgeSizeConstructor = CalcSizeEdgeConstructor();
    EdgeConstructor *edge = malloc(edgeSizeConstructor * (vertexNum + 1));
    if(!edge){
        free(matrix);
        return EXIT_SUCCESS_;
    }

    DijkstraAlgorythm(edge, matrix, vertexNum, startNode - 1);
    PrintFirstLine(edge, vertexNum);
    printf("\n");
    
    if(!PrintSecondLine(edge, startNode - 1, endNode - 1)){
        FreeStructures(matrix, edge);
        return EXIT_SUCCESS_;
    };

    FreeStructures(matrix, edge);
    return EXIT_SUCCESS_;
}
