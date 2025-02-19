#include "CommonHeader.h"

// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                       --- RandExponential ---
//                      class implementation file
// -----------------------------------------------------------------------
// This file is part of Geant4 (simulation toolkit for HEP).

// =======================================================================
// Gabriele Cosmo - Created: 17th May 1996
//                - Added methods to shoot arrays: 28th July 1997
// J.Marraffino   - Added default mean as attribute and
//                  operator() with mean: 16th Feb 1998
// M Fischler      - put and get to/from streams 12/15/04
// M Fischler	      - put/get to/from streams uses pairs of ulongs when
//			+ storing G4doubles avoid problems with precision 
//			4/14/05
// =======================================================================

#include "CLHEP/Random/RandExponential.h"
#include "CLHEP/Random/DoubConv.h"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace CLHEP {

std::string RandExponential::name() const {return "RandExponential";}
HepRandomEngine & RandExponential::engine() {return *localEngine;}

RandExponential::~RandExponential() {
}

G4double RandExponential::operator()() {
  return fire( defaultMean );
}

G4double RandExponential::operator()( G4double mean ) {
  return fire( mean );
}

G4double RandExponential::shoot() {
  return -std::log(HepRandom::getTheEngine()->flat());
}

G4double RandExponential::shoot(G4double mean) {
  return -std::log(HepRandom::getTheEngine()->flat())*mean;
}

void RandExponential::shootArray( const int size, G4double* vect,
                                  G4double mean )
{
  for( G4double* v = vect; v != vect+size; ++v )
    *v = shoot(mean);
}

void RandExponential::shootArray(HepRandomEngine* anEngine, const int size,
                                   G4double* vect, G4double mean )
{
  for( G4double* v = vect; v != vect+size; ++v )
    *v = shoot(anEngine, mean);
}

void RandExponential::fireArray( const int size, G4double* vect)
{
  for( G4double* v = vect; v != vect+size; ++v )
    *v = fire( defaultMean );
}

void RandExponential::fireArray( const int size, G4double* vect,
                                 G4double mean )
{
  for( G4double* v = vect; v != vect+size; ++v )
    *v = fire( mean );
}

std::ostream & RandExponential::put ( std::ostream & os ) const {
  int pr=os.precision(20);
  std::vector<unsigned long> t(2);
  os << " " << name() << "\n";
  os << "Uvec" << "\n";
  t = DoubConv::dto2longs(defaultMean);
  os << defaultMean << " " << t[0] << " " << t[1] << "\n";
  os.precision(pr);
  return os;
}

std::istream & RandExponential::get ( std::istream & is ) {
  std::string inName;
  is >> inName;
  if (inName != name()) {
    is.clear(std::ios::badbit | is.rdstate());
    std::cerr << "Mismatch when expecting to read state of a "
    	      << name() << " distribution\n"
	      << "Name found was " << inName
	      << "\nistream is left in the badbit state\n";
    return is;
  }
  if (possibleKeywordInput(is, "Uvec", defaultMean)) {
    std::vector<unsigned long> t(2);
    is >> defaultMean >> t[0] >> t[1]; defaultMean = DoubConv::longs2G4double(t); 
    return is;
  }
  // is >> defaultMean encompassed by possibleKeywordInput
  return is;
}


}  // namespace CLHEP
