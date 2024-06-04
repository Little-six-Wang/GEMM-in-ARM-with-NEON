#define A(i, j) a[(j) * lda + (i)]
#define B(i, j) b[(j) * ldb + (i)]
#define C(i, j) c[(j) * ldc + (i)]

void AddDot1x4(int, double*, int, double*, int, double*, int);

void MY_MMult(int m,  int n, int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int i, j;
    for(j = 0; j < n; j += 4){
        for(i = 0; i < m; i++){
            AddDot1x4(k, &A(i, 0), lda, &B(0, j), ldb, &C(i, j), ldc);
        }
    }
}

void AddDot1x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int p;
    register double c_00_reg, c_01_reg, c_02_reg, c_03_reg, a_0p_reg;
    double *b_p0_ptr, *b_p1_ptr, *b_p2_ptr, *b_p3_ptr;
    b_p0_ptr = &B(0, 0);
    b_p1_ptr = &B(0, 1);
    b_p2_ptr = &B(0, 2);
    b_p3_ptr = &B(0, 3);
    c_00_reg = 0.0;
    c_01_reg = 0.0;
    c_02_reg = 0.0;
    c_03_reg = 0.0;
    for(p = 0; p < k; p += 4){
        a_0p_reg = A(0, p);
        c_00_reg += a_0p_reg * *(b_p0_ptr);
        c_01_reg += a_0p_reg * *(b_p1_ptr);
        c_02_reg += a_0p_reg * *(b_p2_ptr);
        c_03_reg += a_0p_reg * *(b_p3_ptr);
        a_0p_reg = A(0, p + 1);
        c_00_reg += a_0p_reg * *(b_p0_ptr + 1);
        c_01_reg += a_0p_reg * *(b_p1_ptr + 1);
        c_02_reg += a_0p_reg * *(b_p2_ptr + 1);
        c_03_reg += a_0p_reg * *(b_p3_ptr + 1);
        a_0p_reg = A(0, p + 2);
        c_00_reg += a_0p_reg * *(b_p0_ptr + 2);
        c_01_reg += a_0p_reg * *(b_p1_ptr + 2);
        c_02_reg += a_0p_reg * *(b_p2_ptr + 2);
        c_03_reg += a_0p_reg * *(b_p3_ptr + 2);
        a_0p_reg = A(0, p + 3);
        c_00_reg += a_0p_reg * *(b_p0_ptr + 3);
        c_01_reg += a_0p_reg * *(b_p1_ptr + 3);
        c_02_reg += a_0p_reg * *(b_p2_ptr + 3);
        c_03_reg += a_0p_reg * *(b_p3_ptr + 3);
        b_p0_ptr += 4;
        b_p1_ptr += 4;
        b_p2_ptr += 4;
        b_p3_ptr += 4;
    }
    C(0, 0) += c_00_reg;
    C(0, 1) += c_01_reg;
    C(0, 2) += c_02_reg;
    C(0, 3) += c_03_reg;
}