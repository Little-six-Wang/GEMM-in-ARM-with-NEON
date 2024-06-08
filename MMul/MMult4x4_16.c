#define A(i, j) a[(j) * lda + (i)]
#define B(i, j) b[(j) * ldb + (i)]
#define C(i, j) c[(j) * ldc + (i)]

#define mc 256
#define kc 128
#define nb 1000

#define min(a, b) ((a) < (b) ? (a) : (b))

void InnerKernel(int, int, int, double*, int, double*, int, double*, int, int);
void PackMatrixA(int, double*, int, double*);
void PackMatrixB(int, double*, int, double*);
void AddDot4x4(int, double*, int, double*, int, double*, int);

void MY_MMult(int m, int n, int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int ar, ac;
    int i, j;
    for(j = 0; j < k; j += kc){
        ar = min(kc, k - j);
        for(i = 0; i < m; i += mc){
            ac = min(mc, m - i);
            InnerKernel(ac, n, ar, &A(i, j), lda, &B(j, 0), ldb, &C(i, 0), ldc, i==0);  //mc x n
        }
    }
}

void InnerKernel(int m, int n, int k, double* a, int lda, double* b, int ldb, double* c, int ldc, int flag){
    int i, j;
    double packedA[m * k]; //mc * kc
    static double packedB[kc * nb];
    for(j = 0; j < n; j += 4){
        if(flag){
            PackMatrixB(k, &B(0, j), ldb, &packedB[j * k]);
        }
        for(i = 0; i < m; i += 4){
            if(j == 0){
                PackMatrixA(k, &A(i, 0), lda, &packedA[i * k]);
            }
            AddDot4x4(k, &packedA[i * k], 4, &packedB[j * k], 4, &C(i, j), ldc);
        }
    }
}

void PackMatrixA(int k, double* a, int lda, double* a_to){
    int j;
    double* a_ptr;
    for(j = 0; j < k; j++){
        a_ptr = &A(0, j);
        *a_to++ = *a_ptr;
        *a_to++ = *(a_ptr + 1);
        *a_to++ = *(a_ptr + 2);
        *a_to++ = *(a_ptr + 3);
        
    }
}

void PackMatrixB(int k, double* b, int ldb, double* b_to){
    int j;
    double *b_00_ptr, *b_01_ptr, * b_02_ptr, *b_03_ptr;
    b_00_ptr = &B(0, 0);
    b_01_ptr = &B(0, 1);
    b_02_ptr = &B(0, 2);
    b_03_ptr = &B(0, 3);
    for(j = 0; j < k; j++){
        *b_to++ = *b_00_ptr++;
        *b_to++ = *b_01_ptr++;
        *b_to++ = *b_02_ptr++;
        *b_to++ = *b_03_ptr++;
    }
}

#include<arm_neon.h>

void AddDot4x4(int k, double* a, int lda, double* b, int ldb, double* c, int ldc){
    int p;
    float64x2_t c_00_10_vreg, c_01_11_vreg, c_02_12_vreg, c_03_13_vreg;
    float64x2_t c_20_30_vreg, c_21_31_vreg, c_22_32_vreg, c_23_33_vreg;

    float64x2_t a_0p_1p_vreg, a_2p_3p_vreg;

    float64x2_t b_p0_vreg, b_p1_vreg, b_p2_vreg, b_p3_vreg;

    double *c_00_ptr, *c_01_ptr, *c_02_ptr, *c_03_ptr;

    c_00_ptr = &C(0, 0);
    c_01_ptr = &C(0, 1);
    c_02_ptr = &C(0, 2);
    c_03_ptr = &C(0, 3);

    //init
    c_00_10_vreg = vmovq_n_f64(0.0);
    c_01_11_vreg = vmovq_n_f64(0.0);
    c_02_12_vreg = vmovq_n_f64(0.0);
    c_03_13_vreg = vmovq_n_f64(0.0);
    c_20_30_vreg = vmovq_n_f64(0.0);
    c_21_31_vreg = vmovq_n_f64(0.0);
    c_22_32_vreg = vmovq_n_f64(0.0);
    c_23_33_vreg = vmovq_n_f64(0.0);

    for(p = 0; p < k; p++){
        a_0p_1p_vreg = vld1q_f64(&A(0, p));
        a_2p_3p_vreg = vld1q_f64(&A(2, p));

        b_p0_vreg = vld1q_dup_f64(&B(0, p));
        b_p1_vreg = vld1q_dup_f64(&B(1, p));
        b_p2_vreg = vld1q_dup_f64(&B(2, p));
        b_p3_vreg = vld1q_dup_f64(&B(3, p));

        c_00_10_vreg = vfmaq_f64(c_00_10_vreg, a_0p_1p_vreg, b_p0_vreg);
        c_01_11_vreg = vfmaq_f64(c_01_11_vreg, a_0p_1p_vreg, b_p1_vreg);
        c_02_12_vreg = vfmaq_f64(c_02_12_vreg, a_0p_1p_vreg, b_p2_vreg);
        c_03_13_vreg = vfmaq_f64(c_03_13_vreg, a_0p_1p_vreg, b_p3_vreg);

        c_20_30_vreg = vfmaq_f64(c_20_30_vreg, a_2p_3p_vreg, b_p0_vreg);
        c_21_31_vreg = vfmaq_f64(c_21_31_vreg, a_2p_3p_vreg, b_p1_vreg);
        c_22_32_vreg = vfmaq_f64(c_22_32_vreg, a_2p_3p_vreg, b_p2_vreg);
        c_23_33_vreg = vfmaq_f64(c_23_33_vreg, a_2p_3p_vreg, b_p3_vreg);
    }

    *c_00_ptr += vgetq_lane_f64(c_00_10_vreg, 0);
    *(c_00_ptr + 1) += vgetq_lane_f64(c_00_10_vreg, 1);
    *(c_00_ptr + 2) += vgetq_lane_f64(c_20_30_vreg, 0);
    *(c_00_ptr + 3) += vgetq_lane_f64(c_20_30_vreg, 1);

    *c_01_ptr += vgetq_lane_f64(c_01_11_vreg, 0);
    *(c_01_ptr + 1) += vgetq_lane_f64(c_01_11_vreg, 1);
    *(c_01_ptr + 2) += vgetq_lane_f64(c_21_31_vreg, 0);
    *(c_01_ptr + 3) += vgetq_lane_f64(c_21_31_vreg, 1);

    *c_02_ptr += vgetq_lane_f64(c_02_12_vreg, 0);
    *(c_02_ptr + 1) += vgetq_lane_f64(c_02_12_vreg, 1);
    *(c_02_ptr + 2) += vgetq_lane_f64(c_22_32_vreg, 0);
    *(c_02_ptr + 3) += vgetq_lane_f64(c_22_32_vreg, 1);

    *c_03_ptr += vgetq_lane_f64(c_03_13_vreg, 0);
    *(c_03_ptr + 1) += vgetq_lane_f64(c_03_13_vreg, 1);
    *(c_03_ptr + 2) += vgetq_lane_f64(c_23_33_vreg, 0);
    *(c_03_ptr + 3) += vgetq_lane_f64(c_23_33_vreg, 1);
}