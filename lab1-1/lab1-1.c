#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_LENGTH 18
#define COEFF_FOR_RING_BUFFER 2

int CountHash(const char *sequence, size_t lengthOfSequence, const int *powersOfThree) {
    int hashSequence = 0;
    for (size_t i = 0; i < lengthOfSequence; ++i) {
        hashSequence +=
            powersOfThree[i] * (int)((unsigned char)sequence[i] % 3);
    }
    return hashSequence;
}

size_t RabinCarpMatcher(const char *pattern, const char *string,
                        size_t lengthOfString, size_t lengthOfPattern,
                        size_t *matches, const int *powersOfThree, int step) {
    int hashPattern = CountHash(pattern, lengthOfPattern, powersOfThree);
    int hashString = CountHash(string, lengthOfPattern, powersOfThree);

    size_t numberOfMatches = 0;
    for (size_t i = 0; i <= (lengthOfString - lengthOfPattern); ++i) {
        if (hashPattern == hashString) {
            for (size_t j = 0; j < lengthOfPattern; ++j) {
                matches[numberOfMatches++] =
                    i + j + 1 + (step > 1 ? (step - 1) * PATTERN_LENGTH : 0);
                if (string[i + j] != pattern[j]) break;
            }
        }

        int powForNewSymbol = powersOfThree[lengthOfPattern - 1];
        if (i < lengthOfString - lengthOfPattern) {
            hashString = ((hashString - ((unsigned char)string[i] % 3)) / 3) +
                         ((unsigned char)string[i + lengthOfPattern] % 3) *
                             powForNewSymbol;
        }
    }
    return numberOfMatches;
}

size_t CountStringLength(FILE *inputFile, char *line, size_t sizeline) {
    size_t readed = 0;
    while (1) {
        size_t bytesToRead = sizeline - readed;
        if (bytesToRead == 0) return readed; 
        size_t newReaded =
            fread((void *)(line + readed), 1, bytesToRead, inputFile);
        readed += newReaded;
        if (newReaded < bytesToRead) break;
    }
    return readed;
}

int FindAllComparisons(size_t *oldComparisons, size_t *comparisons,
                       char *pattern, char *string, size_t lengthOfPattern,
                       const int *powersOfThree, FILE *inputFile) {
    int i = 0;
    size_t oldNumberOfComparisons = 0;
    while (1) {
        if (i != 0) {
            memcpy(string, string + PATTERN_LENGTH, PATTERN_LENGTH);
        }
        size_t lengthOfString = CountStringLength(
            inputFile, string + PATTERN_LENGTH, PATTERN_LENGTH);

        if (lengthOfString == 0) return 0;

        if (i != 0) lengthOfString += PATTERN_LENGTH;

        size_t numberOfComparisons;
        if (i == 0) {
            numberOfComparisons = RabinCarpMatcher(
                pattern, string + PATTERN_LENGTH, lengthOfString,
                lengthOfPattern, comparisons, powersOfThree, i);
        } else {
            numberOfComparisons = RabinCarpMatcher(
                pattern, string, lengthOfString, lengthOfPattern, comparisons,
                powersOfThree, i);
        }

        for (size_t i = 0; i < numberOfComparisons; ++i) {
            char bad = 0;
            for (size_t j = 0; j < oldNumberOfComparisons; ++j) {
                if ((oldComparisons[j]) == (comparisons[i])) {
                    bad = 1;
                    break;
                }
            }
            if (!bad) printf("%zu ", comparisons[i]);
        }

        memcpy(oldComparisons, comparisons, PATTERN_LENGTH * COEFF_FOR_RING_BUFFER);
        oldNumberOfComparisons = numberOfComparisons;
        if (((lengthOfString < PATTERN_LENGTH) && (i == 0)) ||
            ((lengthOfString < COEFF_FOR_RING_BUFFER * PATTERN_LENGTH) && (i != 0))) {
            break;
        }
        ++i;
    }
    return 0;
}

int main(void) {
    int powersOfThree[PATTERN_LENGTH] = {1};
    for (int i = 1; i < PATTERN_LENGTH; ++i) {
        powersOfThree[i] = powersOfThree[i - 1] * 3;
    }

    FILE *const inputFile = fopen("in.txt", "r");
    if (inputFile == NULL) {
        return 0;
    }

    char pattern[PATTERN_LENGTH] = {0};
    if (fgets(pattern, PATTERN_LENGTH, inputFile) == NULL) {
        fclose(inputFile);
        return 0;
    }

    size_t lengthOfPattern = strlen(pattern);

    if (pattern[lengthOfPattern - 1] == '\n') {
        pattern[lengthOfPattern - 1] = 0;
        lengthOfPattern--;
    }
    if (lengthOfPattern > 16) {
        fclose(inputFile);
        return 0;
    }

    unsigned int hashPattern =
        CountHash(pattern, lengthOfPattern, powersOfThree);
    printf("%u ", hashPattern);

    char string[COEFF_FOR_RING_BUFFER * PATTERN_LENGTH];
    size_t comparisons[COEFF_FOR_RING_BUFFER * PATTERN_LENGTH];
    size_t oldComparisons[COEFF_FOR_RING_BUFFER * PATTERN_LENGTH];

    FindAllComparisons(oldComparisons, comparisons, pattern, string,
                       lengthOfPattern, powersOfThree, inputFile);

    fclose(inputFile);
}
