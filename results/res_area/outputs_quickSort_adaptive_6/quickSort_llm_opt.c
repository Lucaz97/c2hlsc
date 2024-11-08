#include <stdio.h> 

#define MAX_STACK_SIZE 100

typedef struct {
    int low;
    int high;
} StackFrame;

// Function to partition the array
int partition(int arr[100], int low, int high) {
    int pivot = arr[high]; // Choosing the last element as pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++; // Increment index of smaller element
            // Swap arr[i] and arr[j]
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // Swap arr[i + 1] and arr[high] (or pivot)
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// Iterative quickSort function
void quickSort(int arr[100], int low, int high) {
    StackFrame stack[MAX_STACK_SIZE];
    int top = -1;

    // Push initial values onto the stack
    stack[++top] = (StackFrame){low, high};

    while (top >= 0) {
        // Pop the top values
        StackFrame frame = stack[top--];
        low = frame.low;
        high = frame.high;

        if (low < high) {
            int partitionIndex = partition(arr, low, high);

            // Push right side to stack
            stack[++top] = (StackFrame){partitionIndex + 1, high};
            // Push left side to stack
            stack[++top] = (StackFrame){low, partitionIndex - 1};
        }
    }
}

// Driver code
int main() {
    int arr[] = { 19, 17, 15, 12, 16, 18, 4, 11, 13 };
    int n = sizeof(arr) / sizeof(arr[0]);

    // Printing the original array
    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    // Calling quickSort() to sort the given array
    quickSort(arr, 0, n - 1);

    // Printing the sorted array
    printf("\nSorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}
