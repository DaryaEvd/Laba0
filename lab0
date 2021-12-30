#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 50

int GetLenghtOfLeftPart(const char* input, int lenghtOfNumber) {
  const char* dotIndex = strchr(input, '.');
  if (dotIndex != NULL) return dotIndex - input;
  return lenghtOfNumber;
}

int CharToInt(char digit) {
  if (digit >= '0' && digit <= '9')
    return digit - '0';
  else if (digit >= 'a' && digit <= 'f')
    return digit - 'a' + 10;
  else if (digit >= 'A' && digit <= 'F')
    return digit - 'A' + 10;
  return -1;
}

long long int ConvertLeftPartToDec(const char numb[], double baseFrom,
                                   int lengthOfLeftPart) {
  long long int partLeftInDec = 0;
  for (int i = 0; i < lengthOfLeftPart; ++i) {
    int charNumber = CharToInt(numb[i]);
    partLeftInDec += charNumber * pow(baseFrom, lengthOfLeftPart - i - 1);
  }
  return partLeftInDec;
}

double ConvertRightPartToDec(const char numb[], double baseFrom,
                             int lengthOfRightPart) {
  double partRightInDec = 0;
  for (int i = 0; i < lengthOfRightPart; ++i) {
    int charNumber = CharToInt(numb[i]);
    partRightInDec += charNumber * pow(baseFrom, -1 - i);
  }
  return partRightInDec;
}

char IntToChar(int digit) { return digit > 9 ? digit % 10 + 'a' : digit + '0'; }

char* ConvertLeftPartToBaseTo(long long int num, int baseTo,
                              char* convertationLeft) {
  int rank = 0;
  long long int bufferNumber = num;
  while (bufferNumber > 0) {
    bufferNumber /= baseTo;
    rank++;
  }

  char* convertedLeftPartToBaseTo = convertationLeft;

  if (rank == 0) {
    convertedLeftPartToBaseTo[0] = '0';
    convertedLeftPartToBaseTo[1] = '\0';
    return convertedLeftPartToBaseTo;
  }

  bufferNumber = num;
  for (int i = rank - 1; i >= 0; --i) {
    convertedLeftPartToBaseTo[i] = IntToChar(bufferNumber % baseTo);
    bufferNumber /= baseTo;
  }

  convertedLeftPartToBaseTo[rank] = '\0';
  return convertedLeftPartToBaseTo;
}

char* ConvertRightPartToBaseTo(double num, int baseTo, char* convRight) {
  int maxNumbersInFactorialPart = 12;

  char* convertedRightPartToBaseTo = convRight;

  int i;
  for (i = 0; i < maxNumbersInFactorialPart; i++) {
    convertedRightPartToBaseTo[i] = IntToChar((int)(num * baseTo));
    if ((num * baseTo - (int)(num * baseTo)) == 0) {
      ++i;
      break;
    }
    num *= baseTo;
    num -= (int)num;
  }
  convertedRightPartToBaseTo[i] = '\0';
  return convertedRightPartToBaseTo;
}

int IsWrongBase(int base) {
  if (base < 2 || base > 16) return 1;
  return 0;
}

int BadInput(char number[], int baseFrom, int baseTo, int length) {
  if (IsWrongBase(baseFrom) || IsWrongBase(baseTo)) return 1;
  int flagDot = 0;

  for (int i = 0; i < length; ++i) {
    if (number[i] == '.') {
      if (flagDot || i == 0 || i == length - 1) return 1;
      flagDot = 1;
      continue;
    }
    if (CharToInt(number[i]) == -1) return 1;
    if (CharToInt(number[i]) >= baseFrom) return 1;
  }
  return 0;
}

char* ToResult(const char* num, int length, int baseFrom, int baseTo,
               char* convertationRight, char* convertationLeft) {
  int left = GetLenghtOfLeftPart(num, length);
  long long int decLeft = ConvertLeftPartToDec(num, baseFrom, left);
  int right = length - left - 1;
  char* result;
  result = ConvertLeftPartToBaseTo(decLeft, baseTo, convertationLeft);

  if (right > 0) {
    double decRight = ConvertRightPartToDec(num + left + 1, baseFrom, right);
    char stringToAppend[] = ".";
    strcat(result, stringToAppend);
    char* rightBuffer =
        ConvertRightPartToBaseTo(decRight, baseTo, convertationRight);
    strcat(result, rightBuffer);
  }
  return result;
}

int Free (char* convertation){
  if (convertation == NULL) return 0;
  free (convertation);
  return 1;
}

int main(void) {
  int baseFrom, baseTo;
  if (!scanf("%d%d", &baseFrom, &baseTo)) return 1;
  char input[100];
  if (!scanf("%99s", input)) return 1;

  char* convertationLeft = (char*)malloc(sizeof(char) * MAXLEN + 1);
  char* convertationRight = (char*)malloc(sizeof(char) * MAXLEN + 1);  

  int lengthOfNumber = (int)(strlen(input));
  if (BadInput(input, baseFrom, baseTo, lengthOfNumber)) {
    printf("bad input");
  } else {
    char* result = ToResult(input, lengthOfNumber, baseFrom, baseTo,
                            convertationRight, convertationLeft);
    printf("%s", result);
  }

  Free(convertationLeft);
  Free(convertationRight);
}
