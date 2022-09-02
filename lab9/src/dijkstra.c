#include "dijkstra.h"

#define ll long long
#define INT_MAX_PLUS_ONE 2147483648L

typedef enum{
    MAX_VERTEX_NUMBER = 5000,
    MAX_PATH_LENGTH = 2,
    ROOT = -1
} values;

typedef enum{
    NOT_VISITED,
    VISITED    
} statusOfVisit;

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

typeOfError IsOKInput(int vertexNum, int startNode, int endNode, int edgeNum){    
    if(!IsCorrectVertexesNumber(vertexNum)){
        return BAD_NUMBER_OF_VERTEX;
    }
    if(!IsBadVertex(startNode, endNode, vertexNum)){
        return BAD_VERTEX;
    }
    if(!IsCorrectEdgesNumber(edgeNum, vertexNum)){
        return BAD_NUMBER_OF_EDGES;
    } 
    return EXIT_SUCCESS_;
}

void PrintRes(typeOfError resultOfFilling){
    switch (resultOfFilling) {
        case BAD_NUMBER_OF_VERTEX:
            PrintError(BAD_NUMBER_OF_VERTEX);
            break;
        case BAD_VERTEX:
            PrintError(BAD_VERTEX);
            break;
        case BAD_NUMBER_OF_EDGES:
            PrintError(BAD_NUMBER_OF_EDGES);
            break;
        default:;
    }
}

struct EdgeConstructor{
    unsigned int dist;
    int parents;
    int lengthOFWays;
    int visited;
};

size_t CalcSizeEdgeConstructor(){
    return sizeof(EdgeConstructor);
}

static int FindClosestMinVertex(EdgeConstructor *edge, int vertexNum){
    int minIndex = ROOT;
    for (int i = 0 ; i < vertexNum; i++){
        if (!edge[i].visited && (minIndex == ROOT || 
                    edge[i].dist < edge[minIndex].dist)){
            minIndex = i;
        }
    }
    return minIndex;
}

static void InitFieldsOfStruct(EdgeConstructor *edge, int vertexNum){
    for (int i = 0; i < vertexNum; i++){
        edge[i].lengthOFWays  = 0;
        edge[i].visited = NOT_VISITED;
        edge[i].dist = UINT_MAX;        
    }
}

static void Relaxation(EdgeConstructor *edge, int *matrix, int vertexNum, int minVert){
    for (int j = 0; j < vertexNum; j++){
        if(matrix[minVert * vertexNum + j] != ROOT){
            if ((ll) matrix[minVert * vertexNum + j] + 
            (ll) edge[minVert].dist < (ll) edge[j].dist){
                edge[j].parents = minVert;

                if ((ll) matrix[minVert * vertexNum + j] + 
                            (ll) edge[minVert].dist > INT_MAX)
                    edge[j].dist = INT_MAX_PLUS_ONE;                

                edge[j].dist = matrix[minVert * vertexNum + j] + edge[minVert].dist;
            }
            
            if (edge[j].lengthOFWays <= MAX_PATH_LENGTH)
                edge[j].lengthOFWays += edge[minVert].lengthOFWays;
        }
    }
}

void DijkstraAlgorythm(EdgeConstructor *edge, int *matrix, const unsigned int 
                                            vertexNum, const unsigned int startNode){
    InitFieldsOfStruct(edge, vertexNum);
    edge[startNode].dist  = 0;
    edge[startNode].lengthOFWays = 1;
    
    for (size_t i = 0; i < vertexNum; i++){
        int minVert = FindClosestMinVertex(edge, vertexNum);
        if (edge[minVert].dist == UINT_MAX)
            break;
        edge[minVert].visited = VISITED;
        Relaxation(edge, matrix, vertexNum, minVert);         
    }
}

void PrintFirstLine(EdgeConstructor *edge, const unsigned int vertexNum){
    for (size_t i = 0; i < vertexNum; i++){
        if (edge[i].dist == UINT_MAX)
            printf("oo ");
        else if (edge[i].dist > INT_MAX)
            printf("INT_MAX+ ");
        else
            printf("%u ", edge[i].dist);
    } 
}

int PrintSecondLine(EdgeConstructor *edge, const unsigned int startNode, unsigned int endNode){
    if (edge[endNode].dist == UINT_MAX){
        printf("no path");
        return EXIT_SUCCESS_;       
    }
    if (edge[endNode].dist > INT_MAX && 
                        edge[endNode].lengthOFWays >= MAX_PATH_LENGTH){
        printf("overflow");
        return EXIT_SUCCESS_;
    }

    printf("%u ", endNode + 1);    
    while(endNode != startNode){
        endNode = edge[endNode].parents;
        printf("%u ", endNode + 1);
    }
    return EXIT_SUCCESS_;
}

static void InitMatrCells(int *matrix, const unsigned int vertexNum){
    for(size_t i = 0; i < vertexNum; i++)
        for(size_t j = 0; j < vertexNum; j++)
            matrix[i * vertexNum + j] = ROOT;
}

size_t InitMatrixValues(int *matrix, const unsigned int vertexNum, const unsigned int edgeNum){
    InitMatrCells(matrix, vertexNum);
    for(size_t i = 0; i < edgeNum; i++){       
        int from, to;
        long long length;
        if (scanf("%d %d %lld", &from, &to, &length) != 3) {
            PrintError(BAD_NUMBER_OF_LINES);
            return EXIT_SUCCESS_;
        }
        if (!IsBadVertex(from, to, vertexNum)) {
            PrintError(BAD_VERTEX);
            return EXIT_SUCCESS_;
        }
        if (!IsCorrectLength(length)) {
            PrintError(BAD_LENGTH);
            return EXIT_SUCCESS_;
        }

        const int idxRow = (from - 1) * vertexNum + (to - 1); 
        const int idxColumn = (to - 1) * vertexNum + (from - 1);
        matrix[idxRow] = matrix[idxColumn] = length;
    } 
    return *matrix;
}

void FreeStructures(int *matrix, EdgeConstructor *edge){
    free(matrix);
    free(edge);
}
