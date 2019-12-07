# see: https://github.com/Leviyu/EQTime/blob/postprocess/README.md

<div align="center">
  <img src="https://github.com/Leviyu/EQTime/blob/master/image/header.png"><br><br>
</div>


# EQTime
EQTime is an automated seismic data processing tool, featuring fast travel time 
acquisition and other measurements, including waveform distortion and attenuation. 

It is designed to automatically process large mount of seismic data and extract 
useful information systematically and objectively.

The code is developed in C and includes large numbers of time series operations,
	including:
* Iterative Stacking
* Cross correlation
* Time series convolution/waveform distortation

The C code library is inherited from [Empirical Wavelet Construction,Maligaro](https://github.com/Leviyu/Maligaro)


Currently this code is only executable on our own server at Arizona State University, as the required data repository is there. 

This code is still improving, if there is any bug, please take a few minutes and open an issue.

## Dependency

* Python (>=3.4)
* GMT 4.5 (>5.0 version would not be compatible)
* SAC (>101.6a)
* TauP (>2.4.5)
* SOD (>=3.2.8)

EQTime also contains a superversued classification module, the following package is required to function properly:
* numpy
* scipy
* pandas
* seaborn
* IPython
* matplotlib
* sklearn
* lighgbm



Please also include the following directory in your search path:
/mnt/data2/maligaro/bin

## Preparation
EQTime has some further dependencies, you may want to:
* Add additional CMT information into `./code_dir/CMT.data`, for each event that you want to process

## Data Download

The seismic data used for this package is pre-download and stored on our sever at Geophysics Lab at Arizona State University. This package directly download data from the sever through SSH.

To automate the process, you would need access of the server, please contact Professor [Garnero](http://garnero.asu.edu/). To see how to setup [SSH connection without password](http://www.linuxproblem.org/art_9.html).

Another way to access the data is to mount the disk on your local drive, which would need assistance from IT support.

Modify script `./SRC/code_dir/get_EQ_sac` to choose the suitable option.

## Usage

First, clone the repository to your local directory
```shell
git clone https://github.com/Leviyu/EQTime.git
```

Then, go to `SRC` directory, where most of the code would be implemented.


We have maintained a `INFILE` to manage the parameters for this task, include:
* Taks name (unique ID to identify each run)
* Phase list (Phase of interest)
* Earthquake File (Earthquake of interest)
* Number of processes to run simultaneously
* Process Number (number of working process to be run at the same time )
* Support software path, including taup, sac and gmt


### Phase dependent parameter
For each different phases, we have individual configure file to manage the filter, the frequency range, the distance range, et al. All phase dependent configure file is listed under `./INFILE_DIR/` and each file is named after `./INFILE_DIR/INFILE_${PHASE_NAME}`:

For explanation of each parameter, please refer to `./INFILE_DIR/INFILE_S`. Some of the key parameters you should change include:
* Phase Name
* Component
* Filter Flag
* Distance Range
* Frequency Range

To execute the code, try:
```shell
./big_run.sh
```

All processing happends under `../DATADIR/` and a corresponding subdirectory is created named after `TASK_NAME`.
The produced catalog pdf plots are located under `../PLOTDIR`, the directory name is same as the `TASK_NAME` specified in `INFILE`.


 

