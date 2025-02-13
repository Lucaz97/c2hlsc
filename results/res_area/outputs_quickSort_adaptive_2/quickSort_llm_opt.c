#include <stdio.h> 

// Function to partition the array
int partition(int arr[10], int low, int high) {
    int pivot = arr[high]; // Choosing the last element as pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
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

    return i + 1; // Return the partitioning index
}

void quickSort(int arr[10], int low, int high) {
    int stack[20]; // Adjusted stack size for HLS
    int top = -1;

    // Push initial values onto the stack
    stack[++top] = low;
    stack[++top] = high;

    // Continue until the stack is empty
    while (top >= 0) {
        // Pop high and low
        high = stack[top--];
        low = stack[top--];

        // Partition the array
        int partitionIndex = partition(arr, low, high);

        // If there are elements on the left side of the partition, push them onto the stack
        if (partitionIndex - 1 > low) {
            stack[++top] = low;
            stack[++top] = partitionIndex - 1;
        }

        // If there are elements on the right side of the partition, push them onto the stack
        if (partitionIndex + 1 < high) {
            stack[++top] = partitionIndex + 1;
            stack[++top] = high;
        }
    }
}

// Driver code
int main() 
{ 
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
