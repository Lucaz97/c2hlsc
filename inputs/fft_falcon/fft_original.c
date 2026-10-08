void
falcon_FFT(fpr f[512], unsigned logn) {
    /*
     * FFT algorithm in bit-reversal order uses the following
     * iterative algorithm:
     *
     *   t = N
     *   for m = 1; m < N; m *= 2:
     *       ht = t/2
     *       for i1 = 0; i1 < m; i1 ++:
     *           j1 = i1 * t
     *           s = GM[m + i1]
     *           for j = j1; j < (j1 + ht); j ++:
     *               x = f[j]
     *               y = s * f[j + ht]
     *               f[j] = x + y
     *               f[j + ht] = x - y
     *       t = ht
     *
     * GM[k] contains w^rev(k) for primitive root w = exp(i*pi/N).
     *
     * In the description above, f[] is supposed to contain complex
     * numbers. In our in-memory representation, the real and
     * imaginary parts of f[k] are in array slots k and k+N/2.
     *
     * We only keep the first half of the complex numbers. We can
     * see that after the first iteration, the first and second halves
     * of the array of complex numbers have separate lives, so we
     * simply ignore the second part.
     */

    unsigned u;
    size_t t, n, hn, m;

    /*
     * First iteration: compute f[j] + i * f[j+N/2] for all j < N/2
     * (because GM[1] = w^rev(1) = w^(N/2) = i).
     * In our chosen representation, this is a no-op: everything is
     * already where it should be.
     */

    /*
     * Subsequent iterations are truncated to use only the first
     * half of values.
     */
    n = (size_t)1 << logn;
    hn = n >> 1;
    t = hn;
    for (u = 1, m = 2; u < logn; u ++, m <<= 1) {
        size_t ht, hm, i1, j1;

        ht = t >> 1;
        hm = m >> 1;
        for (i1 = 0, j1 = 0; i1 < hm; i1 ++, j1 += t) {
            size_t j, j2;

            j2 = j1 + ht;
            fpr s_re, s_im;

            s_re = fpr_gm_tab[((m + i1) << 1) + 0];
            s_im = fpr_gm_tab[((m + i1) << 1) + 1];
            for (j = j1; j < j2; j ++) {
                fpr x_re, x_im, y_re, y_im;

                x_re = f[j];
                x_im = f[j + hn];
                y_re = f[j + ht];
                y_im = f[j + ht + hn];
                FPC_MUL(y_re, y_im, y_re, y_im, s_re, s_im);
                FPC_ADD(f[j], f[j + hn],
                        x_re, x_im, y_re, y_im);
                FPC_SUB(f[j + ht], f[j + ht + hn],
                        x_re, x_im, y_re, y_im);
            }
        }
        t = ht;
    }
}