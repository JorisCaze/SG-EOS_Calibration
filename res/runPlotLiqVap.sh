#!/bin/bash

# Run all gnuplot files for liquid and its vapor calibration

# Preview
gnuplot plotScript/plotEnthalpy.gnu
gnuplot plotScript/plotLatentHeat.gnu
gnuplot plotScript/plotSpeVol.gnu
gnuplot plotScript/plotPsat.gnu

# Eps
gnuplot plotScript/epsPlotLiqVap.gnu

exit 1