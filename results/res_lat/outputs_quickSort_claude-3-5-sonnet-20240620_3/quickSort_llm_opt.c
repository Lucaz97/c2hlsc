#include <stdio.h> 

int partition(int arr[9], int low, int high) {
    #pragma hls_pipeline_init_interval 1
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    int t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    return (i + 1);
}

void quickSort(int arr[9], int low, int high) {
    #pragma hls_pipeline_init_interval 1
    const int MAX_LEVELS = 32;  // This should be enough for arrays up to 2^32 elements
    int stack_low[MAX_LEVELS];
    int stack_high[MAX_LEVELS];
    int top = 0;

    // Push initial values of low and high to stack
    stack_low[top] = low;
    stack_high[top] = high;
    top++;

    // Keep popping from stack while is not empty
    while (top > 0) {
        #pragma hls_unroll yes
        // Pop low and high
        top--;
        low = stack_low[top];
        high = stack_high[top];

        // Set pivot element at its correct position
        int partitionIndex = partition(arr, low, high);

        // If there are elements on left side of pivot,
        // then push left side to stack
        if (partitionIndex - 1 > low) {
            stack_low[top] = low;
            stack_high[top] = partitionIndex - 1;
            top++;
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (partitionIndex + 1 < high) {
            stack_low[top] = partitionIndex + 1;
            stack_high[top] = high;
            top++;
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
