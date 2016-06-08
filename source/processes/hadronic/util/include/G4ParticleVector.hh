// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ParticleVector.hh,v 1.3 1999/12/15 14:53:40 gunter Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// ------------------------------------------------------------
// HPW decoupling theo models from RW (Mon Mar 16 1998)
// ------------------------------------------------------------

#ifndef G4ParticleVector_h
#define G4ParticleVector_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4DynamicParticle.hh"
#include "g4rw/tpordvec.h"

// #ifdef STL
// for future use STL vector as container 
// typedef Vector<G4DynamicParticle> G4ParticleVector;
// #elseifdef RWT

typedef G4RWTPtrOrderedVector<G4DynamicParticle> G4ParticleVector;

#endif