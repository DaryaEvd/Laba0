#include "topsort.h"

#define MAX_VERTEX_NUMBER 2000

typedef enum { 
    WHITE,
    GRAY, 
    BLACK
} colourStatus;

static void PrintMistakes(typeOfError error){
    switch (error) {
        case BAD_NUMBER_OF_VERTEX:
            printf("bad number of vertices");
            break;
        case BAD_NUMBER_OF_EDGES:
            printf("bad number of edges");
            break;
        case BAD_VERTEX:
            printf("bad vertex");
            break;
        case BAD_NUMBER_OF_LINES:
            printf("bad number of lines");
            break;
        case IMPOSSIBLE_TO_SORT:
            printf("impossible to sort");
            break;
        default: ;
    }
}

static int IsCorrectVertexesNumber(const int vertexNum) {
    return (!(vertexNum < 0 || vertexNum > MAX_VERTEX_NUMBER));
}

static int IsCorrectEdgesNumber(const int edgeNum, const int vertexNum) {
    return (!(edgeNum < 0 || edgeNum > (vertexNum * (vertexNum - 1) / 2)));
}

static int IsBadVertex(const int startNode, const int endNode,
                const int vertexNum) {
    return (!(startNode < 0 || startNode > vertexNum || endNode < 0 ||
              endNode > vertexNum));
}

int ReadAndCheckVertex() {
    int countOfVertex = 0;
    if (scanf("%d", &countOfVertex) != 1) {
        PrintMistakes(BAD_NUMBER_OF_LINES);
        return EXIT_SUCCESS_;
    }
    if (!IsCorrectVertexesNumber(countOfVertex)) {
        PrintMistakes(BAD_NUMBER_OF_VERTEX);
        return EXIT_SUCCESS_;
    }
    return countOfVertex;
}

int ReadAndCheckEdge(const int vertexNum) {
    int countOfEdge = -1;
    if (scanf("%d", &countOfEdge) == -1) {
        PrintMistakes(BAD_NUMBER_OF_LINES); 
        return EXIT_SUCCESS_;
    }
    if (!IsCorrectEdgesNumber(countOfEdge, vertexNum)) {
        PrintMistakes(BAD_NUMBER_OF_EDGES);
        return EXIT_SUCCESS_;
    }
    return countOfEdge;
}

static int DFS(unsigned char *matrix, char *colours,
         unsigned int *stackOfSortedVertexes, const int visitedVertex,
         int *visitedVertexInd, const int vertNum) {
    if (colours[visitedVertex] == GRAY) return EXIT_SUCCESS_;

    if (colours[visitedVertex] == BLACK) return EXIT_FAILURE_;

    colours[visitedVertex] = GRAY;
    for (int i = 1; i <= vertNum; i++) {
        if (matrix[visitedVertex * vertNum + i] == 1) {
            if (colours[i] == GRAY) 
                return EXIT_SUCCESS_;
            if (!DFS(matrix, colours, stackOfSortedVertexes, i,
                     visitedVertexInd, vertNum))
                return EXIT_SUCCESS_;
        }
    }
    colours[visitedVertex] = BLACK;
    stackOfSortedVertexes[*visitedVertexInd] = visitedVertex;
    (*visitedVertexInd)++;
    return EXIT_FAILURE_;
}

static int TopSort(unsigned char *matrix, char *colours,
             unsigned int *stackOfSortedVertexes, const int vertNum) {
    int index = 0;
    for (int i = 1; i <= vertNum; i++) 
        if (!colours[i])  
            if (!DFS(matrix, colours, stackOfSortedVertexes, i,
                     &index, vertNum)) 
                return EXIT_SUCCESS_;
    return EXIT_FAILURE_;
}

int FillEdgesToMatrix(const int edgeNum, const int vertexNum,
                      unsigned char *matrix) {
    int startNode = 0, endNode = 0;
    for (int i = 0; i < edgeNum; i++) {
        if (scanf("%d %d", &startNode, &endNode) != 2)
            return BAD_NUMBER_OF_LINES;
        else if (!IsBadVertex(startNode, endNode, vertexNum))
            return BAD_VERTEX;
        int index = startNode * vertexNum + endNode;
        matrix[index] = 1;
    }
    return EXIT_SUCCESS_;
}

void InitColours(char *colours, const unsigned int vertexNum){
    for (unsigned int i = 1; i <= vertexNum; i++) 
        colours[i] = WHITE;
}

void FillRes(typeOfError resultOfFilling){
    switch (resultOfFilling) {
        case BAD_NUMBER_OF_LINES:
            PrintMistakes(BAD_NUMBER_OF_LINES);
            break;
        case BAD_VERTEX:
            PrintMistakes(BAD_VERTEX);
            break;
        default:;
    }
}

void FinishProgram(const unsigned int vertexNum, unsigned char *matrix,
                     unsigned int *stackOfSortedVertexes, char *colours){
    int possibilityToSort = 1;
    if (!TopSort(matrix, colours, stackOfSortedVertexes, vertexNum)) 
        possibilityToSort = 0;
    
    if (possibilityToSort) 
        for (int i = vertexNum - 1; i >= 0; i--) 
            printf("%u ", stackOfSortedVertexes[i]);

    if (!possibilityToSort) 
        PrintMistakes(IMPOSSIBLE_TO_SORT);  
}
