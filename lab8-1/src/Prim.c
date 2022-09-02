#include "Prim.h"

#define ROOT (-1)

#define INFINITY 2147483648
#define MAX_VERTEX_NUMBER 5000

typedef enum{
    addedToMST,
    notAddedToMST
} chackAddToMST;

typedef enum { 
    NO_SP_TREE, 
    HAVE_SP_TREE 
} checkingMST;

void PrintError(typeOfError error) {
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
        case BAD_LENGTH:
            printf("bad length");
            break;
        case NO_SPANNING_TREE:
            printf("no spanning tree");
            break;
        default:;
    }
}

static int IsCorrectVertexesNumber(const int vertexNum) {
    return (!(vertexNum < 0 || vertexNum > MAX_VERTEX_NUMBER));
}

static int IsCorrectEdgesNumber(const int edgeNum, const int vertexNum) {
    return (!(edgeNum < 0 || edgeNum > (vertexNum * (vertexNum - 1) / 2)));
}

static int IsCorrectLength(const long long length) {
    return (!(length < 0 || length > INT_MAX));
}

static int IsBadVertex(const int startNode, const int endNode, const int vertexNum) {
    return (!(startNode <= 0 || startNode > vertexNum || endNode <= 0 ||
              endNode > vertexNum));
}

unsigned int IsCorrectInputVertex() {
    int countOfVertex = 0;
    if (scanf("%d", &countOfVertex) != 1) {
        PrintError(BAD_NUMBER_OF_LINES);
        return EXIT_SUCCESS_;
    }
    if (!IsCorrectVertexesNumber(countOfVertex)) {
        PrintError(BAD_NUMBER_OF_VERTEX);
        return EXIT_SUCCESS_;
    }
    if (countOfVertex == 0) {
        PrintError(NO_SPANNING_TREE);
        return EXIT_SUCCESS_;
    }
    return countOfVertex;
}

unsigned int IsCorrectInputEdge(const int vertexNum) {
    int countOfEdge = -1;
    if (scanf("%d", &countOfEdge) == -1) {
        PrintError(BAD_NUMBER_OF_LINES);
        return EXIT_SUCCESS_;
    }
    if (!IsCorrectEdgesNumber(countOfEdge, vertexNum)) {
        PrintError(BAD_NUMBER_OF_EDGES);
        return EXIT_SUCCESS_;
    }
    return countOfEdge;
}

int haveMST(const int vertexNum, const int edgeNum) {
    if ((vertexNum == 0) || (edgeNum == 0 && vertexNum != 1) ||
        (edgeNum < vertexNum - 1)) {
        PrintError(NO_SPANNING_TREE);
        return EXIT_SUCCESS_;
    }
    return EXIT_FAILURE_;
}

struct EdgeConstructor {
    int parent;
    unsigned long long minKey;
    int inMST;
};

size_t CalcSizeEdgeConstructor(){
    return sizeof(EdgeConstructor);
}

struct EdgeAns {
    unsigned int vertex1;
    unsigned int vertex2;
};

size_t CalcEdgeAns(){
    return sizeof(EdgeAns);
}

static void InitFieldsInStructure(EdgeConstructor *edgePrim,
                           const unsigned int vertexNum) {
    for (size_t i = 0; i < vertexNum; i++) {
        edgePrim[i].parent = ROOT;
        edgePrim[i].minKey = INFINITY;
        edgePrim[i].inMST = notAddedToMST;
    }
}

static int FindMinValue(EdgeConstructor *edgePrim,
                 const unsigned int vertexNum) {
    int vertexInd = ROOT;
    for (size_t j = 0; j < vertexNum; j++) {
        if (edgePrim[j].inMST == notAddedToMST &&
            (vertexInd == ROOT ||
             edgePrim[j].minKey < edgePrim[vertexInd].minKey)) {
            vertexInd = j;
        }
    }
    return vertexInd;
}

int PrimMST(EdgeConstructor *edgePrim, unsigned int *matrix,
             const unsigned int vertexNum, EdgeAns *ans) {
    int spanTree = HAVE_SP_TREE;
    InitFieldsInStructure(edgePrim, vertexNum);

    unsigned int countOfAddedEdges = 0;
    edgePrim[0].minKey = 0;

    for (size_t i = 0; i < vertexNum; i++) {
        const unsigned int minValue = FindMinValue(edgePrim, vertexNum);

        if (edgePrim[minValue].minKey == INFINITY) {
            spanTree = NO_SP_TREE;
            break;
        }

        edgePrim[minValue].inMST = addedToMST;

        for (size_t k = 0; k < vertexNum; k++) {
            if (matrix[minValue * vertexNum + k] < edgePrim[k].minKey) {
                edgePrim[k].parent = minValue;
                edgePrim[k].minKey = matrix[minValue * vertexNum + k];
            }
        }

        if (edgePrim[minValue].parent != ROOT) {
            ans[countOfAddedEdges].vertex1 = minValue;
            ans[(vertexNum - 1) + countOfAddedEdges].vertex2 =
                edgePrim[minValue].parent;
            countOfAddedEdges++;
        }
    }
    return spanTree;
}

void InitCellsInMatrix(unsigned int *matrix, const unsigned int vertexNum) {
    for (size_t i = 0; i < vertexNum; i++) {
        for (size_t j = 0; j < vertexNum; j++) {
            matrix[i * vertexNum + j] = INFINITY;
        }
    }
}

unsigned int FillValuesInMatrix(unsigned int *matrix,
                                const unsigned int edgeNum,
                                const unsigned int vertexNum) {
    for (size_t i = 0; i < edgeNum; i++) {
        int startNode = 0, endNode = 0;
        long long weight;
        if (scanf("%d %d %lld", &startNode, &endNode, &weight) != 3) {
            PrintError(BAD_NUMBER_OF_LINES);
            return EXIT_FAILURE_;
        }
        if (!IsBadVertex(startNode, endNode, vertexNum)) {
            PrintError(BAD_VERTEX);
            return EXIT_FAILURE_;
        }
        if (!IsCorrectLength(weight)) {
            PrintError(BAD_LENGTH);
            return EXIT_FAILURE_;
        }

        unsigned int idxRow = (startNode - 1) * vertexNum + (endNode - 1);
        unsigned int idxColumn = (endNode - 1) * vertexNum + (startNode - 1);
        matrix[idxRow] = matrix[idxColumn] = weight;
    }
    return *matrix;
}

void PrintMST(EdgeAns *ans, const unsigned int vertexNum) {
    for (size_t i = 0; i < vertexNum - 1; i++) {
        printf("%u %u \n", ans[i].vertex1 + 1, ans[(vertexNum - 1) + i].vertex2 + 1);
    }
}

void FreeStructures(unsigned int *matrix, EdgeConstructor *edge, EdgeAns *ans) {
    free(matrix);
    free(edge);
    free(ans);
}
