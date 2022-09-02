#ifndef STUFF_H_
#define STUFF_H_

#include <stdio.h>

struct FileHandler;
typedef struct HuffTree HuffTree;
typedef struct CodingTable CodingTable;

void FillFreqTable(size_t *frequencyTable, size_t *totalNumber,
                   struct FileHandler *fin);
size_t CountSizeCodingTable(void);
CodingTable *CreateNewCodeElement(unsigned int *code, const int size);
void DestroyCodeTable(CodingTable **codeTable);

void AddBitInCodeElement(CodingTable *element, const unsigned char bit);
void UpdateBuffer(CodingTable *element);
unsigned int *CopyCodeElement(CodingTable *copyFrom);

void NullifyCodeTable(CodingTable **codeTable);
void InitNewCodeElement(CodingTable *newCodeElement);

void Swap(HuffTree **fisrtTree, HuffTree **secondTree);

void PrintCompressedText(struct FileHandler *fin, struct FileHandler *fout,
                         CodingTable **codeTable);

size_t GetBuff(CodingTable *table);
unsigned int *GetBinCode(CodingTable *table);

#endif  // STUFF_H_
