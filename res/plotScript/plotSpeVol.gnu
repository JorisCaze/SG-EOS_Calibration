# Gnuplot plot file to plot vk(T) with k = L, G

reset
# set style data lp
set style line 1 lw 3 lc rgb "blue"

set key bottom right
set xlabel "Temperature T (k)"
set ylabel "Specific volume (m3/kg)"

set title "vg"
plot "vG_th.txt" u 1:2 title "Theoric" w l ls 1, "vG_exp.txt" u 1:2 title "Experimental" w points pt 1 ps 1.2 lc rgb "red"
pause(-1)

set title "vl"
plot "vL_th.txt" u 1:2 title "Theoric" w l ls 1, "vL_exp.txt" u 1:2 title "Experimental" w points pt 1 ps 1.2 lc rgb "red"
pause(-1)