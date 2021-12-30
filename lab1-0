#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 1025

void CreateTable(const char *pattern, const int lengthOfPattern, int table[]) {
  for (int i = 0; i < 256; i++) {
    table[i] = lengthOfPattern;
  }

  for (int j = 0; j < lengthOfPattern - 1; j++) {
    table[(unsigned char)pattern[j]] = lengthOfPattern - j - 1;
  }

  int counter = 0, nextToLast = 0;
  for (int i = 0; i < lengthOfPattern - 1; i++) {
    if (pattern[lengthOfPattern - 1] == pattern[i]) {
      counter++;
      nextToLast = i;
    }
  }
  if (counter)
    table[(unsigned char)pattern[lengthOfPattern - 1]] =
        lengthOfPattern - nextToLast - 1;
  else
    table[(unsigned char)pattern[lengthOfPattern - 1]] = lengthOfPattern;
}

void FindAllIntromissions(const char *pattern, const char *string,
                          const size_t lengthOfPattern, const size_t lengthOfString,
                          int *table, size_t lengthPartOfString) {
  int i = lengthOfPattern - 1;
  do {
    int indexPattern = lengthOfPattern - 1;
    int indexString = i;
    printf("%zu ", lengthPartOfString + i + 1);
    int numberOfMatchedSymbols = 0;
    while ((indexPattern >= 0) &&
           (pattern[indexPattern] == string[indexString])) {
      numberOfMatchedSymbols++;
      indexString--;
      indexPattern--;
      if (indexPattern >= 0) {
        printf("%zu ", lengthPartOfString + indexString + 1);
      }
    }
    if (numberOfMatchedSymbols > 0) {
      i += table[(unsigned char)pattern[lengthOfPattern - 1]];
    } else {
      i += table[(unsigned char)string[i]];
    }
  } while (((unsigned)i < lengthOfString));
}

int main(void) {
  FILE *const inputFile = fopen("in.txt", "r");

  char pattern[LENGTH] = {0};
  if (fgets(pattern, 18, inputFile) == NULL) {
    fclose(inputFile);
    return 0;
  }
  size_t lengthOfPattern = strlen(pattern);
  if (pattern[lengthOfPattern - 1] == '\n') {
    pattern[lengthOfPattern - 1] = 0;
    --lengthOfPattern;
  }

  int massiveOfSymbols[256];
  CreateTable(pattern, lengthOfPattern, massiveOfSymbols);

  char string[LENGTH] = {0};
  size_t lengthPartOfString = 0;
  while (fgets(string, LENGTH, inputFile) != NULL) {
    const size_t lengthOfString = strlen(string);
    FindAllIntromissions(pattern, string, lengthOfPattern, lengthOfString,
                         massiveOfSymbols, lengthPartOfString);
    lengthPartOfString += lengthOfString;
  }
  fclose(inputFile);
}
