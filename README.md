# BOFfIn - BDF One dimentianal FEL INtergrator

## Installing

#### GNU/Linux

In a terminal, run the following commands:

```
cd ~/Downloads/
git clone https://github.com/Paulire/boffin-FEL-sim
cd boffin-FEL-sim
sudo ./install
```
Requires ''gcc'' and ''gsl'' to be installed for integration. For plotting ''Python3'', ''numpy'', ''matplotlib'' and ''python3-devs'' are required:

###### Debian/Ubuntu etc.
```
sudo apt-get install gcc gsl-bin python3 python3-devs python3-matplotlib python3-numpy
```

NOTE: this has only be tested on a Debian distrobution, edit the ''install'' file according to your settup.

## Manual
 Usage: boffin [options] file...

 Options:
 * -i	 input file - this is assumed if no argument is given
 * -o	 data output file (csv)
 * -s  runs simulation with shot noise
 * -ap  plot a(z) values
 * -pp plot phi(z) values
 * -pha [z]  plot phase space at z value 
 * -bp       Plot the bunching paramiter
 * -hp [h]   Plot for a certain harmonic (only works if -h flag used while running boffin)
 * -plotmode	Will only plot and not run the simulation. Note: the input file is only valed
 * -CMDMODE  enerer CMD input mode
 * -s        Run with shot noise in pondermotive phase
 * -sa       Run with shot noise in pondermotive phase and in p
 * -h [n]    Number of harmonics to model. n = 1 implies first harmonic; n = 2 implies first and thrid harmonic; etc.
 * -shift    Have a shift in the pondermotive phase, exact condtions defined in input file
 
 Input File Syntax:
 COMMAND = VALUE;
 * ```N_theta```  The number of theta points between 0 and 2pi.
 * ```N_p```      The number of points between +/-m\*sigma.
 * ```sigma```    Energy spread veriance
 * ```off_p```    Mean energy offset
 * ```a_0```      Inital a value for the first harmonic, all subsequent odd harmonics are assumed to be two orders of magnitude lower.
 * ```z_0```      Inital z value
 * ```z_f```      Final z value
 * ```z_num```    Number of points between z_0 and z_f
 * ```shot_n_coff```    Shot noise n coaficant (-s flag only)
 * ```mean_electron```    Macroparticle mean electron number (-sa flag only)
 * ```pulse_duration```    Pulse duration (-sa flag only)
 * ```rms_undulator```     Route mean squared of the undulator paramiter (-h flag greater that 1 must be used)
 * ```theta_shift_start``` Start of pondermotive phase shift (-shift flag only)
 * ```theta_shift_interval``` Interval between phase shifts (-shift flag only)
 * ```theta_shift_interval``` n value for harmomic generation 2pi/n => nth harmonic to be amplified

Users can interact with boffin via an input file and comand line
arguments. The input file's syntax was outlined above. The following is an
example of how to use boffin. It should be noted that all commands contain
an '=' and are ended by a ';'

Examples are avalibale in the example folder (TO BE MADE)
