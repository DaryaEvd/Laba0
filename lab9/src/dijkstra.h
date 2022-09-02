#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {  
    EXIT_SUCCESS_,
    EXIT_FAILURE_,
    BAD_LENGTH,
    BAD_NUMBER_OF_EDGES,
    BAD_NUMBER_OF_VERTEX,
    BAD_VERTEX,
    BAD_NUMBER_OF_LINES    
} typeOfError;

void PrintError(typeOfError error);
void PrintRes(typeOfError resultOfFilling);
typeOfError IsOKInput(int vertexNum, int startNode, int endNode, int edgeNum);
void FillRes(typeOfError resultOfFilling);

size_t InitMatrixValues(int *matrix, const unsigned int vertexNum, const unsigned int edgeNum);

typedef struct EdgeConstructor EdgeConstructor;
size_t CalcSizeEdgeConstructor();

void DijkstraAlgorythm(EdgeConstructor *edge, int *matrix, const unsigned int 
                                            vertexNum, const unsigned int startNode);
void PrintFirstLine(EdgeConstructor *edge, const unsigned int vertexNum);
int PrintSecondLine(EdgeConstructor *edge, const unsigned int startNode, unsigned int endNode);
void FreeStructures(int *matrix, EdgeConstructor *edge);

#endif //DIJKSTRA_H_
