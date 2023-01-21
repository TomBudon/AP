set term png size 2000,1250
set output "perf1_BW.png"

set grid

set datafile separator ";"

set auto x

set key right top

set xlabel "Version"
set ylabel "MiB/s" 

set style data histogram
set style fill solid border -1
set boxwidth 0.5

# set yrange [0:750]

set xtic rotate by -45 scale 0 

set multiplot layout 2,2 rowsfirst title "sobel" 

set title "GCC"
plot "data/BW-gcc.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

set title "Clang"
plot "data/BW-clang-14.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

set title "ICC"
plot "data/BW-icc.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

set title "ICX"
plot "data/BW-icx.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

unset multiplot



set term png size 2000,1250
set output "perf1_FPS.png"

set grid

set datafile separator ";"

set auto x

set key right top

set xlabel "Version"
set ylabel "FPS" 

set style data histogram
set style fill solid border -1
set boxwidth 0.5

# set yrange [0:750]

set xtic rotate by -45 scale 0 

set multiplot layout 2,2 rowsfirst title "sobel" 

set title "GCC"
plot "data/FPS-gcc.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

set title "Clang"
plot "data/FPS-clang-14.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

set title "ICC"
plot "data/FPS-icc.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

set title "ICX"
plot "data/FPS-icx.dat" u 2:xtic(1) t "-O1" lc "blue", "" u 3:xtic(1) t "-O2" lc "orange", "" u 4:xtic(1) t "-O3" lc "red", "" u 5:xtic(1) t "-Ofast" lc "purple"

unset multiplot