#include <stdio.h> 

#define MAX_ARRAY_SIZE 100
#define MAX_STACK_SIZE 100

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[MAX_ARRAY_SIZE], int low, int high) {
    #pragma HLS inline off
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        #pragma HLS pipeline off
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[MAX_ARRAY_SIZE], int low, int high) {
    #pragma HLS inline off
    typedef struct {
        int low;
        int high;
    } StackItem;

    StackItem stack[MAX_STACK_SIZE];
    int top = -1;

    stack[++top] = (StackItem){low, high};

    while (top >= 0) {
        #pragma HLS pipeline off
        StackItem current = stack[top--];
        low = current.low;
        high = current.high;

        if (low < high) {
            int partitionIndex = partition(arr, low, high);

            if (partitionIndex - 1 > low) {
                stack[++top] = (StackItem){low, partitionIndex - 1};
            }

            if (partitionIndex + 1 < high) {
                stack[++top] = (StackItem){partitionIndex + 1, high};
            }
        }
    }
}

// driver code 
int main() 
{ 
    int arr[] = { 19, 17, 15, 12, 16, 18, 4, 11, 13 }; 
    int n = sizeof(arr) / sizeof(arr[0]); 
  
    // printing the original array 
    printf("Original array: "); 
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    // calling quickSort() to sort the given array 
    quickSort(arr, 0, n - 1); 
  
    // printing the sorted array 
    printf("\nSorted array: "); 
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    return 0; 
}
