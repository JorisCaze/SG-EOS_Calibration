# Stiffened Gas equation of state calibration

## About :pencil2:
Stiffened Gas (SG) equations of state (EOS) calibration for compressible diphasic flow under mechanical equilibrium. 
Calibration of SG EOS parameters methods :
* Dynamic of Shock wave
* Liquid and its vapor

## Reference :book:
[Le Métayer, O., Massoni, J., & Saurel, R.](https://doi.org/10.1016/j.ijthermalsci.2003.09.002) (2004).
Elaborating equations of state of a liquid and its vapor for two-phase flow models.

## How to use :wrench:

### General remarks

1. Compilation & Run

	The compilation is done with a Makefile. Under Unix systems it reads :

	```sh
	$ make 
	```

	To run the calibration tool you just need to type the following :
	```sh
	$ ./exe
	```

	If you want to delete all `.o` files from the compilation you can run :
	```sh
	$ make clean
	```

	In case you want to remove all results files and the executable `exe`: 
	```sh
	$ make mrproper
	```

2. Selection of calibration type

	Once executed you will have the shell window asking you to select the method you want to use to calibrate your SG parameters : 
	* (1) Dynamic of Shock wave
	* (2) Liquid and its vapor

	By default the *shock wave calibration* is done for liquid water and the *liquid and its vapor calibration* is configured for dodecane. To create a new calibration for a new fluid or under a new range of study you will need to modify the file according your desired method of calibration.

	These files are located at the following path : `EOS_Calibration/input/Calib_xxx.txt` with ***xxx*** the method name of the calibration. Further description of these files is given in the next section. 

	According to the calibration method used you will also have to add experimental data files in the folder `EOS_Calibration/input/Shock/` and/or `EOS_Calibration/input/Liq-vap/`.

3. Results and plot

	Once the SG parameters are calibrated you will get their values in the shell environnement.

	Theoritical curves can be found in the folder `EOS_Calibration/res/`. If you want to plot theses curves and compare with the experimental ones you will have to go to the folder `EOS_Calibration/res/` and run the script `./runPlotShock.sh` and/or `./runPlotLiqVap.sh`. It might be possible that you don't have the rights to lauch these scripts, thus you will have to do a quick `chmod +x runPlotXXX.sh`


### Shock wave calibration

To run a shock calibration you will have to fill the file `EOS_Calibration/input/Calib_shock.txt`. You will have to give experimental values for the fluid at rest: the sound speed *c<sub>0</sub> (m/s)*, the specific volume *v<sub>0</sub> (m<sup>3</sup>/kg)* and the pressure *p<sub>0</sub> (Pa)*.

To calibrate SG parameters you will also need informations on dynamic adiabatic curves :
* if you already have the experimental proportionnal coefficient (written *a*) of dynamic adiabatic curve *D = c<sub>0</sub> + a.u* you can directly specify it.
* if you don't know it yet, you have to put the value *-1* to *a* and provide an experimental dynamic adiabatic file to let the tool determine it. This file must be located in `EOS_Calibration/input/Shock/AdiabDyn.txt`. The first line of this file is not read, the first column is the material speed *u (m/s)* and the second column is the shock speed *D (m/s)*.

### Liquid and its vapor calibration

To run a liquid/vapor calibration you have to fill the file `EOS_Calibration/input/Calib_liq-vap.txt`. In the section *Compute Cp_k and q_k* of this file you need to give two reference states: the state 0 and the state 1. At each of these reference states you have to give the temperature and the according enthalpies for the liquid and the gas. You must choose the states according to the use range of the EOS. 

In the following section *Compute pInf_k and gamma_k* you have to give again two reference states. Of course, if you want you can use the same reference states than the previous section but this time at a given reference temperature you will also need to specify the specific volume and saturation pressure of each phase. 

To allow the calibration you also must give an experimental saturation curve P<sub>sat</sub>(T) at the path `EOS_Calibration/input/Liq-vap/Psat_exp.txt`. In this file, the first line is not read, the first column is temperature *T (K)* and the second column is the saturation pressure *P<sub>sat</sub> (Pa)*. 

In order to compare the theoric curves obtained with this calibration with experimental data you should provide experimental data in the folder `EOS_Calibration/input/Liq-vap/`, for all these files the first line is not read and they are described below:
* phasic enthalpies `hG_exp.txt` for the Gas and `hL_exp.txt` for the Liquid. The first column is the temperature *T (K)* and the second one is the phasic enthalpy *hG* or *hL (J/kg)*. 
* the latent heat is defined throught the file `Lv_exp`. The first column is the temperature *T (K)* and the second one is the latent heat *Lv (J/kg)*.
* phasic specific volumes `vG_exp.txt` for the Gas and `vL_exp.txt` for the Liquid. The first column is the temperature *T (K)* and the second one is the phasic specific volume *vG* or *vL (J/kg)*.
* the saturation pressure `Psat_exp.txt`. The first column is the temperature *T (K)* and the second one the saturation pressure *P<sub>sat</sub> (Pa)*.
