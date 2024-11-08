#include <stdio.h> 

// Partition function
int partition(int arr[200], int low, int high) {
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

// Iterative QuickSort function
void quickSort(int arr[200], int n) {
    int stack[200];  // Using a fixed size for the stack (2 * max number of elements)
    int top = -1;
    int low = 0;
    int high = n - 1;

    // Push initial values of low and high to stack
    stack[++top] = low;
    stack[++top] = high;

    // Keep popping from stack while is not empty
    #pragma hls_pipeline_init_interval 1
    while (top >= 0) {
        // Pop high and low
        high = stack[top--];
        low = stack[top--];

        // Set pivot element at its correct position
        int pi = partition(arr, low, high);

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
    int arr[200] = { 19, 17, 15, 12, 16, 18, 4, 11, 13 }; 
    int n = 9; 
  
    // printing the original array 
    printf("Original array: "); 
    #pragma hls_unroll 4
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    // calling quickSort() to sort the given array 
    quickSort(arr, n); 
  
    // printing the sorted array 
    printf("\nSorted array: "); 
    #pragma hls_unroll 4
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    return 0; 
}
