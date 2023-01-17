#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>

#include "common.h"


i32 convolve_baseline(u8 *m, i32 *f, u64 fh, u64 fw);
void sobel_baseline(u8 *cframe, u8 *oframe, f32 threshold);

i32 convolve_unroll(u8 *m, i32 *f, u64 fh, u64 fw);
void sobel_unroll4(u8 *cframe, u8 *oframe, f32 threshold);
void sobel_unroll8(u8 *cframe, u8 *oframe, f32 threshold);
