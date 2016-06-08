// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Fancy3DNucleus.hh,v 1.6 1999/12/15 14:52:50 gunter Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
#ifndef G4Fancy3DNucleus_h
#define G4Fancy3DNucleus_h 1

// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      For information related to this code contact:
//      CERN, CN Division, ASD group
//      ---------------- G4Fancy3DNucleus ----------------
//             by Gunter Folger, May 1998.
//       class for a 3D nucleus, arranging nucleons in space and momentum.
// ------------------------------------------------------------

#include "globals.hh"
#include "G4DynamicParticle.hh"
#include "G4V3DNucleus.hh"
#include "G4Nucleon.hh"
#include "G4VNuclearDensity.hh"
#include "G4FermiMomentum.hh"
#include "g4rw/tpordvec.h"

class G4Fancy3DNucleus : public G4V3DNucleus
{

  public:
      G4Fancy3DNucleus();
      ~G4Fancy3DNucleus();

  private:
      G4Fancy3DNucleus(const G4Fancy3DNucleus &right);
      const G4Fancy3DNucleus & operator=(const G4Fancy3DNucleus &right);
      int operator==(const G4Fancy3DNucleus &right) const;
      int operator!=(const G4Fancy3DNucleus &right) const;
      

//  Implementation 
      void ChooseNucleons();
      void ChoosePositions();
      void ChooseFermiMomenta();
      G4double BindingEnergy();
      G4bool ReduceSum(G4ThreeVector * momentum, G4double *);

  public:
      void Init(G4double theA, G4double theZ);
      G4bool StartLoop();
      G4Nucleon * GetNextNucleon();
      const G4RWTPtrOrderedVector<G4Nucleon> & GetNucleons();
      G4int GetMassNumber();
      G4double GetMass();
      G4int GetCharge();
      G4double GetNuclearRadius();
      G4double GetNuclearRadius(const G4double maxRelativeDensity);
      G4double GetOuterRadius();
      void DoLorentzBoost(const G4LorentzVector & theBoost);
      void DoLorentzBoost(const G4ThreeVector & theBeta);
      void DoLorentzContraction(const G4LorentzVector & theBoost);
      void DoLorentzContraction(const G4ThreeVector & theBeta);
      void CenterNucleons();
      void DoTranslation(const G4ThreeVector & theShift);
      
  private:
  
  G4int myA;
  G4int myZ;
  G4Nucleon * theNucleons;
  G4RWTPtrOrderedVector<G4Nucleon> theRWNucleons;  // should not have two...
  G4int currentNucleon;
  G4VNuclearDensity * theDensity;
  G4FermiMomentum theFermi;  
  const G4double nucleondistance;
  
};

inline G4int G4Fancy3DNucleus::GetCharge()
{
	return myZ;
}

inline G4int G4Fancy3DNucleus::GetMassNumber()
{
	return myA;
}

#endif

