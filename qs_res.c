
#include <stdio.h>

// Function to swap two elements
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function
int partition(int arr[], int low, int high)
{
    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while (i <= j)
    {
        while (i <= high && arr[i] <= pivot)
        {
            i++;
        }
        while (j > low && arr[j] > pivot)
        {
            j--;
        }
        if (i < j)
        {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[low], &arr[j]);
    return j;
}

// Iterative QuickSort function
void quickSort(int arr[1024], int low, int high)
{
    int stack[100]; // Assume a maximum stack size
    int top = -1;
    
    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0)
    {
        high = stack[top--];
        low = stack[top--];

        int pivotIndex = partition(arr, low, high);

        if (pivotIndex - 1 > low)
        {
            stack[++top] = low;
            stack[++top] = pivotIndex - 1;
        }

        if (pivotIndex + 1 < high)
        {
            stack[++top] = pivotIndex + 1;
            stack[++top] = high;
        }
    }
}

// Driver code
int main()
{
    int arr[] = {19, 17, 15, 12, 16, 18, 4, 11, 13};
    int n = sizeof(arr) / sizeof(arr[0]);

    // printing the original array
    printf("Original array: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    // calling quickSort() to sort the given array
    quickSort(arr, 0, n - 1);

    // printing the sorted array
    printf("\nSorted array: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}
