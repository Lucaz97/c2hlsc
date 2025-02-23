#include <stdio.h>

int fibonacci(int n){
    int a = 0;
    int b = 1;
    int c;
    for (int i = 2; i < n; i++){
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

int even_sum(int n){
    int sum = 0;
    for (int i = 2; i < n; i+=2){
        sum += i;
    }
    return sum;
}
int odd_sum(int n){
    int sum = 0;
    for (int i = 1; i < n; i+=2){
        sum += i;
    }
    return sum;
}



void compute(int n[5]){
    int result0, result1, result2, result3;
    for(int i = 0; i < 5; i++){
        result0 = fibonacci(n[i]);
        printf("result0 = %d\n", result0);
        result1 = odd_sum(result0);
        printf("result1 = %d\n", result1);
        result2 = even_sum(n[i]);
        printf("result2 = %d\n", result2);
        result3 = fibonacci(result2);
        printf("result3 = %d\n", result3);
        n[i] = result3-result1;
    }
}

