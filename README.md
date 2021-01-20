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
Requires ''gcc'' and ''gsl'' to be installed as well as ''Python3'', ''numpy'', ''matplotlib'' and ''python3-devs'' for plotting:

###### Debian/Ubuntu etc.
```
sudo apt-get install gcc gsl-bin python3 python3-devs python3-matplotlib python3-numpy
```

## Manual
 Usage: boffin [options] file...

 Options:
 * ''-v''	 display infomation while running model
 * ''-i''	 input file - this is assumed if no argument is given
 * ''-o''	 data output file (csv)
 * ''-ap''  plot a(z) values
 * ''-pp'' plot phi(z) values
 * ''-pha [z]'' plot phase space at z value
