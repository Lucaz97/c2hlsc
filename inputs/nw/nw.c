

void reverse_string(char str[ALEN + BLEN], int length) {
    
    int start = 0;
    int end = length - 1;
    
    while (start < end) {
        
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        
        start++;
        end--;
    }
}

void fill_matrix(char seqA[ALEN], char seqB[BLEN], 
    int M[ALEN + 1][BLEN + 1], char ptr[ALEN + 1][BLEN + 1]) {
    
    int score, up_left, up, left, max;
    int row, row_up;
    int a_idx, b_idx;

    // initialize the first row and column of the matrix
    for (a_idx = 0; a_idx < ALEN + 1; a_idx++) {
        M[0][a_idx] = a_idx * GAP_SCORE;
    }
    for (b_idx = 0; b_idx < (BLEN + 1); b_idx++) {
        M[b_idx][0] = b_idx * GAP_SCORE;
    }
    
    // fill the matrix
    for (b_idx = 1; b_idx < BLEN + 1; b_idx++) {

        for (a_idx = 1; a_idx < ALEN + 1; a_idx++) {

            score = (seqA[a_idx - 1] == seqB[b_idx - 1]) ? MATCH_SCORE : MISMATCH_SCORE;

            row_up = b_idx - 1;
            row = b_idx;

            up_left = M[row_up][a_idx - 1] + score;
            up = M[row_up][a_idx] + GAP_SCORE;
            left = M[row][a_idx - 1] + GAP_SCORE;

            max = MAX(up_left, MAX(up, left));

            M[row][a_idx] = max;

            if (max == left) {
                ptr[row][a_idx] = SKIPB;
            } 
            else if (max == up) {
                ptr[row][a_idx] = SKIPA;
            } 
            else {
                ptr[row][a_idx] = ALIGN;
            }
        }
    }
}

void traceback(char seqA[ALEN], char seqB[BLEN], 
    char alignedA[ALEN + BLEN], char alignedB[ALEN + BLEN], 
    int M[ALEN + 1][BLEN + 1], char ptr[ALEN + 1][BLEN + 1]) {
    
    int a_idx = ALEN;
    int b_idx = BLEN;
    int a_str_idx = 0, b_str_idx = 0;
    int row;

    // traceback to build aligned sequences
    while (a_idx > 0 || b_idx > 0) {

        int r = b_idx;

        // align
        if (ptr[r][a_idx] == ALIGN) {

            alignedA[a_str_idx++] = seqA[a_idx-1];
            alignedB[b_str_idx++] = seqB[b_idx-1];

            a_idx--;
            b_idx--;
        }

        // skip B
        else if (ptr[r][a_idx] == SKIPB) {

            alignedA[a_str_idx++] = seqA[a_idx-1];
            alignedB[b_str_idx++] = '-';

            a_idx--;
        }

        // skip A
        else { 
            
            alignedA[a_str_idx++] = '-';
            alignedB[b_str_idx++] = seqB[b_idx-1];

            b_idx--;
        }
    }

    // reverse the aligned sequences (since traceback builds them backwards)
    reverse_string(alignedA, a_str_idx);
    reverse_string(alignedB, b_str_idx);
}

void needwun(char seqA[ALEN], char seqB[BLEN], 
    char alignedA[ALEN + BLEN], char alignedB[ALEN + BLEN], 
    int M[ALEN + 1][BLEN + 1], char ptr[ALEN + 1][BLEN + 1]) {
    
    // fill scoring matrix and traceback pointers
    fill_matrix(seqA, seqB, M, ptr);

    // perform traceback to generate aligned sequences
    traceback(seqA, seqB, alignedA, alignedB, M, ptr);
}

