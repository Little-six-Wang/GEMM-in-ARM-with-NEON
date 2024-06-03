#include <stdlib.h>

#define A(i, j) a[(j) * lda + (i)]

void random_matrix( int m, int n, float *a, int lda ){
    int i,j;
    for ( j=0; j<n; j++ )
        for ( i=0; i<m; i++ )
            A( i,j ) = (float)rand()/(float)(RAND_MAX);
}
