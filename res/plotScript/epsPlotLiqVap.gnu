# Gnuplot eps plot for liquid-vapor calibration

# ---------------

reset
set size 1, 1
set terminal eps color enhanced font "14"
set style line 1 lw 3 lc rgb "blue"

set key bottom right
set xlabel "Temperature T (k)"
set ylabel "Enthalpy (kJ/kg)"

set output "hG.eps"
set title "h_g"
plot "hG_th.txt" u 1:($2*0.001) title "Theoric" w l ls 1, "hG_exp.txt" u 1:($2*0.001) title "Experimental" w points pt 1 ps 1.2 lc rgb "red"

set output "hL.eps"
set title "h_l"
plot "hL_th.txt" u 1:($2*0.001) title "Theoric" w l ls 1, "hL_exp.txt" u 1:($2*0.001) title "Experimental" w points pt 1 ps 1.2 lc rgb "red"

# ---------------

reset
set size 1, 1
set terminal eps color enhanced font "14"
set style line 1 lw 3 lc rgb "blue"

set key bottom left
set xlabel "Temperature T (k)"
set title "Latent Heat Lv (kJ/kg)"

set output "Lv.eps"
plot "Lv_th.txt" u 1:($2*0.001) title "Theoric" w l ls 1, "Lv_exp.txt" u 1:($2*0.001) title "Experimental" w points pt 1 ps 1.2 lc rgb "red"

# ---------------

reset
set size 1, 1
set terminal eps color enhanced font "14"
set style line 1 lw 3 lc rgb "blue"

set key top left
set xlabel "Temperature T (k)"
set title "Saturated pressure Psat (bar)"

set output "Psat.eps"
plot "Psat_th.txt" u 1:($2*0.00001) title "Theoric" w l ls 1, "Psat_exp.txt" u 1:($2*0.00001) title "Experimental" w points pt 1 ps 1.2 lc rgb "red"

# ---------------

reset
set size 1, 1
set terminal eps color enhanced font "14"
set style line 1 lw 3 lc rgb "blue"

set xlabel "Temperature T (k)"
set ylabel "Specific volume (m^3/kg)"

set key top right
set output "vG.eps"
set title "v_g"
plot "vG_th.txt" u 1:2 title "Theoric" w l ls 1, "vG_exp.txt" u 1:2 title "Experimental" w points pt 1 ps 1.2 lc rgb "red"

set key top left
set output "vL.eps"
set title "v_l"
plot "vL_th.txt" u 1:2 title "Theoric" w l ls 1, "vL_exp.txt" u 1:2 title "Experimental" w points pt 1 ps 1.2 lc rgb "red"