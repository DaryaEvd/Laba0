#ifndef TOP_SORT_H_
#define TOP_SORT_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    EXIT_SUCCESS_,
    EXIT_FAILURE_,
    BAD_NUMBER_OF_VERTEX,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_NUMBER_OF_LINES,
    IMPOSSIBLE_TO_SORT,
} typeOfError;

int ReadAndCheckVertex();
int ReadAndCheckEdge(const int vertexNum);
void InitColours(char *colours, const unsigned int vertexNum);
int FillEdgesToMatrix(const int edgeNum, const int vertexNum,
                      unsigned char *matrix);
void FillRes(typeOfError resultOfFilling);
void FinishProgram(const unsigned int vertexNum, unsigned char *matrix,
                     unsigned int *stackOfSortedVertexes, char *colours);

#endif //TOP_SORT_H_//
