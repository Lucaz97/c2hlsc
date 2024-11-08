#include <stdio.h> 

int partition(int arr[9], int low, int high) {
    #pragma hls_pipeline_init_interval 1
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quickSort(int arr[9], int low, int high) {
    #define MAX_LEVELS 9  // Maximum number of levels for a 9-element array
    int stack_low[MAX_LEVELS];
    int stack_high[MAX_LEVELS];
    int top = 0;

    stack_low[top] = low;
    stack_high[top] = high;

    #pragma hls_pipeline_init_interval 1
    while (top >= 0) {
        low = stack_low[top];
        high = stack_high[top];
        top--;

        int partitionIndex = partition(arr, low, high);

        if (partitionIndex - 1 > low) {
            top++;
            stack_low[top] = low;
            stack_high[top] = partitionIndex - 1;
        }

        if (partitionIndex + 1 < high) {
            top++;
            stack_low[top] = partitionIndex + 1;
            stack_high[top] = high;
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
    #pragma hls_unroll yes
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    // calling quickSort() to sort the given array 
    quickSort(arr, 0, n - 1); 
  
    // printing the sorted array 
    printf("\nSorted array: "); 
    #pragma hls_unroll yes
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    return 0; 
}
