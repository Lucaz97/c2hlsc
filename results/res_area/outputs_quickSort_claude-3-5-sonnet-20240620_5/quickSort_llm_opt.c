#include <stdio.h> 

void quickSort(int arr[9], int low, int high) {
    #pragma HLS ARRAY_PARTITION variable=arr complete dim=1
    
    struct {
        int low;
        int high;
    } stack[100];  // Using a fixed size of 100
    #pragma HLS ARRAY_PARTITION variable=stack complete dim=1
    
    int top = -1;

    // Push initial values of low and high to stack
    top++;
    stack[top].low = low;
    stack[top].high = high;

    // Keep popping from stack while is not empty
    while (top >= 0) {
        #pragma HLS PIPELINE off
        
        // Pop low and high
        int current_low = stack[top].low;
        int current_high = stack[top].high;
        top--;

        // Partition logic
        int pivot = arr[current_high];
        int i = (current_low - 1);

        for (int j = current_low; j <= current_high - 1; j++) {
            #pragma HLS LOOP_FLATTEN off
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
            top++;
            stack[top].low = current_low;
            stack[top].high = partitionIndex - 1;
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (partitionIndex + 1 < current_high) {
            top++;
            stack[top].low = partitionIndex + 1;
            stack[top].high = current_high;
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
