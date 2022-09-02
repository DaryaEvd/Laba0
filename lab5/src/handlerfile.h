#ifndef HANDLERFILE_H_
#define HANDLERFILE_H_

#include <stdio.h>

typedef struct FileHandler FileHandler;

struct FileHandler *NewFileHandler(FILE *file);
struct FileHandler *NewFileWriter(FILE *file);
struct FileHandler *NewFileReader(FILE *file);

void FhFlush(struct FileHandler *fh);
void FhWrite(int c, struct FileHandler *fh);

int FhRead(struct FileHandler *fh);
size_t FhReadBytes(void *buff, size_t size, size_t nmembs, struct FileHandler *fh);
size_t fhWriteBytes(void *buff, size_t size, size_t nmembs, struct FileHandler *fh);

void CloseFileHandler(struct FileHandler *fh);

FILE *OpenFile(const char *filePath, const char *mode);
void CloseStreams(FILE *fin, FILE *fout);

void InitFileHandler(struct FileHandler *fin);
void WriteBits(struct FileHandler *fout, const int bitsAmount, const unsigned char bit);

unsigned int GetOneBit(FileHandler *fin);
unsigned char GetEightBits(FileHandler *fin);

long CountFileSize(FILE *fin);
size_t GetCurrByte(struct FileHandler *fh);
unsigned char GetCharacter(struct FileHandler *fh);

#endif //HANDLERFILE_H_
