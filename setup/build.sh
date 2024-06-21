cd $ROOTDIR/clhep/build/
cmake -DCMAKE_INSTALL_PREFIX=$PWD/../install -DCMAKE_BUILD_TYPE=Release ../src -DCMAKE_CXX_STANDARD=17

cd $ROOTDIR/clhep/build/
make install -j80

cd $ROOTDIR/geant4/build/
cmake -DCMAKE_INSTALL_PREFIX=$PWD/../install ../src -DCMAKE_BUILD_TYPE=Release -DGEANT4_INSTALL_DATA=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCLHEP_ROOT_DIR=$PWD/../../clhep/install

cd $ROOTDIR/geant4/build/
make install -j80

cd $ROOTDIR/g4hepem/build/
cmake ../src -DGeant4_DIR=$PWD/../../geant4/install/lib/Geant4-11.0.4 -DCMAKE_INSTALL_PREFIX=$PWD/../install -DCMAKE_BUILD_TYPE=Release -DEASYAD_FORWARD=yes

cd $ROOTDIR/g4hepem/build/
make install -j80

cd $ROOTDIR/g4hepem/src/apps/examples/TestEm3/build
cmake ../ -DGeant4_DIR=$ROOTDIR/geant4/install/lib/Geant4-11.0.4 -DG4HepEm_DIR=$ROOTDIR/g4hepem/install/lib/cmake/G4HepEm/ -DCMAKE_BUILD_TYPE=Release

cd $ROOTDIR/g4hepem/src/apps/examples/TestEm3/build
make -j80


