#ifndef HUFFALGO_H_
#define HUFFALGO_H_

struct FileHandler;

void CompressAlgo(struct FileHandler *fin, struct FileHandler *fout);
void DecompressAlgo(struct FileHandler *fin, struct FileHandler *fout);

#endif //HUFFALGO_H_
