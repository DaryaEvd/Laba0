#include "handlerfile.h"
#include "huffalgo.h"
#include "stuff.h"

#include <stdlib.h>

struct HuffTree {
    unsigned char symbol;
    unsigned int frequency;
    struct HuffTree *leftChild;
    struct HuffTree *rightChild;
};

static void DestroyTree(struct HuffTree *tree) {
    if (!tree) return;
    if (tree->leftChild) DestroyTree(tree->leftChild);
    if (tree->rightChild) DestroyTree(tree->rightChild);
    free(tree);
}

static void DestroyMemoryForCompression(CodingTable **codeTable, HuffTree *root,
                                 HuffTree **trees) {
    DestroyCodeTable(codeTable);
    DestroyTree(root);
    free(trees);
}

static void NullifyHuffmanTrees(HuffTree **trees) {
    for (int i = 0; i < 256; i++) {
        trees[i] = 0;
    }
}

static void InitAmountOfTrees(HuffTree **trees, const size_t *frequencyTable,
                      size_t *diffSymbols) {
    for (size_t i = 0; i < 256; i++) {
        if (frequencyTable[i] > 0) {            
            trees[(*diffSymbols)] = malloc(sizeof(HuffTree));
            if (!trees[(*diffSymbols)]) return;
            trees[*diffSymbols]->symbol = i;
            trees[*diffSymbols]->frequency = frequencyTable[i];
            trees[*diffSymbols]->leftChild = NULL;
            trees[*diffSymbols]->rightChild = NULL;
 
            (*diffSymbols)++;
        }
    }
}

static int IsVertexLeaves(HuffTree *tree) {
    return (tree->leftChild == NULL && tree->rightChild == NULL);
}

static void MergeTrees(HuffTree **trees, HuffTree *merged) {
    merged->frequency = trees[0]->frequency + trees[1]->frequency;
    merged->leftChild = trees[0];
    merged->rightChild = trees[1];
    trees[0] = merged;
}

static void Heapify(HuffTree **trees, size_t root, const size_t heapSize) {
    size_t largestRoot = root;
    size_t left = (largestRoot * 2) + 1;
    size_t right = (largestRoot * 2) + 2;
    if (left < heapSize &&
        trees[largestRoot]->frequency < trees[left]->frequency) {
        largestRoot = left;
    }
    if (right < heapSize &&
        trees[largestRoot]->frequency < trees[right]->frequency) {
        largestRoot = right;
    }
    if (largestRoot != root) {
        Swap(&trees[largestRoot], &trees[root]);
        Heapify(trees, largestRoot, heapSize);
    }
}

static void HeapSort(HuffTree **trees, size_t differentSymbols) {
    size_t frequency = differentSymbols;
    for (int i = frequency / 2 - 1; i >= 0; i--) 
        Heapify(trees, i, frequency);

    for (int i = differentSymbols - 1; i >= 0; i--) {
        Swap(&trees[0], &trees[differentSymbols - 1]);
        differentSymbols--;
        Heapify(trees, 0, differentSymbols);
    }
}

static HuffTree *BuildHuffmanTree(HuffTree **trees, size_t differentSymbols) {
    size_t index = 0;
    while (differentSymbols != 1) {
        HuffTree *mergedTrees = malloc(sizeof(HuffTree));
        if(!mergedTrees) return NULL;

        MergeTrees(trees, mergedTrees);

        Swap(&trees[index + 1], &trees[differentSymbols - 1]);
        differentSymbols--;
        HeapSort(trees, differentSymbols);
    }
    return trees[0];
}

static void BuildCodeTable(HuffTree *root, CodingTable **codeTable,
                    CodingTable *element) {
    if (!root) return;
    if (IsVertexLeaves(root)) {
        if (!GetBinCode(element)) {
            UpdateBuffer(element);
        }
        codeTable[root->symbol] = element;
    } 
    else {
        UpdateBuffer(element);

        unsigned int *copiedElement = CopyCodeElement(element);

        CodingTable *newElement =
            CreateNewCodeElement(copiedElement, GetBuff(element));

        AddBitInCodeElement(element, 0);
        BuildCodeTable(root->leftChild, codeTable, element);
        AddBitInCodeElement(newElement, 1);
        BuildCodeTable(root->rightChild, codeTable, newElement);
    }
}

static void PrintHuffmanTree(FileHandler *fout, HuffTree *tree) {
    if (!tree) return;

    if (!tree->leftChild) {
        WriteBits(fout,  1, 0);
        WriteBits(fout, 8, tree->symbol);
        return;
    }

    WriteBits(fout,  1, 1);
    PrintHuffmanTree(fout, tree->leftChild);
    PrintHuffmanTree(fout, tree->rightChild);
}

static void DecodeSymbol(HuffTree *tree, FileHandler *fin, FileHandler *fout) {
    if (!IsVertexLeaves(tree)) {
        unsigned int bit = GetOneBit(fin);
        if (bit == 0) {
            DecodeSymbol(tree->leftChild,  fin, fout);
        } 
        else {
            DecodeSymbol(tree->rightChild,  fin, fout);
        }
    } 
    else {
        FhWrite(tree->symbol, fout);
    }
}

static HuffTree *TakeHuffmanTree(FileHandler *fin) {
    HuffTree *tree = malloc(sizeof(HuffTree));
    if (!tree) return NULL;
    unsigned int bit = GetOneBit(fin);

    if (bit) {
        tree->symbol = 0;
        tree->frequency = 0;
        tree->leftChild = TakeHuffmanTree(fin);
        tree->rightChild = TakeHuffmanTree(fin);
    } 
    else {
        tree->symbol = GetEightBits(fin);
        tree->frequency = 0;
        tree->leftChild = NULL;
        tree->rightChild = NULL;
    }
    return tree;
}

void CompressAlgo(struct FileHandler *fin, struct FileHandler *fout) {
    size_t frequencyTable[256] = {0};
    size_t differentSymbols = 0;
    FillFreqTable(frequencyTable, &differentSymbols, fin);

    InitFileHandler(fin);

    HuffTree **trees = malloc(256 * sizeof(HuffTree *));
    if (!trees) return;
    NullifyHuffmanTrees(trees);
    InitAmountOfTrees(trees, frequencyTable, &differentSymbols);
    HuffTree *root = BuildHuffmanTree(trees, differentSymbols);

    CodingTable **codeTable = malloc(256 * sizeof(CodingTable *));
    if (!codeTable) {
        free(trees);
        DestroyTree(root);
        return;
    }
    NullifyCodeTable(codeTable);

    size_t codeTableSize = CountSizeCodingTable();
    CodingTable *newCodeElement = malloc(codeTableSize * 256);
    if (!newCodeElement) {
        DestroyCodeTable(codeTable);
        DestroyTree(root);
        free(trees);
        return;
    }
    InitNewCodeElement(newCodeElement);
    BuildCodeTable(root, codeTable, newCodeElement);     

    if (root) {
        fhWriteBytes(&root->frequency, sizeof(unsigned int), 1, fout);
    }

    PrintHuffmanTree(fout, root);
    PrintCompressedText(fin, fout, codeTable);
    DestroyMemoryForCompression(codeTable, root, trees);
}


void DecompressAlgo(struct FileHandler *fin, struct FileHandler *fout) {
    size_t length = 0;
    if (!FhReadBytes(&length, sizeof(unsigned int), 1, fin)) return;

    HuffTree *huffmanTree = TakeHuffmanTree(fin);

    for (size_t i = 0; i < length; i++) {
        DecodeSymbol(huffmanTree, fin, fout);
    }

    DestroyTree(huffmanTree);
}
