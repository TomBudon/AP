#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <omp.h>

#include "common.h"


i32 convolve_baseline(u8 *m, i32 *f, u64 fh, u64 fw);
void sobel_baseline(u8 *cframe, u8 *oframe, f32 threshold);

void sobel_unroll4(u8 *cframe, u8 *oframe, f32 threshold);
void sobel_unroll8(u8 *cframe, u8 *oframe, f32 threshold);
void sobel_unroll8_intrinsic_AVX2(f32 *cframe, f32 *oframe, f32 threshold);
void sobel_unroll8_intrinsic_AVX2_nosqrt(f32 *cframe, f32 *oframe, f32 threshold);
void sobel_unroll8_intrinsic_AVX2_nosqrt_openMP(f32 *cframe, f32 *oframe, f32 threshold);