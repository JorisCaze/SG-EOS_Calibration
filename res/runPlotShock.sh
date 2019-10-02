#!/bin/bash

# Run gnuplot files for shock calibration

# Preview
./plotScript/scriptPlotGamma.sh
gnuplot plotScript/plotGamma.gnu

# Eps
./plotScript/epsPlotShock.sh
gnuplot plotScript/epsPlotShock.gnu

exit 1