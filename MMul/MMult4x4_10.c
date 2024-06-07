#define A(i, j) a[(j) * lda + (i)]
#define B(i, j) b[(j) * ldb + (i)]
#define C(i, j) c[(j) * ldc + (i)]

void AddDot4x4(int, double*, int, double*, int, double*, int);

void MY_MMult(int m, int n, int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int i, j;
    for(j = 0; j < n; j += 4){
        for(i = 0; i < m; i += 4){
            AddDot4x4(k, &A(i, 0), lda, &B(0, j), ldb, &C(i, j), ldc);
        }
    }
}

#include<arm_neon.h>

typedef union{
    float64x2_t v;
    double d[2];
}v2df_t;

void AddDot4x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int p;
    v2df_t c_00_10_vreg, c_01_11_vreg, c_02_12_vreg, c_03_13_vreg;
    v2df_t c_20_30_vreg, c_21_31_vreg, c_22_32_vreg, c_23_33_vreg;

    v2df_t a_0p_1p_vreg, a_2p_3p_vreg;

    v2df_t b_p0_vreg, b_p1_vreg, b_p2_vreg, b_p3_vreg;

    double *b_p0_ptr, *b_p1_ptr, *b_p2_ptr, *b_p3_ptr;

    //init
    c_00_10_vreg.v = vmovq_n_f64(0.0);
    c_01_11_vreg.v = vmovq_n_f64(0.0);
    c_02_12_vreg.v = vmovq_n_f64(0.0);
    c_03_13_vreg.v = vmovq_n_f64(0.0);
    c_20_30_vreg.v = vmovq_n_f64(0.0);
    c_21_31_vreg.v = vmovq_n_f64(0.0);
    c_22_32_vreg.v = vmovq_n_f64(0.0);
    c_23_33_vreg.v = vmovq_n_f64(0.0);

    b_p0_ptr = &B(0, 0);
    b_p1_ptr = &B(0, 1);
    b_p2_ptr = &B(0, 2);
    b_p3_ptr = &B(0, 3);

    for(p = 0; p < k; p++){
        a_0p_1p_vreg.v = vld1q_f64(&A(0, p));
        a_2p_3p_vreg.v = vld1q_f64(&A(2, p));

        b_p0_vreg.v = vld1q_dup_f64(b_p0_ptr++);
        b_p1_vreg.v = vld1q_dup_f64(b_p1_ptr++);
        b_p2_vreg.v = vld1q_dup_f64(b_p2_ptr++);
        b_p3_vreg.v = vld1q_dup_f64(b_p3_ptr++);

        // c_00_10_vreg.v += a_0p_1p_vreg.v * b_p0_vreg.v;
        // c_01_11_vreg.v += a_0p_1p_vreg.v * b_p1_vreg.v;
        // c_02_12_vreg.v += a_0p_1p_vreg.v * b_p2_vreg.v;
        // c_03_13_vreg.v += a_0p_1p_vreg.v * b_p3_vreg.v;

        // c_20_30_vreg.v += a_2p_3p_vreg.v * b_p0_vreg.v;
        // c_21_31_vreg.v += a_2p_3p_vreg.v * b_p1_vreg.v;
        // c_22_32_vreg.v += a_2p_3p_vreg.v * b_p2_vreg.v;
        // c_23_33_vreg.v += a_2p_3p_vreg.v * b_p3_vreg.v;

        c_00_10_vreg.v = vfmaq_f64(c_00_10_vreg.v, a_0p_1p_vreg.v, b_p0_vreg.v);
        c_01_11_vreg.v = vfmaq_f64(c_01_11_vreg.v, a_0p_1p_vreg.v, b_p1_vreg.v);
        c_02_12_vreg.v = vfmaq_f64(c_02_12_vreg.v, a_0p_1p_vreg.v, b_p2_vreg.v);
        c_03_13_vreg.v = vfmaq_f64(c_03_13_vreg.v, a_0p_1p_vreg.v, b_p3_vreg.v);

        c_20_30_vreg.v = vfmaq_f64(c_20_30_vreg.v, a_2p_3p_vreg.v, b_p0_vreg.v);
        c_21_31_vreg.v = vfmaq_f64(c_21_31_vreg.v, a_2p_3p_vreg.v, b_p1_vreg.v);
        c_22_32_vreg.v = vfmaq_f64(c_22_32_vreg.v, a_2p_3p_vreg.v, b_p2_vreg.v);
        c_23_33_vreg.v = vfmaq_f64(c_23_33_vreg.v, a_2p_3p_vreg.v, b_p3_vreg.v);
    }
    C(0, 0) += c_00_10_vreg.d[0];
    C(0, 1) += c_01_11_vreg.d[0];
    C(0, 2) += c_02_12_vreg.d[0];
    C(0, 3) += c_03_13_vreg.d[0];

    C(1, 0) += c_00_10_vreg.d[1];
    C(1, 1) += c_01_11_vreg.d[1];
    C(1, 2) += c_02_12_vreg.d[1];
    C(1, 3) += c_03_13_vreg.d[1];

    C(2, 0) += c_20_30_vreg.d[0];
    C(2, 1) += c_21_31_vreg.d[0];
    C(2, 2) += c_22_32_vreg.d[0];
    C(2, 3) += c_23_33_vreg.d[0];

    C(3, 0) += c_20_30_vreg.d[1];
    C(3, 1) += c_21_31_vreg.d[1];
    C(3, 2) += c_22_32_vreg.d[1];
    C(3, 3) += c_23_33_vreg.d[1];
}