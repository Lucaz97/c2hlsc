
void Frequency(int *is_random)
{
        int             i;
        double  sum;

        sum = 0.0;
        for ( i=0; i<N; i++ )
                sum += 2*(int)epsilon[i]-1;
        
        is_random = sum >= -29 && sum <= 29;

}
