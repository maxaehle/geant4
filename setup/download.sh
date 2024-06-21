cd $ROOTDIR
git clone https://gitlab.rhrk.uni-kl.de/aehle/easyAD.git
cd easyAD
git checkout work

cd $ROOTDIR
mkdir clhep clhep/build clhep/install
cd clhep
git clone https://gitlab.rhrk.uni-kl.de/aehle/clhep.git src
cd src
git checkout ad

cd $ROOTDIR
mkdir geant4 geant4/build geant4/install
cd geant4
git clone https://gitlab.rhrk.uni-kl.de/aehle/geant4.git src
cd src
git checkout fix_types_2

cd $ROOTDIR
mkdir g4hepem g4hepem/build g4hepem/install
cd g4hepem
git clone https://gitlab.rhrk.uni-kl.de/aehle/g4hepem.git src
cd src
git checkout easyAD_geant4

cd $ROOTDIR/g4hepem/src/apps/examples/TestEm3
mkdir build

