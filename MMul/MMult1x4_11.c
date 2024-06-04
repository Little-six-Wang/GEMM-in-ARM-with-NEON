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

#include<arm_neon.h>
#include<stdlib.h>

typedef union{
    float64x2_t v;
    double d[2];
}v2df_t;


void AddDot1x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int p;
    double *b_p0_ptr, *b_p1_ptr, *b_p2_ptr, *b_p3_ptr;
    v2df_t c_00_01_nreg, c_02_03_nreg;
    v2df_t a_0p_nreg;
    v2df_t b_p0_p1_nreg, b_p2_p3_nreg;
    c_00_01_nreg.v = vmovq_n_f64(0.0);
    c_02_03_nreg.v = vmovq_n_f64(0.0);
    b_p0_ptr = &B(0, 0);
    b_p1_ptr = &B(0, 1);
    b_p2_ptr = &B(0, 2);
    b_p3_ptr = &B(0, 3);
    for(p = 0; p < k; p += 4){
        a_0p_nreg.v = vld1q_dup_f64(&A(0, p));
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p0_ptr, b_p0_p1_nreg.v, 0);
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p1_ptr, b_p0_p1_nreg.v, 1);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p2_ptr, b_p2_p3_nreg.v, 0);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p3_ptr, b_p2_p3_nreg.v, 1);
        c_00_01_nreg.v = vfmaq_f64(c_00_01_nreg.v, a_0p_nreg.v, b_p0_p1_nreg.v);
        c_02_03_nreg.v = vfmaq_f64(c_02_03_nreg.v, a_0p_nreg.v, b_p2_p3_nreg.v);

        a_0p_nreg.v = vld1q_dup_f64(&A(0, p + 1));
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p0_ptr + 1, b_p0_p1_nreg.v, 0);
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p1_ptr + 1, b_p0_p1_nreg.v, 1);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p2_ptr + 1, b_p2_p3_nreg.v, 0);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p3_ptr + 1, b_p2_p3_nreg.v, 1);
        c_00_01_nreg.v = vfmaq_f64(c_00_01_nreg.v, a_0p_nreg.v, b_p0_p1_nreg.v);
        c_02_03_nreg.v = vfmaq_f64(c_02_03_nreg.v, a_0p_nreg.v, b_p2_p3_nreg.v);

        a_0p_nreg.v = vld1q_dup_f64(&A(0, p + 2));
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p0_ptr + 2, b_p0_p1_nreg.v, 0);
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p1_ptr + 2, b_p0_p1_nreg.v, 1);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p2_ptr + 2, b_p2_p3_nreg.v, 0);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p3_ptr + 2, b_p2_p3_nreg.v, 1);
        c_00_01_nreg.v = vfmaq_f64(c_00_01_nreg.v, a_0p_nreg.v, b_p0_p1_nreg.v);
        c_02_03_nreg.v = vfmaq_f64(c_02_03_nreg.v, a_0p_nreg.v, b_p2_p3_nreg.v);

        a_0p_nreg.v = vld1q_dup_f64(&A(0, p + 3));
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p0_ptr + 3, b_p0_p1_nreg.v, 0);
        b_p0_p1_nreg.v = vld1q_lane_f64(b_p1_ptr + 3, b_p0_p1_nreg.v, 1);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p2_ptr + 3, b_p2_p3_nreg.v, 0);
        b_p2_p3_nreg.v = vld1q_lane_f64(b_p3_ptr + 3, b_p2_p3_nreg.v, 1);
        c_00_01_nreg.v = vfmaq_f64(c_00_01_nreg.v, a_0p_nreg.v, b_p0_p1_nreg.v);
        c_02_03_nreg.v = vfmaq_f64(c_02_03_nreg.v, a_0p_nreg.v, b_p2_p3_nreg.v);

        b_p0_ptr += 4;
        b_p1_ptr += 4;
        b_p2_ptr += 4;
        b_p3_ptr += 4;
    }
    C(0, 0) += c_00_01_nreg.d[0];
    C(0, 1) += c_00_01_nreg.d[1];
    C(0, 2) += c_02_03_nreg.d[0];
    C(0, 3) += c_02_03_nreg.d[1];
}