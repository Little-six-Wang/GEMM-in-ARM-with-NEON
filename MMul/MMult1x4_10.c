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

void AddDot1x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int p;
    double *b_p0_ptr, *b_p1_ptr, *b_p2_ptr, *b_p3_ptr;
    double* c_result = (double*)malloc(4 * sizeof(double));
    float64x2_t c_00_01_nreg, c_02_03_nreg;
    float64x2_t a_0p_nreg;
    float64x2_t b_p0_p1_nreg, b_p2_p3_nreg;
    c_00_01_nreg = vmovq_n_f64(0.0);
    c_02_03_nreg = vmovq_n_f64(0.0);
    b_p0_ptr = &B(0, 0);
    b_p1_ptr = &B(0, 1);
    b_p2_ptr = &B(0, 2);
    b_p3_ptr = &B(0, 3);
    for(p = 0; p < k; p += 4){
        a_0p_nreg = vld1q_dup_f64(&A(0, p));
        b_p0_p1_nreg = vld1q_lane_f64(b_p0_ptr++, b_p0_p1_nreg, 0);
        b_p0_p1_nreg = vld1q_lane_f64(b_p1_ptr++, b_p0_p1_nreg, 1);
        b_p2_p3_nreg = vld1q_lane_f64(b_p2_ptr++, b_p2_p3_nreg, 0);
        b_p2_p3_nreg = vld1q_lane_f64(b_p3_ptr++, b_p2_p3_nreg, 1);

        c_00_01_nreg = vfmaq_f64(c_00_01_nreg, a_0p_nreg, b_p0_p1_nreg);
        c_02_03_nreg = vfmaq_f64(c_02_03_nreg, a_0p_nreg, b_p2_p3_nreg);
    }
    vst1q_f64(c_result, c_00_01_nreg);
    vst1q_f64(c_result + 2, c_02_03_nreg);
    C(0, 0) = c_result[0];
    C(0, 1) = c_result[1];
    C(0, 2) = c_result[2];
    C(0, 3) = c_result[3];
    free(c_result);
}