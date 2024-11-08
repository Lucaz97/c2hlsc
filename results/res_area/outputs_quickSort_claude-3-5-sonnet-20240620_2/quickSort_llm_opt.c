#include <stdio.h> 

// Iterative QuickSort function
void quickSort(int arr[9], int low, int high) {
    #pragma HLS INLINE off
    // Structure to represent a range
    typedef struct {
        int low;
        int high;
    } Range;

    Range stack[9];  // Using a fixed size for the stack
    int top = -1;

    // Push initial values of low and high to stack
    stack[++top] = (Range){low, high};

    // Keep popping from stack while is not empty
    while (top >= 0) {
        #pragma HLS PIPELINE off
        // Pop low and high
        Range range = stack[top--];
        low = range.low;
        high = range.high;

        // Partition the array
        int pivot = arr[high];
        int i = (low - 1);

        #pragma HLS LOOP_MERGE
        for (int j = low; j <= high - 1; j++) {
            #pragma HLS PIPELINE off
            if (arr[j] < pivot) {
                i++;
                // Swap arr[i] and arr[j]
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        // Swap arr[i + 1] and arr[high]
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;

        // If there are elements on left side of pivot,
        // then push left side to stack
        if (pi - 1 > low) {
            stack[++top] = (Range){low, pi - 1};
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (pi + 1 < high) {
            stack[++top] = (Range){pi + 1, high};
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
