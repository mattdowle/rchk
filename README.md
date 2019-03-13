
This project consists of several bug-finding tools that look for memory
protection errors in C source code using R API, that is in the source code
of [R](http://www.r-project.org/) itself and packages.  The tools perform
whole-program static analysis on LLVM bitcode and run on Linux.  About
200-300 memory protection bugs have been found using rchk and fixed in R. 
rchk is now regularly used to check [CRAN
packages](https://github.com/kalibera/cran-checks/tree/master/rchk).

The tools can be installed automatically into a Singularity container and
invoke that container from command line to check a single package.  This is
the easiest way to checking a package (Singularity requires Linux, in
principle it is very similar to Docker).  See [Singularity
Instructions](doc/SINGULARITY.md)], [Installation](doc/INSTALLATION.md).
I've tested on Ubuntu 18.04 as host system.

The tools can also be installed automatically into a Virtualbox or Docker
container and log into that virtual machine and use it from command line
interactively to check packages, check R itself, etc.  Virtualbox
installation is possible on Windows, macOS and Linux; Docker installation
only on Linux.  See [Installation](doc/INSTALLATION.md) and the steps below
on checking the first package.

On Linux, one can also install `rchk` natively, which has been tested on
recent Ubuntu, Debian and Fedora distributions.  This is the fastest and
most flexible way to use `rchk` for users working on Linux.  See
[Installation](doc/INSTALLATION.md) and the steps below on checking the
first package.

## Checking the first package

For this that one also needs to install `subversion`, `rsync` (`apt-get
install subversion rsync`, but already available in the automated install). 
More importantly, one also needs any dependencies needed by that package.

1. Build R producing also LLVM bitcode
	* `svn checkout https://svn.r-project.org/R/trunk`
	* `cd trunk`
	* `. ../scripts/config.inc` (*in automated install*, `. /opt/rchk/scripts/config.inc`)
	* `. ../scripts/cmpconfig.inc` (*in automated install*, `. /opt/rchk/scripts/cmpconfig.inc`)
	* `../scripts/build_r.sh` (*in automated install*, `/opt/rchk/scripts/build_r.sh`)
2. Install and check the package
	* `echo 'install.packages("jpeg",repos="http://cloud.r-project.org")' |  ./bin/R --slave`
	* `../scripts/check_package.sh jpeg` (in VM install, `/opt/rchk/scripts/check_package.sh jpeg`)

The output of the checking is in files
`packages/lib/jpeg/libs/jpeg.so.*check`. For version 0.1-8 of the package,
`jpeg.so.maacheck` includes

```
WARNING Suspicious call (two or more unprotected arguments) to Rf_setAttrib at read_jpeg /rchk/trunk/packages/build/IsnsJjDm/jpeg/src/read.c:131
```

which is a true error. `bcheck` does not find any errors, `jpeg.so.bcheck`
only contains something like

```
Analyzed 15 functions, traversed 1938 states.
```

To check the next package, just follow the same steps, installing it into
this customized version of R.  When checking a tarball, one would typically
first install the CRAN/BIOC version of the package to get all dependencies
in, and then use `R CMD INSTALL` to install the newest version to check from
the tarball.

Further information:

* [User documentation](doc/USAGE.md) - how to use the tools and what they check.
* [Internals](doc/INTERNALS.md) - how the tools work internally.
* [Building](doc/BUILDING.md) - how to get the necessary bitcode files for R/packages; this is now encapsulated in scripts, but the background is here
