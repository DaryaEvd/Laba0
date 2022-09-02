#include "handlerfile.h"
#include "stuff.h"

#include <stdlib.h>

struct CodingTable {
    unsigned int *binaryCode;
    size_t lengthCode;
};

void FillFreqTable(size_t *frequencyTable, size_t *totalNumber,
                   FileHandler *fin) {
    unsigned char bufferRread[256];
    int readLength;
    while ((readLength = FhReadBytes(bufferRread, 1, 256, fin))) {
        for (int i = 0; i < readLength; i++) {
            if (!frequencyTable[bufferRread[i]]) {
                totalNumber++;
            }
            frequencyTable[bufferRread[i]]++;
        }
    }
}

size_t CountSizeCodingTable(void){
    return sizeof(CodingTable);
}

CodingTable *CreateNewCodeElement(unsigned int *code, const int lengthCode) {
    CodingTable *element = malloc(sizeof(CodingTable) * 256);
    if (!element) return NULL;

    element->binaryCode = code;
    element->lengthCode = lengthCode;
    return element;
}

void DestroyCodeTable(CodingTable **codeTable) {
    for (size_t i = 0; i < 256; i++) {
        if (codeTable[i]) {
            free(codeTable[i]->binaryCode);
            free(codeTable[i]);
        }
    }
    free(codeTable);
}

void AddBitInCodeElement(CodingTable *element, const unsigned char bit) {
    const int pos = (element->lengthCode - 1) / 8;
    element->binaryCode[pos] = (element->binaryCode[pos] << 1) + bit;
}

void UpdateBuffer(CodingTable *buffer) {
    buffer->lengthCode++;
    const int isOverflow = buffer->lengthCode % 8;
    if (isOverflow == 1) {
        int bytes = 256 - buffer->lengthCode / 8 + 1;
        buffer->binaryCode = realloc(
            buffer->binaryCode, sizeof(unsigned int) * (256 + 1));
        buffer->binaryCode[bytes - 1] = 0;
    }
}

unsigned int *CopyCodeElement(CodingTable *copyFrom) {
    unsigned int *newSymb = malloc(sizeof(unsigned int) * 256);
        if (!newSymb) return 0;
    const int bytes = (copyFrom->lengthCode - 1) / 8 + 1;  
    for (int i = 0; i < bytes; i++) 
        newSymb[i] = copyFrom->binaryCode[i];
    return newSymb;
}

void NullifyCodeTable(CodingTable **codeTable) {
    for (int i = 0; i < 256; i++) {
        codeTable[i] = 0;
    }
}

void InitNewCodeElement(CodingTable *newCodeElement) {
    newCodeElement->binaryCode = NULL;
    newCodeElement->lengthCode = 0;
}

void Swap(HuffTree **fisrtTree, HuffTree **secondTree) {
    HuffTree *tmp = *fisrtTree;
    *fisrtTree = *secondTree;
    *secondTree = tmp;
}

void PrintCompressedText(FileHandler *fin, FileHandler *fout, CodingTable **codeTable) {
    int readedSymb = 0;
    size_t index = 0;
    while ((readedSymb = FhRead(fin)) != EOF) {
        for (index = 0; index < codeTable[readedSymb]->lengthCode / 8; index++) {
            WriteBits(fout, 8, codeTable[readedSymb]->binaryCode[index]);
        }
        WriteBits(fout,  codeTable[readedSymb]->lengthCode % 8,
                  codeTable[readedSymb]->binaryCode[index]);
    }
    if (GetCurrByte(fout)) {
        FhWrite(GetCharacter(fout) << (8 - GetCurrByte(fout)), fout);
    }
}

size_t GetBuff(CodingTable *element){ 
    return element->lengthCode;
}

unsigned int *GetBinCode(CodingTable *element){
    return element->binaryCode;
}
