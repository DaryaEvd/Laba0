#ifndef PRIM_H_
#define PRIM_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    EXIT_SUCCESS_,
    EXIT_FAILURE_,
    BAD_NUMBER_OF_EDGES,
    BAD_NUMBER_OF_VERTEX,
    BAD_VERTEX,
    NO_SPANNING_TREE,
    BAD_NUMBER_OF_LINES,
    BAD_LENGTH
} typeOfError;

unsigned int IsCorrectInputVertex();
unsigned int IsCorrectInputEdge(const int vertexNum);
int haveMST(const int vertexNum, const int edgeNum);

void InitCellsInMatrix(unsigned int *matrix, const unsigned int vertexNum);
unsigned int FillValuesInMatrix(unsigned int *matrix, const unsigned int edgeNum,
                                const unsigned int vertexNum);

typedef struct EdgeConstructor EdgeConstructor;
size_t CalcSizeEdgeConstructor();

typedef struct EdgeAns EdgeAns;
size_t CalcEdgeAns();

int PrimMST(EdgeConstructor *edgePrim, unsigned int *matrix,
             const unsigned int vertexNum, EdgeAns *ans);

void PrintMST(EdgeAns *ans, const unsigned int vertexNum);
void PrintError(typeOfError error);
void FreeStructures(unsigned int *matrix, EdgeConstructor *edge, EdgeAns *ans);

#endif //PRIM_H_
