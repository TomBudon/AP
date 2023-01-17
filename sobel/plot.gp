set term png size 2000,1250
set output "perf1.png"

set grid

set datafile separator ";"

set auto x

set key right top

set xlabel "Version"
set ylabel "MiB/s" 

set style data histogram
set style fill solid border -1
set boxwidth 0.5

set yrange [0:750]

set xtic rotate by -45 scale 0 

set multiplot layout 2,2 rowsfirst title "sobel" 

set title "GCC"
plot "data/gcc.dat" u 2:xtic(1) t "-O2" lc "blue", "" u 3:xtic(1) t "-O3" lc "orange"

# set title "Clang"
# plot "data/clang-14.dat" u 2:xtic(1) t "" lc "blue", "" u 3:xtic(1) t "-O3" lc "orange"

# set title "ICC"
# plot "data/icc.dat" u 2:xtic(1) t "" lc "blue", "" u 3:xtic(1) t "-O3" lc "orange"

# set title "ICX"
# plot "data/icx.dat" u 2:xtic(1) t "" lc "blue", "" u 3:xtic(1) t "-O3" lc "orange"

unset multiplot