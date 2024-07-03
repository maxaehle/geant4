cd $ROOTDIR
git clone https://github.com/SciCompKL/easyAD.git
cd easyAD
git checkout master

cd $ROOTDIR
mkdir clhep clhep/build clhep/install
cd clhep
git clone https://github.com/SciCompKL/clhep.git src
cd src
git checkout with_easyad

cd $ROOTDIR
mkdir geant4 geant4/build geant4/install
cd geant4
git clone https://github.com/SciCompKL/geant4.git src
cd src
git checkout with_easyad

cd $ROOTDIR
mkdir g4hepem g4hepem/build g4hepem/install
cd g4hepem
git clone https://github.com/SciCompKL/g4hepem.git src
cd src
git checkout with_easyad

cd $ROOTDIR/g4hepem/src/apps/examples/TestEm3
mkdir build

