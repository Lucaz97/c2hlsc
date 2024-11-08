#include <stdio.h> 

// Partition function (unchanged)
int partition(int arr[9], int low, int high) {
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

// Optimized Iterative QuickSort function
void quickSort(int arr[9], int low, int high) {
    #pragma HLS INLINE off
    #define MAX_STACK_SIZE 32
    int stack[MAX_STACK_SIZE][2];
    int top = -1;

    // Push initial values of low and high to stack
    top++;
    stack[top][0] = low;
    stack[top][1] = high;

    // Keep popping from stack while is not empty
    while (top >= 0) {
        #pragma HLS PIPELINE off
        // Pop low and high
        high = stack[top][1];
        low = stack[top][0];
        top--;

        // Set pivot element at its correct position
        int pi = partition(arr, low, high);

        // If there are elements on left side of pivot,
        // then push left side to stack
        if (pi - 1 > low) {
            top++;
            stack[top][0] = low;
            stack[top][1] = pi - 1;
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (pi + 1 < high) {
            top++;
            stack[top][0] = pi + 1;
            stack[top][1] = high;
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
    quickSort(arr, 0, n - 1); 
  
    // printing the sorted array 
    printf("\nSorted array: "); 
    for (int i = 0; i < n; i++) { 
        printf("%d ", arr[i]); 
    } 
  
    return 0; 
}
