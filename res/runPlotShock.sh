#!/bin/bash

# Run gnuplot files for shock calibration

./plotScript/scriptPlotGamma.sh
gnuplot plotScript/plotGamma.gnu

exit 1