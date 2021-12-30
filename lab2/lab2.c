#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALUE 50

int CheckBadInput(char* sequence, size_t lengthOfSequence) {
  int counters[VALUE] = {0};

  for (size_t i = 0; i < lengthOfSequence; ++i) {
    if (sequence[i] > '9' || sequence[i] < '0') {
      return 1;
    }
    ++counters[sequence[i] - '0'];
    if (counters[sequence[i] - '0'] > 1) {
      return 1;
    }
  }
  return 0;
}

void Swap(char* a, char* b) {
  char tmp = *a;
  *a = *b;
  *b = tmp;
}

int GenerateNextPermutation(char* permutation, size_t length) {
  int maxIndex1 = -1, maxIndex2 = -1;
  for (size_t j = 0; j < length - 1; ++j) {
    if (permutation[j] < permutation[j + 1]) {
      maxIndex1 = j;
    }
  }
  if (maxIndex1 == -1) return -1;
  for (size_t l = maxIndex1 + 1; l < length; ++l) {
    if (permutation[l] > permutation[maxIndex1]) {
      maxIndex2 = l;
    }
  }
  if (maxIndex2 == -1) return -1;
  Swap(&permutation[maxIndex1], &permutation[maxIndex2]);
  for (size_t low = maxIndex1 + 1, high = length - 1; low < high; ++low, --high) {
    Swap(&permutation[low], &permutation[high]);
  }
  return 0;
}

int GetLengthOfSequence(char* sequence){
    size_t lengthOfSequence = strlen(sequence);
    if (sequence[lengthOfSequence - 1] == '\n') {
      sequence[lengthOfSequence - 1] = 0;
      --lengthOfSequence;
    }
    return lengthOfSequence;
}

int ReadSequenceWithCheck(char input[VALUE], FILE* const inFile){
  if (fgets(input, 50, inFile) == NULL) {
    fclose(inFile);
    return -1;
  }
  return 0;
}

int main(void) {
  FILE* const inputFile = fopen("in.txt", "r");
  
  if (inputFile == NULL){
    return -1;
  }

  char sequence[VALUE] = {0};  
  ReadSequenceWithCheck(sequence, inputFile);

  char countOfPermutation[VALUE] = {0};
  if (fgets(countOfPermutation, 50, inputFile) == NULL){
    fclose(inputFile);
    return 0;    
  }

  const size_t lengthOfSequence = GetLengthOfSequence(sequence);
  if (CheckBadInput(sequence, lengthOfSequence)) {
    printf("bad input");
    return 0;
  }
  const size_t numberOfPermutations = atoi(countOfPermutation);
  for (size_t i = 0; i < numberOfPermutations; ++i) {
    if (!GenerateNextPermutation(sequence, lengthOfSequence)){
      for (size_t j = 0; j < lengthOfSequence; ++j)
          printf("%i", sequence[j] - '0');
        printf("\n");
    }
    else
      break;
  }
  fclose(inputFile);
}

