
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
    int i = low;
    int j = high;

    while (i < j)
    {
        while (arr[i] <= pivot && i <= high - 1)
        {
            i++;
        }

        while (arr[j] > pivot && j >= low + 1)
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

// QuickSort function
void quickSort(int arr[64], int low, int high) // Assuming a max of 64 elements for the array
{
    int stack[128]; // Fixed size stack to work with HLS tools
    int top = -1;

    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0)
    {
        high = stack[top--];
        low = stack[top--];

        if (low < high)
        {
            int partitionIndex = partition(arr, low, high);

            stack[++top] = low;
            stack[++top] = partitionIndex - 1;

            stack[++top] = partitionIndex + 1;
            stack[++top] = high;
        }
    }
}

// Driver code
int main()
{
    int arr[] = {19, 17, 15, 12, 16, 18, 4, 11, 13};
    int n = 9;

    printf("Original array: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    quickSort(arr, 0, n - 1);

    printf("\nSorted array: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}
