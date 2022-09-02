#include "kruskal.h"

#define MAX_VERTEX_NUMBER 5000

void PrintError(codeStatus error) {
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

unsigned int ReadAndCheckVertex() {
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

unsigned int ReadAndCheckEdge(const int vertexNum) {
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

static void CreateSet(unsigned int *parents, unsigned int *rank, const unsigned int vertNum) {
    for (size_t i = 0; i < vertNum; i++) {
        parents[i] = i;
        rank[i] = 0;
    }
}

static int FindSet(const unsigned int vertNum, unsigned int *parents) {
    if (parents[vertNum] != vertNum) 
        parents[vertNum] = FindSet(parents[vertNum], parents);
    return parents[vertNum];
}

static int UnionizeSets(unsigned int subSet1, unsigned int subSet2, unsigned int *rank, unsigned int *parents) {
    int res = EXIT_SUCCESS_;
    subSet1 = FindSet(subSet1, parents);
    subSet2 = FindSet(subSet2, parents);

    if (subSet1 == subSet2)
        return res;

    if (rank[subSet1] < rank[subSet2]) {
        parents[subSet1] = subSet2;
        res = EXIT_FAILURE_;
    }

    else {
        parents[subSet2] = subSet1;
        if (rank[subSet1] == rank[subSet2]) {
            rank[subSet1]++;
        }
        res = EXIT_FAILURE_;
    }
    return res;
}

static void FreeSubsets(unsigned int *parents, unsigned int *rank){
    free(parents);
    free(rank);
}

struct Edge {
    int start;
    int end;
    int weight;
};

size_t EdgeSize() {
    return sizeof(Edge);
}

struct Graph {
    Edge *edges;
    int indexEdge;
};

Graph *InitGraph(const int edgeNum) {
    Graph *res = malloc(sizeof(Graph));
    if(!res) return NULL;

    res->edges = malloc(edgeNum * sizeof(Graph));
    if(!res->edges){
        free(res);
        return NULL;
    }

    res->indexEdge = 0;
    return res;
}

void DeleteGraph(Graph *graph){
    free(graph->edges);
    free(graph);
}

static void PushEdge(const unsigned int start, const unsigned int end, const unsigned int weight, Graph *graph) {
    graph->edges[graph->indexEdge].start = start;
    graph->edges[graph->indexEdge].end = end;
    graph->edges[graph->indexEdge].weight = weight;
    graph->indexEdge ++;
}

int AddEdgesToGraph(Graph *graph, const unsigned int vertexNum, const unsigned int edgeNum){
    int from, to;
    long long length;
    for (size_t i = 0; i < edgeNum; i++) {
        if (scanf("%d %d %lld", &from, &to, &length) != 3) 
            return BAD_NUMBER_OF_LINES;        

        if (!IsBadVertex(from, to, vertexNum))
            return BAD_VERTEX;

        if (!IsCorrectLength(length)) 
            return BAD_LENGTH;        

        PushEdge(from, to, length, graph);
    }
    return EXIT_SUCCESS_;
}

void PrintResOfAdding(codeStatus resultOfFilling){
    switch (resultOfFilling) {
        case BAD_NUMBER_OF_LINES:
            PrintError(BAD_NUMBER_OF_LINES);
            break;
        case BAD_VERTEX:
            PrintError(BAD_VERTEX);
            break;
        case BAD_LENGTH:
            PrintError(BAD_LENGTH);
            break;
        default:;
    }
}

static int CompareWeightOfEdges(const void *edge1, const void *edge2){
    return ((Edge *) edge1)->weight - ((Edge *) edge2)->weight;
}

size_t KruskalAlgorythm(Graph *graph, const unsigned int vertexNum, const unsigned int edgeNum, Edge *minFrame) {
    qsort(graph->edges, edgeNum, sizeof(Edge), CompareWeightOfEdges);
    
    unsigned int *parents = malloc(sizeof(int) * vertexNum);
    if(!parents) return EXIT_SUCCESS_;
    unsigned int *rank = malloc(sizeof(int) * vertexNum);
    if(!rank){
        free(parents);
        return EXIT_SUCCESS_;
    }
        
    CreateSet(parents, rank, vertexNum);

    size_t idxOfVisitedEdge = 0, numOfAddedEdges = 0; 

    while ((numOfAddedEdges < vertexNum - 1) && (idxOfVisitedEdge < edgeNum)) {
        Edge edge = graph->edges[idxOfVisitedEdge];
        if (UnionizeSets(edge.start - 1, edge.end - 1, rank, parents)) {
            minFrame[numOfAddedEdges] = edge;
            numOfAddedEdges++;
        }
        idxOfVisitedEdge++;
    }
    FreeSubsets(parents, rank);
    return numOfAddedEdges;
}

void PrintMinimalSpanningTree(Edge *minFrame, const size_t countOFAddedEdges){
    for (size_t i = 0; i < countOFAddedEdges; i++) {
        Edge edge = minFrame[i];
        printf("%d %d \n", edge.start, edge.end);
    }
}
