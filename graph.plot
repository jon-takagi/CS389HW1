set term png enhanced giant size 1980,1080
set output 'graph.png'
set logscale x 2
set ylabel "Mean access time per byte (ns)"
set xlabel "Buffer Size"
plot 'results.dat' with lines
