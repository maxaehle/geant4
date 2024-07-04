*This README is about a version of Geant4 enabled for operator-overloading
algorithmic differentiation; see [README.rst](README.rst) for information 
about Geant4.*

# Differentiated Geant4

The [Geant4](https://geant4.web.cern.ch/) toolkit for the simulation of the passage
of particles through matter is widely used in high energy physics and many other 
areas. Being able to compute derivatives of user-defined outputs of Geant4 applications 
with respect to user-defined inputs would facilitate many interesting application, like
gradient-based design optimization studies of particles detectors. To this end, we created
this **work-in-progress prototype of a version of Geant4 enabled for forward-mode algorithmic differentiation (AD)**, based on Geant4 version 11.0.4. Feel free to take a look on our [preprint](https://arxiv.org/abs/2407.02966) for more information.

## Build instructions
1. Make sure that your system is able to build the original Geant4. According to their [documentation of prerequisites](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/gettingstarted.html), this basically means that you need a recent C++ compiler and CMake. It might also be advisable to have expat installed.  
  Things are more robust if you do not already have Geant4 installed to system directories or into your present anaconda environment or anything, because this version might later conflict with our AD-differentiated version.
2. You will need to download the [AD tool](https://gitlab.rhrk.uni-kl.de/aehle/easyAD), and to download, configure and build the differentiated versions of CLHEP, Geant4, and ideally also the G4HepEm toolkit and example application. To that end, you may cd into an empty directory, download [build.sh](setup/build.sh) (set execute permission, and adapt the number of build threads and build type if you like), [download.sh](setup/download.sh) (set execute permission), [env-build.sh](setup/env-build.sh), [env-run.sh](setup/env-run.sh), and run the following commands:
```bash
export ROOTDIR=$PWD
./download.sh
source env-build.sh
./build.sh
```
3. Whenever you want to use the differentiated version of Geant4, `source env-build.sh` and `source env-run.sh`. You should now be able to run the example application `g4hepem/src/apps/examples/TestEm3/build/TestEm3` as
```bash
./TestEm3 -m ../ATLASbar.mac
```

**Comments**:
- We have mainly tested this with GCC on Ubuntu. Compilation on MacOS with the LLVM implementation of the C++ standard library (libc++) led to compiler errors, partly because we are using `std::complex` beyond what is guaranteed by the C++ language standard. 
- On some systems, the `lib` directory in installation directories must be replaced by `lib64` in the build scripts.
- This is an **early-stage work-in-progress prototype** and we expect to improve the user experience in the future. You are nevertheless encouraged to try this out already now, if you like, and let us know about your observations. We are happy to collaborate on applications of Geant4 derivatives.

