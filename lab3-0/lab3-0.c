#include <stdio.h>
#include <stdlib.h>

void Swap(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void MaxHeapify(int* array, int length, int index) {
  int largestElement = index;

  int leftElement = 2 * index;
  while (leftElement <= length && array[leftElement] > array[largestElement]) {
    largestElement = leftElement;
  }

  int rightElement = (2 * index) + 1;
  while (rightElement <= length &&
         array[rightElement] > array[largestElement]) {
    largestElement = rightElement;
  }

  if (largestElement != index) {
    Swap(&array[largestElement], &array[index]);
    MaxHeapify(array, length, largestElement);
  }
}

void HeapSort(int* array, int length) {
  for (int i = length / 2; i >= 1; --i) {
    MaxHeapify(array, length, i);
  }
  for (int j = length; j >= 1; --j) {
    Swap(&array[1], &array[j]);
    --length;
    MaxHeapify(array, length, 1);
  }
}

int main(void) {
  int length;
  if (!scanf("%d", &length)) return 0;

  int* array = malloc(sizeof(int) * (length + 1));
  if((array) == NULL){
    return 0;
  }
  
  for (int i = 1; i <= length; ++i) {
    if (!scanf("%d", &array[i])){
      free(array);
      return 0;
    }
  }

  HeapSort(array, length);

  for (int i = 1; i <= length; ++i) {
    printf("%d ", array[i]);
  }
  free(array);
}
