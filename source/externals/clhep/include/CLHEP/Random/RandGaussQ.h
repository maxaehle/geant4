#include "CommonHeader.h"

// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                          --- RandGaussQ ---
//                          class header file
// -----------------------------------------------------------------------

// Class defining methods RandGaussQ, which is derived from RandGauss.
// The user interface is identical; but RandGaussQ is faster and a bit less
// accurate.

// =======================================================================
// M. Fischler - Created: 24th Jan 2000
// M Fischler      - put and get to/from streams 12/10/04
//
// =======================================================================

#ifndef RandGaussQ_h
#define RandGaussQ_h 1

#include "CLHEP/Random/RandGauss.h"

namespace CLHEP {

/**
 * @author
 * @ingroup random
 */
class RandGaussQ : public RandGauss {

public:

  inline RandGaussQ ( HepRandomEngine& anEngine, G4double mean=0.0,
                                                G4double stdDev=1.0 );
  inline RandGaussQ ( HepRandomEngine* anEngine, G4double mean=0.0,
                                                G4double stdDev=1.0 );
  // These constructors should be used to instantiate a RandGaussQ
  // distribution object defining a local engine for it.
  // The static generator will be skipped using the non-static methods
  // defined below.
  // If the engine is passed by pointer the corresponding engine object
  // will be deleted by the RandGaussQ destructor.
  // If the engine is passed by reference the corresponding engine object
  // will not be deleted by the RandGaussQ destructor.

  // Destructor
  virtual ~RandGaussQ();

  //
  // Methods to generate Gaussian-distributed random deviates:
  //
  //   If a fast good engine takes 1 usec, RandGauss::fire() adds 1 usec while 
  //   RandGaussQ::fire() adds only .4 usec.
  //

  // Static methods to shoot random values using the static generator

  static  inline G4double shoot();

  static  inline G4double shoot( G4double mean, G4double stdDev );

  static  void shootArray ( const int size, G4double* vect,
                            G4double mean=0.0, G4double stdDev=1.0 );

  //  Static methods to shoot random values using a given engine
  //  by-passing the static generator.

  static  inline G4double shoot( HepRandomEngine* anotherEngine );

  static  inline G4double shoot( HepRandomEngine* anotherEngine, 
                                  G4double mean, G4double stdDev );


  static  void shootArray ( HepRandomEngine* anotherEngine, 
			    const int size,
                            G4double* vect, G4double mean=0.0,
                            G4double stdDev=1.0 );

  //  Instance methods using the localEngine to instead of the static 
  //  generator, and the default mean and stdDev established at construction

  inline G4double fire();

  inline G4double fire ( G4double mean, G4double stdDev );
  
  void fireArray  ( const int size, G4double* vect);
  void fireArray  ( const int size, G4double* vect,
                    G4double mean, G4double stdDev );

  virtual G4double operator()();
  virtual G4double operator()( G4double mean, G4double stdDev );

  // Save and restore to/from streams
  
  std::ostream & put ( std::ostream & os ) const;
  std::istream & get ( std::istream & is );

  std::string name() const;
  HepRandomEngine & engine();

  static std::string distributionName() {return "RandGaussQ";}  
  // Provides the name of this distribution class
  

protected:

  static G4double transformQuick (G4double r);
  static G4double transformSmall (G4double r);

private:

  // All the engine info, and the default mean and sigma, are in the RandGauss
  // base class.

};

}  // namespace CLHEP

#include "CLHEP/Random/RandGaussQ.icc"

#endif
