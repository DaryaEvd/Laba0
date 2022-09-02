#include "handlerfile.h"
#include "huffalgo.h"

int main() {
    FILE *fin = OpenFile("in.txt", "rb");
    if (!fin) return 0;

    FILE *fout = OpenFile("out.txt", "wb");
    if (!fout) {
        fclose(fin);
        return 0;
    }

    FileHandler *fhReader = NewFileReader(fin);
    if (!fhReader) {
        CloseStreams(fin, fout);
        return 0;
    }

    FileHandler *fhWriter = NewFileWriter(fout);
    if (!fhWriter) {
        CloseFileHandler(fhReader);
        CloseStreams(fin, fout);
        return 0;
    }

    const long fileSize = CountFileSize(fin);

    if (fileSize <= 1) {
        CloseFileHandler(fhReader);
        CloseFileHandler(fhWriter);
        CloseStreams(fin, fout);
        return 0;
    }

    fseek(fin, 0L, SEEK_SET);

    unsigned char directionType;

    if (!FhReadBytes(&directionType, sizeof(unsigned char), 1, fhReader)) {
        CloseFileHandler(fhReader);
        CloseFileHandler(fhWriter);
        CloseStreams(fin, fout);
        return 0;
    }

    switch (directionType) {
        case 'c':
            CompressAlgo(fhReader, fhWriter);
            break;
        case 'd':
            DecompressAlgo(fhReader, fhWriter);
            break;
        default: ;
    }

    CloseFileHandler(fhReader);
    CloseFileHandler(fhWriter);
    CloseStreams(fin, fout);
    return 0;
}
