#include "kruskal.h"

int main(void) {
    const unsigned int vertexNum = ReadAndCheckVertex();
    const unsigned int edgeNum = ReadAndCheckEdge(vertexNum);

    Graph *graph = InitGraph(edgeNum); 
    if(!graph) return EXIT_SUCCESS_;

    const int resOfAdding = AddEdgesToGraph(graph, vertexNum, edgeNum);
    if(resOfAdding != EXIT_SUCCESS_){
        PrintResOfAdding(resOfAdding);
        DeleteGraph(graph);
        return EXIT_SUCCESS_;
    }

    const size_t sizeEdge = EdgeSize();
    Edge *minFrame = malloc(sizeEdge * vertexNum);
    if(!minFrame){
        DeleteGraph(graph);
        return EXIT_SUCCESS_;
    }

    const size_t countOFAddedEdges = KruskalAlgorythm(graph, vertexNum, edgeNum, minFrame);

    if (countOFAddedEdges != vertexNum - 1) {
        PrintError(NO_SPANNING_TREE);
        DeleteGraph(graph);
        free(minFrame);
        return EXIT_SUCCESS_;
    }
    PrintMinimalSpanningTree(minFrame, countOFAddedEdges);

    free(minFrame); 
    DeleteGraph(graph);      
}
