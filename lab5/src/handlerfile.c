#include "handlerfile.h"
#include <stdlib.h>

struct FileHandler {
    unsigned char character;
    size_t length; 
    FILE *file;
    unsigned char buff[8];
    size_t bytesInBuff;
    size_t currentReadByte;
    unsigned char eof;
};

FileHandler *NewFileHandler(FILE *file) {
    FileHandler *fw = malloc(sizeof(FileHandler));
    if (!fw) return NULL;
    fw->file = file;
    fw->bytesInBuff = 0;
    fw->eof = 0;
    return fw;
}

FileHandler *NewFileWriter(FILE *file) {
    FileHandler *fh = NewFileHandler(file);
    if (!fh) return NULL;
    return fh;
}

FileHandler *NewFileReader(FILE *file) {
    FileHandler *fh = NewFileHandler(file);
    if(!fh) return NULL;
    fh->currentReadByte = 0;
    return fh;
}

void FhFlush(FileHandler *fh) {
    fwrite(fh->buff, 1, fh->bytesInBuff, fh->file);
    fh->bytesInBuff = 0;
}

void FhWrite(int c, FileHandler *fh) {
    fh->buff[fh->bytesInBuff++] = (unsigned char)c;
    if (fh->bytesInBuff == 8) {
        fwrite(fh->buff, 1, 8, fh->file);
        fh->bytesInBuff = 0;
    }
}

int FhRead(FileHandler *fh) {
    if (fh->currentReadByte < fh->bytesInBuff) {
        return (int)fh->buff[fh->currentReadByte++];
    }
    if (fh->eof) {
        return EOF;
    }
    fh->bytesInBuff = fread(fh->buff, 1, 8, fh->file);
    fh->currentReadByte = 0;
    if (fh->bytesInBuff < 8 && feof(fh->file)) {
        fh->eof = 1;
    }
    return FhRead(fh);
}

size_t FhReadBytes(void *buff, size_t size, size_t nmembs, FileHandler *fh) {
    char *ubuff = (char *)buff;
    size_t read = 0;
    for (size_t i = 0; i < nmembs * size; i++) {
        int c;
        if ((c = FhRead(fh)) == EOF) {
            return read / size;
        }
        ubuff[i] = c;
        read++;
    }
    return read / size;
}

size_t fhWriteBytes(void *buff, size_t size, size_t nmembs, FileHandler *fh) {
    FhFlush(fh);
    return fwrite(buff, size, nmembs, fh->file);
}

void CloseFileHandler(FileHandler *fh) {
    if (fh->bytesInBuff > 0) {
        FhFlush(fh);
    }
    free(fh);
}

FILE *OpenFile(const char *filePath, const char *mode) {
    FILE *file;
    return (file = fopen(filePath, mode));
}

void CloseStreams(FILE *fin, FILE *fout) {
    fclose(fin);
    fclose(fout);
}

void InitFileHandler(struct FileHandler *fin) {
    fseek(fin->file, 1, SEEK_SET);
    fin->bytesInBuff = 0;
    fin->currentReadByte = 0;
    fin->eof = 0;
    fin->character = 0;
    fin->length = 0;
}

void WriteBits(FileHandler *fout,  
               const int bitsAmount, const unsigned char bit) {
    const int shift = 8 - fout->currentReadByte;
    if (shift >= bitsAmount) {
        fout->character = fout->character << bitsAmount;
        const unsigned char mask = (1 << bitsAmount) - 1;
        fout->character += (bit & mask);
        fout->currentReadByte += bitsAmount;
        if (fout->currentReadByte == 8) {
            FhWrite(fout->character, fout);
            fout->currentReadByte = 0;
            fout->character = 0;
        }        
    } 
    else {
        WriteBits(fout, shift, bit >> (bitsAmount - shift));
        WriteBits(fout,  bitsAmount - shift, bit);
    }
}

unsigned int GetOneBit(FileHandler *fin) {
    if (!fin->length) {
        fin->character = FhRead(fin);
        fin->length = 8;
    }
    unsigned int symbol = fin->character >> (fin->length - 1);
    fin->length--;
    return (symbol & 1);
}

unsigned char GetEightBits(FileHandler *fin) {
    unsigned char byte = 0;
    for (size_t i = 0; i < 8; i++) {
        byte = ((byte << 1)) + GetOneBit(fin);
    }
    return byte;
}

long CountFileSize(FILE *fin) {
    fseek(fin, 0L, SEEK_END);
    long lastPosition = ftell(fin);
    return lastPosition;
}

size_t GetCurrByte(struct FileHandler *fh){
    return fh->currentReadByte;
}

unsigned char GetCharacter(struct FileHandler *fh){
    return fh->character;
}
