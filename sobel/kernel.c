#include "kernel.h"

//
i32 convolve_baseline(u8 *m, i32 *f, u64 fh, u64 fw)
{
  i32 r = 0;

  for (u64 i = 0; i < fh; i++)
    for (u64 j = 0; j < fw; j++)
      r += m[INDEX(i, j, W * 3)] * f[INDEX(i, j, fw)];
  
  return r;
}

//
void sobel_baseline(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx, gy;
  f32 mag = 0.0;

  i32 f1[9] = { -1, 0, 1,
                -2, 0, 2,
                -1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
                 0,  0,  0,
                 1,  2,  1 }; //3x3 matrix
  
  //
  for (u64 i = 0; i < (H - 3); i++) {
    for (u64 j = 0; j < ((W * 3) - 3); j++) {
      gx = convolve_baseline(&cframe[INDEX(i, j, W * 3)], f1, 3, 3);
      gy = convolve_baseline(&cframe[INDEX(i, j, W * 3)], f2, 3, 3);
          
      // mag = sqrt((gx * gx) + (gy * gy));

      if(gx < 0) {
        gx *= -1;
      }

      if(gy < 0) {
        gy *= -1;
      }

      mag = gx + gy;
      
      oframe[INDEX(i, j, W * 3)] = mag;
    }
  }
}

//
void sobel_unroll4(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx = 0, gy = 0;
  f32 mag = 0.0;

  i32 m = ((W * 3) - 3) - (((W * 3) - 3) & 3);
  
  //
  for (u64 i = 0; i < (H - 3); i++) {
    for (u64 j = 0; j < m; j+=4) {

      gx += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 , W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 , W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 , W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 , W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 , W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 , W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j , W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +1, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +1, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +1, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +1, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +1, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +1, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +1, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +1, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +1, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +1, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +1, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +1, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +1, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +2, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +2, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +2, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +2, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +2, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +2, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +2, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +2, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +2, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +2, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +2, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +2, W * 3)] *  1;
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i , j +2, W * 3)] = (mag > threshold) ? 255 : 0;
      
      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +3, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +3, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +3, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +3, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +3, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +3, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +3, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +3, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +3, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +3, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +3, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +3, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +3, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;
    }

    for (u64 j = m; j < ((W * 3) - 3); j++) {

      gx += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 , W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 , W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 , W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 , W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 , W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 , W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j , W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;
    }

  }
}

//
void sobel_unroll8(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx = 0, gy = 0;
  f32 mag = 0.0;
  threshold = 10000;

  i32 m = ((W * 3) - 3) - (((W * 3) - 3) & 7);
  
  //
  for (u64 i = 0; i < (H - 3); i++) {
    for (u64 j = 0; j < m; j+=8) {

      gx += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 , W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 , W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 , W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 , W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 , W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 , W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j , W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +1, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +1, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +1, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +1, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +1, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +1, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +1, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +1, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +1, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +1, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +1, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +1, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +1, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +2, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +2, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +2, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +2, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +2, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +2, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +2, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +2, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +2, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +2, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +2, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +2, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +2, W * 3)] = (mag > threshold) ? 255 : 0;
      
      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +3, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +3, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +3, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +3, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +3, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +3, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +3, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +3, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +3, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +3, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +3, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +3, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +3, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +4, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +4, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +4, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +4, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +4, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +4, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +4, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +4, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +4, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +4, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +4, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +4, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +4, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +5, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +5, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +5, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +5, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +5, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +5, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +5, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +5, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +5, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +5, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +5, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +5, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +5, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +6, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +6, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +6, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +6, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +6, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +6, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +6, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +6, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +6, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +6, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +6, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +6, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +6, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;

      //
      gx += cframe[INDEX(i+0 , j+0 +7, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 +7, W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 +7, W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 +7, W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 +7, W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 +7, W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 +7, W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 +7, W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 +7, W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 +7, W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 +7, W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 +7, W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j +7, W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;
    }

    for (u64 j = m; j < ((W * 3) - 3); j++) {

      gx += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+0 , W * 3)] *  1;
      gx += cframe[INDEX(i+0 , j+1 , W * 3)] * -2;
      gx += cframe[INDEX(i+2 , j+1 , W * 3)] *  2;
      gx += cframe[INDEX(i+0 , j+2 , W * 3)] * -1;
      gx += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;

      gy += cframe[INDEX(i+0 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+1 , j+0 , W * 3)] * -2;
      gy += cframe[INDEX(i+2 , j+0 , W * 3)] * -1;
      gy += cframe[INDEX(i+0 , j+2 , W * 3)] *  1;
      gy += cframe[INDEX(i+1 , j+2 , W * 3)] *  2;
      gy += cframe[INDEX(i+2 , j+2 , W * 3)] *  1;
          
      mag = (gx * gx) + (gy * gy);
      
      oframe[INDEX(i , j , W * 3)] = (mag > threshold) ? 255 : 0;

      gx = 0;
      gy = 0;
    }

  }
}

