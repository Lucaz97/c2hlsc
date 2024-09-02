

#include <stdio.h>

int sum(int *a, int b) {
    int s = 0;
    for (int i = 0; i < b; i++) {
        s += a[i];
    }
    return s;
}

int main(int argc, char *argv[]) {
    int a[5] = {5,4,3,2,1};
    int b = 5;   
    printf("Sum %d\n", sum(a, b));
    return 0;
}