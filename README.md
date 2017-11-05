<div align="center">
  <img src="https://github.com/Leviyu/EQTime/blob/master/image/header.png"><br><br>
</div>


# EQTime
EQTime is An automated seismic data processing tool, featuring fast travel time 
acquisition and other measurements, including waveform distortion and attenuation. 

It is designed to automatically process large mount of seismic data and extract 
useful information systematically and objectively.

The code is developed in C and includes large numbers of time series operations,
	including:
* Iterative Stacking
* Cross correlation
* Stretching
* Time series convolution
The C code library is maintained under [Maligari,Empirical Wavelet Construction](https://github.com/Leviyu/Maligaro)


## Usage
Go into SRC directory and do
`./config`

Change parameters in `INFILE.mother`, including:
* Taks name
* Phase list
* Earthquake File
* Process Number

For each phase, there is a corresponding file to it located under `INFILE_DIR/INFILE_${PHASE_NAME}`, change parameters in it depending on your need.
The description of each entry in each phase infile is only shown in `INFILE_DIR/INFILE_S`.

To execute the code:
```shell
./big_run.sh
```

The produced catalog plots are located under `../PLOTDIR`, the directory name is same as the `TASK_NAME` specified in `INFILE.mother`.


After all processes have finished, to generate a master catalog PDF file of all pdf within the directory, go into the directory and execute:
```shell
./c40.generate_master_pdf.sh .
```

## To do List
- [x] Renew CMT information to include recent year events
- [ ] Debug, possible depth phase travel time missing
- [ ] Combine virtual station into current setup
 

