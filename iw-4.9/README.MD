# Ethanol's iw #

This directory contains the source code for 'iw' version 4.9.
This tool uses nl80211.

## Compile ##

To compile use

```bash
$ make clean
$ make ethanol
```


# Troubleshoot

You may have a problem compiling `iw`.
If you got the error: `Makefile:77: *** Cannot find development files for any supported version of libnl.`, you may need to install one of the following packages:

```
sudo apt-get install pkg-config
sudo apt-get install libnl-dev
sudo apt-get install build-essential
```
