#!/bin/bash


versions=("BASELINE" "UNROLL4" "UNROLL8" "INTRINSIC")
# versions=("INTRINSIC")


compilers=("gcc" "clang-14" "icc" "icx")
# compilers=("gcc")

oflags=("-O1" "-O2" "-O3" "-Ofast")
# oflags=("-Ofast")

make clean

rm -f data/*

for compiler in ${compilers[*]}; do

               # # BASELINE -O1
               # echo -n "BASELINE;" >> data/$compiler.dat

               # make CC="$compiler" VFLAGS="-DBASELINE=1" OPFLAGS="-march=native" OFLAGS="-O1"
               
               # # taskset -c 2 ./sobel in/input.raw out/output.raw
               # echo "`taskset -c 2 ./sobel in/input.raw out/output.raw | cut -d';' -f5`;" >> data/$compiler.dat

               # make clean

     for version in ${versions[*]}; do

          echo -n "$version;" >> data/$compiler.dat
     
          for oflag in ${oflags[*]}; do
     
               make CC="$compiler" VFLAGS="-D$version=1" OFLAGS="$oflag"
               
               # taskset -c 2 ./sobel in/input.raw out/output.raw
               echo -n "`taskset -c 2 ./sobel in/input.raw out/output.raw | cut -d';' -f5`;" >> data/$compiler.dat

               make clean

          done

          echo "" >> data/$compiler.dat

     done

done

gnuplot plot.gp
