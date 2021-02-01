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

## Everything under here is not upto date, for the upto date manual install BoFfIn and run boffin --HELP

## Manual
 Usage: boffin [options] file...

 Options:
 * -i	 input file - this is assumed if no argument is given
 * -o	 data output file (csv)
 * -ap  plot a(z) values
 * -pp plot phi(z) values
 * -pha [z] plot phase space at z value (NOT YET IMPLEMENTED)
 
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
