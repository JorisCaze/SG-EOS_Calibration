# Gnuplot plot file to plot hk(T) with k = L, G

reset
# set style data lp
set style line 1 lw 3 lc rgb "blue"

set key bottom right
set xlabel "Temperature T (k)"
set ylabel "Enthalpy (kJ/kg)"

set title "hg"
plot "hG_th.txt" u 1:($2*0.001) title "Theoric" w l ls 1, "hG_exp.txt" u 1:($2*0.001) title "Experimental" w points pt 1 ps 1.2 lc rgb "red"
pause(-1)

set title "hl"
plot "hL_th.txt" u 1:($2*0.001) title "Theoric" w l ls 1, "hL_exp.txt" u 1:($2*0.001) title "Experimental" w points pt 1 ps 1.2 lc rgb "red"
pause(-1)