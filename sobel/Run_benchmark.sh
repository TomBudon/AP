#!/bin/bash


versions=("BASELINE" "UNROLL4" "UNROLL8" "INTRINSIC" "NOSQRT" "PARALLEL")
# versions=("NOSQRT")


compilers=("gcc" "clang-14" "icc" "icx")
# compilers=("gcc")

oflags=("-O1" "-O2" "-O3" "-Ofast")
# oflags=("-Ofast")

make clean

rm -f data/*

for compiler in ${compilers[*]}; do

     for version in ${versions[*]}; do

          echo -n "$version;" >> data/BW-$compiler.dat
          echo -n "$version;" >> data/FPS-$compiler.dat
     
          for oflag in ${oflags[*]}; do

               # openmp ne marche pas avec clang
               if [ "$version" != "PARALLEL" ] || [ "$compiler" != "clang-14" ] 
               then
     
                    make CC="$compiler" VFLAGS="-D$version=1" OFLAGS="$oflag"

                    # no taskset for parallel
                    if [ "$version" == "PARALLEL" ]
                    then
                         echo "`./sobel in/input.raw out/output.raw`" >> data/$compiler.dat
                    else
                         # taskset -c 2 ./sobel in/input.raw out/output.raw
                         echo "`taskset -c 2 ./sobel in/input.raw out/output.raw`" >> data/$compiler.dat
                    fi

                    echo -n "`cat data/$compiler.dat | cut -d';' -f5`;" >> data/BW-$compiler.dat
                    echo -n "`cat data/$compiler.dat | cut -d';' -f7`;" >> data/FPS-$compiler.dat

                    rm -f data/$compiler.dat                

                    make clean

               fi
               
          done

          echo "" >> data/BW-$compiler.dat
          echo "" >> data/FPS-$compiler.dat

     done

done

gnuplot plot.gp
