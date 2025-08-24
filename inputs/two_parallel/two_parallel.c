#include <stdio.h>

int odd_factorial(int n){
    int res = 1 ;
    for (int i = 1; i < n; i+=2){
        res = res * i;
    }
    return res;
}

int even_sum(int n){
    int sum = 0;
    for (int i = 2; i < n; i+=2){
        sum += i;
    }
    return sum;
}

int compute2(int n){
    int result1, result2;
    result1 = odd_factorial(n);
    result2 = even_sum(n);
    return result1 - result2;
}

