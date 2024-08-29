#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1svoid
CumulativeSums(int *is_random)
{
	int		S, sup, inf, k;
	S = 0;
	sup = 0;
	inf = 0;
	for ( k=0; k<N; k++ ) {
		epsilon[k] ? S++ : S--;
		if ( S > sup )
			sup++;
		if ( S < inf )
			inf--;
	}
	if (sup < 397 && inf > -397) *is_random =1;
	else *is_random =0;
}

int main(){
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int is_random;
    CumulativeSums(&is_random);

    printf("is_random = %d\n", is_random);
}