#include <stdio.h>

int odd_factorial(int n){
    int res = 1 ;
    for (int i = 1; i < n; i+=2){
        res = res * i;
    }
    return res;
}

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


int compute4(int n){
    int result0, result1, result2, result3;
    result0 = odd_factorial(n);
    result1 = fibonacci(n);
    result2 = even_sum(result0-result1);
    result3 = even_sum(result2-result1);
    return result3;
}
