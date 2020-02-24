set term png enhanced giant size 1980,1080
set output 'graph2.png'
set logscale x 2
set ylabel "Mean access time per byte (ns)"
set xlabel "Buffer Size (KB)"
plot 'results.dat' with lines, 'expected.dat' with lines, 'source2.dat' with lines, 'expected2.dat' with lines
