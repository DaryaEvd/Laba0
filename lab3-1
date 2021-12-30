#include <stdio.h>
#include <stdlib.h>

void Swap(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int Partition(int* arrayOfNumbers, int low, int hight) {
  int middleElement = arrayOfNumbers[(low + hight) / 2];
  int start = low;
  int end = hight;

  while (start <= end) {
    while (arrayOfNumbers[start] < middleElement) {
      start++;
    }
    while (arrayOfNumbers[end] > middleElement) {
      end--;
    }

    if (start >= end) {
      return end;
    }
    Swap(&arrayOfNumbers[start++], &arrayOfNumbers[end--]);
  }
  return end;
}

void QuickSort(int* arrayOfNumbers, int low, int hight) {
  if (low < hight) {
    int middleLocation = Partition(arrayOfNumbers, low, hight);
    QuickSort(arrayOfNumbers, low, middleLocation);
    QuickSort(arrayOfNumbers, middleLocation + 1, hight);
  }
}

int main(void) {
  int length;
  if (!scanf("%d", &length)) return 0;

  int* arrayOfNumbers = malloc(sizeof(int) * length);
  
  if(arrayOfNumbers == NULL){
    return 0;
  }

  for (int i = 0; i < length; ++i) {
    if (!scanf("%d", &arrayOfNumbers[i])){
      free(arrayOfNumbers);
      return 0;
    }
  }
  QuickSort(arrayOfNumbers, 0, length - 1);

  for (int i = 0; i < length; ++i) {
    printf("%d ", arrayOfNumbers[i]);
  }
  free(arrayOfNumbers); 
}
