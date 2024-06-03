#define A(i, j) a[(j) * lda + (i)]
#define B(i, j) b[(j) * ldb + (i)]
#define C(i, j) c[(j) * ldc + (i)]

void MY_MMult(int m,  int n, int k, float* a, int lda, float* b, int ldb, float* c, int ldc){
    int i, j, p;
    for(j = 0; j < n; j++){
        for(i = 0; i < m; i++){
            for(p = 0; p < k; p++){
                C(i, j) += A(i, p) * B(p, j);
            }
        }
    }
}