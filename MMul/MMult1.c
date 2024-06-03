#define A(i, j) a[(j) * lda + (i)]
#define B(i, j) b[(j) * ldb + (i)]
#define C(i, j) c[(j) * ldc + (i)]

void AddDot(int, float*, int, float*, float*);

void MY_MMult(int m,  int n, int k, float* a, int lda, float* b, int ldb, float* c, int ldc){
    int i, j;
    for(j = 0; j < n; j++){
        for(i = 0; i < m; i++){
            AddDot(k, &A(i, 0), lda, &B(0, j), &C(i, j));
        }
    }
}

#define X(i) x[(i) * incx]

void AddDot(int k, float* x, int incx, float* y, float* gamma){
    int p;
    for(p = 0; p < k; p++){
        *gamma += X(p) * y[p];
    }
}