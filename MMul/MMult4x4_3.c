#define A(i, j) a[(j) * lda + (i)]
#define B(i, j) b[(j) * ldb + (i)]
#define C(i, j) c[(j) * ldc + (i)]

void AddDot4x4(int, double*, int, double*, int, double*, int);
void AddDot(int, double*, int, double*, double*);

void MY_MMult(int m, int n, int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int i, j;
    for(j = 0; j < n; j += 4){
        for(i = 0; i < m; i += 4){
            AddDot4x4(k, &A(i, 0), lda, &B(0, j), ldb, &C(i, j), ldc);
        }
    }
}

void AddDot4x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
        AddDot(k, &A(0, 0), lda, &B(0, 0), &C(0, 0));
        AddDot(k, &A(0, 0), lda, &B(0, 1), &C(0, 1));
        AddDot(k, &A(0, 0), lda, &B(0, 2), &C(0, 2));
        AddDot(k, &A(0, 0), lda, &B(0, 3), &C(0, 3));

        AddDot(k, &A(1, 0), lda, &B(0, 0), &C(1, 0));
        AddDot(k, &A(1, 0), lda, &B(0, 1), &C(1, 1));
        AddDot(k, &A(1, 0), lda, &B(0, 2), &C(1, 2));
        AddDot(k, &A(1, 0), lda, &B(0, 3), &C(1, 3));

        AddDot(k, &A(2, 0), lda, &B(0, 0), &C(2, 0));
        AddDot(k, &A(2, 0), lda, &B(0, 1), &C(2, 1));
        AddDot(k, &A(2, 0), lda, &B(0, 2), &C(2, 2));
        AddDot(k, &A(2, 0), lda, &B(0, 3), &C(2, 3));

        AddDot(k, &A(3, 0), lda, &B(0, 0), &C(3, 0));
        AddDot(k, &A(3, 0), lda, &B(0, 1), &C(3, 1));
        AddDot(k, &A(3, 0), lda, &B(0, 2), &C(3, 2));
        AddDot(k, &A(3, 0), lda, &B(0, 3), &C(3, 3));
}

#define X(i) x[(i) * incx]

void AddDot(int k, double* x, int incx, double* y, double* gamma){
    int p;
    for(p = 0; p < k; p++){
        *gamma += X(p) * y[p];
    }
}