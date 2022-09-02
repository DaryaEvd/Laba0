#ifndef KRUSKAL_H_
#define KRUSKAL_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h> 

typedef enum {
    EXIT_SUCCESS_,
    EXIT_FAILURE_,
    BAD_NUMBER_OF_VERTEX,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_NUMBER_OF_LINES,
    BAD_LENGTH,
    NO_SPANNING_TREE,
} codeStatus;

unsigned int ReadAndCheckVertex();
unsigned int ReadAndCheckEdge(const int vertexNum);

typedef struct Graph Graph;
Graph *InitGraph(const int edgeNum);
void DeleteGraph(Graph *graph);
int AddEdgesToGraph(Graph *graph, const unsigned int vertexNum, 
                                        const unsigned int edgeNum);

typedef struct Edge Edge;
size_t EdgeSize();

size_t KruskalAlgorythm(Graph *graph, const unsigned int vertexNum, 
                        const unsigned int edgeNum, Edge *minFrame);

void PrintError(codeStatus error);
void PrintResOfAdding(codeStatus resultOfFilling);
void PrintMinimalSpanningTree(Edge *minFrame, const size_t 
                                                countOFAddedEdges);

#endif //KRUSKAL_H_
