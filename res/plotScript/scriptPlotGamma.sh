#!/bin/bash

# Write a gnuplot script to compare dynamic adiabatic curves D=f(u) for several gamma  

file='plotScript/plotGamma.gnu'

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
) > $file

for i in Gamma_*.txt
do
    (
        echo "\"$i\" u 1:2 title \"${i%.txt}\",\\" >> $file
    )
done

echo "" >> $file
echo "pause(-1)" >> $file

exit 1
