#include <stdio.h> 

// Function to swap two elements
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Partition function
int partition(int arr[9], int low, int high) {
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

// Iterative QuickSort function
void quickSort(int arr[9], int n) {
    #pragma HLS inline off
    // Structure to represent a range
    typedef struct {
        int low;
        int high;
    } Range;

    Range stack[9];  // Use a fixed size for the stack equal to the array size
    int top = -1;
    int low = 0;
    int high = n - 1;

    // Push initial values of low and high to stack
    top++;
    stack[top].low = low;
    stack[top].high = high;

    // Keep popping from stack while is not empty
    while (top >= 0) {
        #pragma HLS pipeline off
        // Pop low and high
        low = stack[top].low;
        high = stack[top].high;
        top--;

        // Set pivot element at its correct position
        int pi = partition(arr, low, high);

        // If there are elements on left side of pivot,
        // then push left side to stack
        if (pi - 1 > low) {
            top++;
            stack[top].low = low;
            stack[top].high = pi - 1;
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (pi + 1 < high) {
            top++;
            stack[top].low = pi + 1;
            stack[top].high = high;
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
