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
		0, 0, 0,
		1, 2, 1 }; //3x3 matrix
  
  //
  for (u64 i = 0; i < (H - 3); i++)
    for (u64 j = 0; j < ((W * 3) - 3); j++)
      {
	gx = convolve_baseline(&cframe[INDEX(i, j, W * 3)], f1, 3, 3);
	gy = convolve_baseline(&cframe[INDEX(i, j, W * 3)], f2, 3, 3);
      
	mag = sqrt((gx * gx) + (gy * gy));
	
	oframe[INDEX(i, j, W * 3)] = (mag > threshold) ? 255 : mag;
      }
}

//
i32 convolve_unroll(u8 *m, i32 *f, u64 fh, u64 fw)
{
  i32 r = 0;

  r += m[INDEX(0 , 0 , W * 3)] * f[INDEX(0 , 0 , fw)];
  r += m[INDEX(1 , 0 , W * 3)] * f[INDEX(1 , 0 , fw)];
  r += m[INDEX(2 , 0 , W * 3)] * f[INDEX(2 , 0 , fw)];

  r += m[INDEX(0 , 1 , W * 3)] * f[INDEX(0 , 1 , fw)];
  r += m[INDEX(1 , 1 , W * 3)] * f[INDEX(1 , 1 , fw)];
  r += m[INDEX(2 , 1 , W * 3)] * f[INDEX(2 , 1 , fw)];

  r += m[INDEX(0 , 2 , W * 3)] * f[INDEX(0 , 2 , fw)];
  r += m[INDEX(1 , 2 , W * 3)] * f[INDEX(1 , 2 , fw)];
  r += m[INDEX(2 , 2 , W * 3)] * f[INDEX(2 , 2 , fw)];
  
  return r;
}

//
void sobel_unroll4(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx, gy;
  f32 mag = 0.0;

  i32 f1[9] = { -1, 0, 1,
		-2, 0, 2,
		-1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
		0, 0, 0,
		1, 2, 1 }; //3x3 matrix

  i32 m = ((W * 3) - 3) - (((W * 3) - 3) & 3);
  
  //
  for (u64 i = 0; i < (H - 3); i++) {
    for (u64 j = 0; j < m; j+=4) {
      gx = convolve_unroll(&cframe[INDEX(i  , j  , W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j  , W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j  , W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+1, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+1, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+1, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+2, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+2, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+2, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+3, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+3, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+3, W * 3)] = (mag > threshold) ? 255 : mag;
    }
  }

  for (u64 i = 0; i < (H - 3); i++) {
    for (u64 j = m; j < ((W * 3) - 3); j++) {
      gx = convolve_unroll(&cframe[INDEX(i, j, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i, j, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i, j, W * 3)] = (mag > threshold) ? 255 : mag;
    }
  }
}

//
void sobel_unroll8(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx, gy;
  f32 mag = 0.0;

  i32 f1[9] = { -1, 0, 1,
		-2, 0, 2,
		-1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
		0, 0, 0,
		1, 2, 1 }; //3x3 matrix

  i32 m = ((W * 3) - 3) - (((W * 3) - 3) & 7);

  //
  for (u64 i = 0; i < (H - 3); i++) {
    for (u64 j = 0; j < m; j+=8) {
      gx = convolve_unroll(&cframe[INDEX(i  , j  , W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j  , W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j  , W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+1, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+1, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+1, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+2, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+2, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+2, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+3, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+3, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+3, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+4, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+4, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+4, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+5, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+5, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+5, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+6, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+6, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+6, W * 3)] = (mag > threshold) ? 255 : mag;

      gx = convolve_unroll(&cframe[INDEX(i  , j+7, W * 3)], f1, 3, 3);
      gy = convolve_unroll(&cframe[INDEX(i  , j+7, W * 3)], f2, 3, 3);
          
      mag = sqrt((gx * gx) + (gy * gy));
      
      oframe[INDEX(i  , j+7, W * 3)] = (mag > threshold) ? 255 : mag;
    }
  }

    for (u64 i = 0; i < (H - 3); i++) {
      for (u64 j = m; j < ((W * 3) - 3); j++) {
        gx = convolve_unroll(&cframe[INDEX(i, j, W * 3)], f1, 3, 3);
        gy = convolve_unroll(&cframe[INDEX(i, j, W * 3)], f2, 3, 3);
            
        mag = sqrt((gx * gx) + (gy * gy));
        
        oframe[INDEX(i, j, W * 3)] = (mag > threshold) ? 255 : mag;
      }
    }
}