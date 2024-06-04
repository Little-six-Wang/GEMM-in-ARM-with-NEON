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
    int p;
    for(p = 0; p < k; p++){
        C(0, 0) += A(0, p) * B(p, 0);
    }
    for(p = 0; p < k; p++){
        C(0, 1) += A(0, p) * B(p, 1);
    }
    for(p = 0; p < k; p++){
        C(0, 2) += A(0, p) * B(p, 2);
    }
    for(p = 0; p < k; p++){
        C(0, 3) += A(0, p) * B(p, 3);
    }

    for(p = 0; p < k; p++){
        C(1, 0) += A(1, p) * B(p, 0);
    }
    for(p = 0; p < k; p++){
        C(1, 1) += A(1, p) * B(p, 1);
    }
    for(p = 0; p < k; p++){
        C(1, 2) += A(1, p) * B(p, 2);
    }
    for(p = 0; p < k; p++){
        C(1, 3) += A(1, p) * B(p, 3);
    }

    for(p = 0; p < k; p++){
        C(2, 0) += A(2, p) * B(p, 0);
    }
    for(p = 0; p < k; p++){
        C(2, 1) += A(2, p) * B(p, 1);
    }
    for(p = 0; p < k; p++){
        C(2, 2) += A(2, p) * B(p, 2);
    }
    for(p = 0; p < k; p++){
        C(2, 3) += A(2, p) * B(p, 3);
    }

    for(p = 0; p < k; p++){
        C(3, 0) += A(3, p) * B(p, 0);
    }
    for(p = 0; p < k; p++){
        C(3, 1) += A(3, p) * B(p, 1);
    }
    for(p = 0; p < k; p++){
        C(3, 2) += A(3, p) * B(p, 2);
    }
    for(p = 0; p < k; p++){
        C(3, 3) += A(3, p) * B(p, 3);
    }
}