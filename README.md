# BOFFIN - BDF One dimentianal FEL INtergrator

## Installing (GNU/Linux)

In a terminal, run the following commands:

```
cd ~/Downloads/
git clone https://github.com/Paulire/boffin-FEL-sim
cd boffin-FEL-sim
sudo ./install
```
Requires ''gcc'' and ''gsl'' to be installed:

Debian/Ubuntu 
```
sudo apt-get install gcc gsl-bin
```

## Manual
 Usage: boffin [options] file...

 Options:
 * ''-v''	 display infomation while running model
 * ''-i''	 input file - this is assumed if no argument is given
 * ''-o''	 data output file (csv)
 * ''-a''  plot $a(z)$ values
 * ''-phi'' plot $phi(z) values
 * ''-pha'' plot phase space
