#include <stdio.h> 

// Swap function
void swap(int a[9], int i, int j) {
    #pragma HLS INLINE
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

// Optimized Iterative QuickSort function
void quickSort(int arr[9], int n) {
    #pragma HLS PIPELINE II=1
    #pragma HLS ARRAY_PARTITION variable=arr complete dim=1

    const int MAX_STACK_SIZE = 64;
    int stack[MAX_STACK_SIZE];
    #pragma HLS ARRAY_PARTITION variable=stack complete dim=1
    int top = -1;
    int low = 0;
    int high = n - 1;

    // Push initial values of low and high to stack
    stack[++top] = low;
    stack[++top] = high;

    // Keep popping from stack while is not empty
    while (top >= 0) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=8
        // Pop high and low
        high = stack[top--];
        low = stack[top--];

        // Partition logic
        int pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            #pragma HLS LOOP_TRIPCOUNT min=1 max=8
            #pragma HLS PIPELINE II=1
            if (arr[j] < pivot) {
                i++;
                swap(arr, i, j);
            }
        }
        swap(arr, i + 1, high);
        int pi = i + 1;

        // If there are elements on left side of pivot,
        // then push left side to stack
        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
        }
    }
}

// Driver code
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
    quickSort(arr, n); 
  
    // printing the sorted array 
    printf("\nSorted array: "); 
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    return 0; 
}
