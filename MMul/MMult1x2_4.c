#define A(i, j) a[(j) * lda + (i)]
#define B(i, j) b[(j) * ldb + (i)]
#define C(i, j) c[(j) * ldc + (i)]

void AddDot1x2(int, float*, int, float*, int, float*, int);

void MY_MMult(int m,  int n, int k, float* a, int lda, float* b, int ldb, float* c, int ldc){
    int i, j;
    for(j = 0; j < n; j += 2){
        for(i = 0; i < m; i++){
            AddDot1x2(k, &A(i, 0), lda, &B(0, j), ldb, &C(i, j), ldc);
        }
    }
}

void AddDot1x2(int k, float* a, int lda, float* b, int ldb, float* c, int ldc){
    int p;
    for(p = 0; p < k; p++){
        C(0, 0) += A(0, p) * B(p, 0);
    }
    for(p = 0; p < k; p++){
        C(0, 1) += A(0, p) * B(p, 1);
    }
}