void sobel_unroll8_intrinsic_AVX2(f32 *cframe, f32 *oframe, f32 threshold) {
  
  __m256 v_1 = _mm256_set_ps( 1, 1, 1, 1, 1, 1, 1, 1);
  __m256 v_2 = _mm256_set_ps( 2, 2, 2, 2, 2, 2, 2, 2);
  __m256 v_3 = _mm256_set_ps(-1,-1,-1,-1,-1,-1,-1,-1);
  __m256 v_4 = _mm256_set_ps(-2,-2,-2,-2,-2,-2,-2,-2);

  __m256 threshold_vec = _mm256_set_ps(threshold,threshold,threshold,threshold,threshold,threshold,threshold,threshold);
  __m256 zero_vec = _mm256_set_ps(0,0,0,0,0,0,0,0);

  //
  for (u64 i = 0; i < (H - 3); i++) {
    for (u64 j = 0; j < (W - 3); j+=8) {

      __m256 f1 = _mm256_loadu_ps(&cframe[INDEX(i+0 , j+0 , W)]);
      __m256 f2 = _mm256_loadu_ps(&cframe[INDEX(i+2 , j+0 , W)]);
      __m256 f3 = _mm256_loadu_ps(&cframe[INDEX(i+0 , j+1 , W)]);
      __m256 f4 = _mm256_loadu_ps(&cframe[INDEX(i+2 , j+1 , W)]);
      __m256 f5 = _mm256_loadu_ps(&cframe[INDEX(i+0 , j+2 , W)]);
      __m256 f6 = _mm256_loadu_ps(&cframe[INDEX(i+2 , j+2 , W)]);

      __m256 gx_1 = _mm256_mul_ps(f1, v_3);
      __m256 gx_2 = _mm256_mul_ps(f2, v_1);
      __m256 gx_3 = _mm256_mul_ps(f3, v_4);
      __m256 gx_4 = _mm256_mul_ps(f4, v_2);
      __m256 gx_5 = _mm256_mul_ps(f5, v_3);
      __m256 gx_6 = _mm256_mul_ps(f6, v_1);

      __m256 gx1 = _mm256_add_ps(gx_1,gx_2);
      __m256 gx2 = _mm256_add_ps(gx1,gx_3);
      __m256 gx3 = _mm256_add_ps(gx2,gx_4);
      __m256 gx4 = _mm256_add_ps(gx3,gx_5);
      __m256 gx  = _mm256_add_ps(gx4,gx_6);

      f1 = _mm256_loadu_ps(&cframe[INDEX(i+0 , j+0 , W)]);
      f2 = _mm256_loadu_ps(&cframe[INDEX(i+1 , j+0 , W)]);
      f3 = _mm256_loadu_ps(&cframe[INDEX(i+2 , j+0 , W)]);
      f4 = _mm256_loadu_ps(&cframe[INDEX(i+0 , j+2 , W)]);
      f5 = _mm256_loadu_ps(&cframe[INDEX(i+1 , j+2 , W)]);
      f6 = _mm256_loadu_ps(&cframe[INDEX(i+2 , j+2 , W)]);

      __m256 gy_1 = _mm256_mul_ps(f1, v_3);
      __m256 gy_2 = _mm256_mul_ps(f2, v_4);
      __m256 gy_3 = _mm256_mul_ps(f3, v_3);
      __m256 gy_4 = _mm256_mul_ps(f4, v_1);
      __m256 gy_5 = _mm256_mul_ps(f5, v_2);
      __m256 gy_6 = _mm256_mul_ps(f6, v_1);

      __m256 gy1 = _mm256_add_ps(gy_1,gy_2);
      __m256 gy2 = _mm256_add_ps(gy1,gy_3);
      __m256 gy3 = _mm256_add_ps(gy2,gy_4);
      __m256 gy4 = _mm256_add_ps(gy3,gy_5);
      __m256 gy  = _mm256_add_ps(gy4,gy_6);



      // __m256 mask_1 = _mm256_cmp_ps(gx, zero_vec, _CMP_LT_OS);

      // gx = _mm256_mul_ps(gx, _mm256_add_ps(_mm256_mul_ps(mask_1,v_4),v_1));

      // mask_1 = _mm256_cmp_ps(gy, zero_vec, _CMP_LT_OS);

      // gy = _mm256_mul_ps(gy, _mm256_add_ps(_mm256_mul_ps(mask_1,v_4),v_1));

      // __m256 mag = _mm256_add_ps(gx,gy);


      // mask_1 = _mm256_cmp_ps(mag, threshold_vec ,_CMP_LT_OS);

      // mag = _mm256_mul_ps(mag, mask_1);

      // __m256 vec_255 = _mm256_set_ps(255,255,255,255,255,255,255,255);

      // __m256 mask_2 = _mm256_add_ps(_mm256_mul_ps(v_3, mask_1),v_1);

      // // mag2 = _mm256_mul_ps(mag, mask_2);

      // mag = _mm256_add_ps(mag, _mm256_mul_ps(vec_255,mask_2));


      // __m256 mask = _mm256_cmp_ps(threshold_vec, mag, _CMP_LT_OS);

      // __m256 mask_255 = _mm256_set_ps(255,255,255,255,255,255,255,255);

      // mag = _mm256_mul_ps(mask, mask_255);



      __m256 mag = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(gx,gx),_mm256_mul_ps(gy,gy)));

      __m256 mask = _mm256_cmp_ps(mag, threshold_vec, _CMP_LT_OS);

      __m256 mask_255 = _mm256_set_ps(255,255,255,255,255,255,255,255);

      __m256 mask_inverse = _mm256_add_ps(_mm256_mul_ps(mask,v_3),v_1);

      mag = _mm256_add_ps(_mm256_mul_ps(mask, mask_255), _mm256_mul_ps(mask_inverse, mag));

      _mm256_store_ps(&oframe[INDEX(i , j , W)], mag);
    }
  }
}
