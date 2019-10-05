# Stiffened Gas equation of state calibration

## About :pencil2:
Stiffened Gas (SG) equations of state (EOS) calibration for compressible diphasic flow under mechanical equilibrium. 
Calibration of SG EOS parameters methods :
* Dynamic of a Shock wave
* Liquid and its vapor

The calibration *dynamic of a shock wave* is done with the dynamic adiatic experimental curve (shock speed function of the material speed) and the calibration *liquid and its vapor* is done with experimental saturation curves. 

## Reference :book:
[Le Métayer, O., Massoni, J., & Saurel, R.](https://doi.org/10.1016/j.ijthermalsci.2003.09.002) (2004).
Elaborating equations of state of a liquid and its vapor for two-phase flow models.

## How to use :wrench:

### General remarks

#### 0. Setup
	
This tool is fully compatible under Unix systems, however it is necessary to have the following utilities : 
* Make
* Gnuplot

To install them through a package manager you can use the `apt` command :

```sh
$ sudo apt install pkg-name
```

With `pkg-name` the name of the package that you want to install.

#### 1. Compilation & Run

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

In case you want to remove the executable `exe`: 
```sh
$ make mrproper
```

To delete all the result files you can do :
```sh
$ make resClean
```

#### 2. Selection of calibration type

Once executed you will have the shell window asking you to select the method you want to use to calibrate your SG parameters : 
* (1) Dynamic of Shock wave
* (2) Liquid and its vapor

For liquid and its vapor calibration you can use two differents methods:
* **DM** Calibration with two reference states and experimental saturated pressure curve
* **LSM** Calibration with one reference state and a full-set of experimental curves at saturation

In the section **Liquid and its vapor calibration** you will have a complete description of these methods and the way to use them. 

By default the *shock wave calibration* is done for liquid water and the *liquid and its vapor calibration* is configured for dodecane. To create a new calibration for a new fluid or under a new range of study you will need to modify the file according your desired method of calibration.

These files are located at the following path : `SG-EOS_Calibration/input/xxx/` with ***xxx*** the method name of the calibration. Further description of these files is given in the next section. 

According to the calibration method used you will also have to add experimental data files in the folder `SG-EOS_Calibration/input/Shock/` and/or `SG-EOS_Calibration/input/Liq-vap/`.

#### 3. Results and plot

Once the SG parameters are calibrated you will get their values in the shell environnement.

Theoritical curves can be found in the folder `SG-EOS_Calibration/res/`. If you want to plot theses curves and compare with the experimental ones you will have to go to the folder `SG-EOS_Calibration/res/` and run the script `./runPlotShock.sh` and/or `./runPlotLiqVap.sh`. It might be possible that you don't have the rights to launch these scripts, thus you will have to do :

```sh
$ find . -type f -name "*.sh" -exec chmod +x {} \;
```

Scripts `runPlotXXX.sh` (XXX is the calibration used) display a preview of all curves in Gnuplot GUI and create in the same time plot files in `eps` format in the repository `SG-EOS_Calibration/res/`.

#### 4. Library of test cases 
	
Under the folder `SG-EOS_Calibration/input/lib/` you can find test cases for shock and liquid-vapor calibration. There are input files and the associated experimental data. 

For shock wave calibration the test case is:
* Liquid water with experimental data extracted from *S.P. Marsh, LASL Shock Hugoniot Data, University of California Press, Berkeley, CA, 1980*

For liquid/vapor calibration the test cases are:
* Dodecane in the temperature range T &isin; [300-500 K].
Experimental data are from [J. R. Simões-Moreira](https://elibrary.ru/item.asp?id=5644923), *“Adiabatic evaporation waves” Ph.D. thesis, Rensselaer Polytechnic Institute, Troy, NY, 1994.*
* Dodecane in the temperature range T &isin; [400-600 K]. Experimental data from the same source.
* Water and steam in the temperature range T &isin; [300-500 K].
Experimental data are extracted from [NIST website](https://webbook.nist.gov/chemistry/fluid).

The availibity of the test cases for liquid-vapor calibration according to the method used is summarized here:

| Method/Test | Dodecane T &isin; [300-500 K] | Dodecane T &isin; [400-600 K] | Water/Steam T &isin; [300-500 K] |
|:-----------:|:-----------------------------:|:-----------------------------:|:--------------------------------:|
|      DM     |               x               |                               |                 x                |
|     LSM     |               x               |               x               |                 x                |	

### Shock wave calibration

To run a shock calibration you will have to fill the file `SG-EOS_Calibration/input/Shock/Calib_shock.txt`. You will have to give experimental values for the fluid at rest: the sound speed *c<sub>0</sub> (m/s)*, the specific volume *v<sub>0</sub> (m<sup>3</sup>/kg)* and the pressure *p<sub>0</sub> (Pa)*.

To calibrate SG parameters you will also need informations on dynamic adiabatic curves :
* if you already know the experimental proportionnal coefficient (written *a*) of dynamic adiabatic curve *D = c<sub>0</sub> + a.u* you can directly specify it. Therefore, you will to specify the shock speed interval of study in the section *Interval of shock speed*.
* if you don't know it yet, you have to put the value *-1.* to *a* and provide an experimental dynamic adiabatic file to let the tool determine it. This file must be located in `SG-EOS_Calibration/input/Shock/AdiabDyn.txt`. The first line of this file is not read, the first column is the material speed *u (m/s)* and the second column is the shock speed *D (m/s)*.

### Liquid and its vapor calibration

To calibrate the SG parameters for liquid and its vapor you can use two differents methods:
* DM Calibration with two reference states and experimental saturated pressure curve
* LSM Calibration with one reference state and a full-set of experimental curves at saturation

**DM** calibration stands for **D**ifferential **M**ethod and **LSM** calibration stands for **L**east **S**quare **M**ethod. 

#### *DM Calibration*

The Differential Method (DM) uses two reference states and experimental saturated pressure curve to calibrate SG parameters as it is done in [Le Métayer, O., Massoni, J., & Saurel, R.](https://doi.org/10.1016/j.ijthermalsci.2003.09.002). All the input files must be located under the following path `SG-EOS_Calibration/input/Liq-vap/DM/`, more details are given below. 

To run a liquid/vapor DM calibration you have to fill the file `SG-EOS_Calibration/input/Liq-vap/DM/Calib_liq-vap_DM.txt`. In the section *Compute Cp_k and q_k* of this file you need to give two reference states: the state 0 and the state 1. At each of these reference states you have to give the temperature and the according enthalpies for the liquid and the gas. You must choose the states according to the use range of the EOS. 

In the following section *Compute pInf_k and gamma_k* you have to give again two reference states. Of course, if you want you can use the same reference states than the previous section but this time at a given reference temperature you will also need to specify the specific volume and saturation pressure of each phase. Anyway, in this section you have the possibility to change the reference states, it's often used to get a better calibration for the liquid phase.  

To allow the calibration you also _must_ give an experimental saturation curve P<sub>sat</sub>(T) at the path `SG-EOS_Calibration/input/Liq-vap/DM/Psat_exp.txt`. In this file, the first line is not read, the first column is temperature *T (K)* and the second column is the saturation pressure *P<sub>sat</sub> (Pa)*. 

If you want to compare the theoric curves obtained with this calibration with experimental data you should provide experimental data in the folder `SG-EOS_Calibration/input/Liq-vap/DM/Liq-vap/`. For all these files the first line is not read and they are described below:
* phasic enthalpies `hG_exp.txt` for the Gas and `hL_exp.txt` for the Liquid. The first column is the temperature *T (K)* and the second one is the phasic enthalpy *hG* or *hL (J/kg)*. 
* the latent heat is defined throught the file `Lv_exp`. The first column is the temperature *T (K)* and the second one is the latent heat *Lv (J/kg)*.
* phasic specific volumes `vG_exp.txt` for the Gas and `vL_exp.txt` for the Liquid. The first column is the temperature *T (K)* and the second one is the phasic specific volume *vG* or *vL (m<sup>3</sup>/kg)*.
* the saturation pressure `Psat_exp.txt`. The first column is the temperature *T (K)* and the second one the saturation pressure *P<sub>sat</sub> (Pa)*.

Remark : in this method, each experimental data file can have a different number of points since they are only used to plot experimental curves (the pressure saturation curve is also used to calibrate as mentioned above). 

#### *LSM Calibration*

The Least Square Method (LSM) is using one reference state and a full-set of saturation curves to determine the SG parameters. All the input files must be located under the following path `SG-EOS_Calibration/input/Liq-vap/LSM/`, more details are given below. 

To run a liquid/vapor LSM calibration you have to fill the file `SG-EOS_Calibration/input/Liq-vap/LSM/Calib_liq-vap_LSM.txt`. In this file you have to specify the pressure *p<sub>0</sub> (Pa)*, the density *&rho;<sub>0</sub> (kg/m<sup>3</sup>)* and the sound speed *c<sub>0</sub> (m/s)* of the liquid reference state.

You have to specify the experimental data at saturation in the file `SG-EOS_Calibration/input/Liq-vap/LSM/expData.txt`. The first line of this file is not read, there are 7 rows to fill (each one separated by a blank or a tab) as follows: 

|     T (K)     |     Psat (Pa)    | vG (m<sup>3</sup>/kg) | vL (m<sup>3</sup>/kg) |    hG (J/kg)   |    hL (J/kg)   |    Lv (J/kg)   |
|:-------------:|:----------------:|:---------------------:|:---------------------:|:--------------:|:--------------:|:--------------:|
| T<sub>1</sub> | Psat<sub>1</sub> |     vG<sub>1</sub>    |     vL<sub>1</sub>    | hG<sub>1</sub> | hL<sub>1</sub> | Lv<sub>1</sub> |
|      ...      |        ...       |          ...          |          ...          |       ...      |       ...      |       ...      |
| T<sub>N</sub> | Psat<sub>N</sub> |     vG<sub>N</sub>    |     vL<sub>N</sub>    | hG<sub>N</sub> | hL<sub>N</sub> | Lv<sub>N</sub> |

Each column is respectively the temperature *T*, the saturated pressure *Psat*, the specific volume of the gas *vG* and of the liquid *vL*, the enthalpy of the gas *hG* and of the liquid *hL*, and the latent heat *Lv*.

Remark: the decimal separator must be a dot. 