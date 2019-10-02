#!/bin/bash

# Plot adiabDyn in eps format

file='plotScript/epsPlotShock.gnu'
(
    echo "# Gnuplot eps plot file to plot D=f(u) for each gamma"
    echo ""
    echo "reset"
    echo "set size 1, 1"
    echo "set terminal eps color enhanced font \"14\""
    echo "set output \"adiabDyn.eps\"" 
    echo "set style data lp"
    echo "" 
    echo "set key bottom right"
    echo "set xlabel \"Material speed u [m/s]\""
    echo "set ylabel \"Shock speed D [m/s]\""
    echo ""
    echo "plot \\"
) > $file

for i in Gamma-*.txt
do
    (
        echo "\"$i\" u 1:2 title \"${i%.txt}\",\\" >> $file
    )
done

exit 1
