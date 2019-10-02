# Gnuplot eps plot for shock calibration 

reset
set size 1, 1
set terminal eps color enhanced font "14"
set output "adiabDyn.eps"
set style data lp

set key bottom right
set xlabel "Material speed u [m/s]"
set ylabel "Shock speed D [m/s]"

plot \
"Gamma-5.5.txt" u 1:2 title "Gamma-5.5",\
"Gamma-expLin.txt" u 1:2 title "Gamma-expLin",\
"Gamma-exp.txt" u 1:2 title "Gamma-exp",\
