
void Frequency(int * result)
{
        int             i, sum;

        sum = 0;
        for ( i=0; i<N; i++ )
                sum += 2*(int)epsilon[i]-1;
        
        *result = sum;

}
