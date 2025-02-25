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



int compute3(int n){
    int result0, result1, result2, result3;
    result0 = fibonacci(n);
    result1 = odd_sum(result0);
    result2 = even_sum(n);
    result3 = fibonacci(result2);
    return result3-result1;
}
