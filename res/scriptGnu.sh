#!/bin/bash

# Write a gnuplot script to compare curves with data from several files 

(
    echo "# Gnuplot plot file to plot D=f(u) for each gamma"
    echo ""
    echo "reset"
    echo "set style data lp"
    echo "" 
    echo "set key bottom right"
    echo "set xlabel \"Material speed u [m/s]\""
    echo "set ylabel \"Shock speed D [m/s]\""
    echo ""
    echo "plot \\"
) > plotGamma.gnu

for i in Gamma_*.txt
do
    (
        echo "\"$i\" u 1:2 title \"${i%.txt}\",\\" >> plotGamma.gnu
    )
done

echo "" >> plotGamma.gnu
echo "pause(-1)" >> plotGamma.gnu

exit 1
