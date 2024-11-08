#include <stdio.h> 

void quickSort(int arr[9], int low, int high) {
    #pragma HLS ARRAY_PARTITION variable=arr complete dim=1
    #define MAX_LEVELS 32
    int stack_low[MAX_LEVELS];
    int stack_high[MAX_LEVELS];
    int top = 0;

    // Push initial values of low and high to stack
    stack_low[top] = low;
    stack_high[top] = high;
    top++;

    // Keep popping from stack while is not empty
    while (top > 0) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=32
        #pragma HLS PIPELINE off

        // Pop low and high
        top--;
        int current_low = stack_low[top];
        int current_high = stack_high[top];

        // Partition the array
        int pivot = arr[current_high];
        int i = (current_low - 1);

        for (int j = current_low; j <= current_high - 1; j++) {
            #pragma HLS LOOP_TRIPCOUNT min=1 max=9
            #pragma HLS PIPELINE off
            if (arr[j] < pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[current_high];
        arr[current_high] = temp;
        int partitionIndex = i + 1;

        // If there are elements on left side of pivot,
        // then push left side to stack
        if (partitionIndex - 1 > current_low) {
            stack_low[top] = current_low;
            stack_high[top] = partitionIndex - 1;
            top++;
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (partitionIndex + 1 < current_high) {
            stack_low[top] = partitionIndex + 1;
            stack_high[top] = current_high;
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
