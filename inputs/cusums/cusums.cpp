void
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

