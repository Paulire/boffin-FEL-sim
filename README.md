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
 * -pha [z] plot phase space at z value 
 * -plotmode	 Will only plot and not run the simulation. Note: the input file is only valed
 * -CMDMODE  enerer CMD input mode
 
 Input File Syntax:
 COMMAND = VALUE;
 * ```N_theta```  The number of theta points between 0 and 2pi.
 * ```N_p```      The number of points between +/-m\*sigma.
 * ```sigma```    Energy spread veriance
 * ```off_p```    Mean energy offset
 * ```a_0```      Inital a value
 * ```z_0```      Inital z value
 * ```z_f```      Final z value
 * ```z_num```    Number of points between z_0 and z_f
 * ```shot_n_coff```    Shot noise n coaficant

Users can interact with boffin via an input file and comand line
arguments. The input file's syntax was outlined above. The following is an
example of how to use boffin. It should be noted that all commands contain
an '=' and are ended by a ';'

**input_file.bffn**
```
N_theta=1000;
phi_0=0;
a_0=0.00001;
z_f=20;
z_0=0;
z_num = 1000;
N_p = 1;
shot_noise = 0
```

This is run in the terminal:

    ```$ boffin -i input_file.bffn -o out_data.csv -ap```

A user can bypass the input file and use CMDMODE:

     ```$ boffin -o output.csv -CMDMODE "N_theta=100; phi_0=0; a_0=0.001;z_f=15;z_0=0;z_num = 1000;N_p = 1;" -ap```
