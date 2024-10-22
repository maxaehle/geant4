#include "CommonHeader.h"

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// Author: Luciano Pandola
//
// History:
// -----------
// 13 Jan 2010   L. Pandola  First implementation
// 24 May 2011   L. Pandola  Renamed (make v2008 as default Penelope)
// 18 Sep 2013   L. Pandola  Migration to MT paradigm
// 04 Mar 2021   L. Pandola  Replace map with static array
//
// -------------------------------------------------------------------
//
// Class description:
// Low Energy Electromagnetic Physics, Gamma Conversion 
// with Penelope Model, version 2008
// -------------------------------------------------------------------

#ifndef G4PENELOPEGAMMACONVERSIONMODEL_HH
#define G4PENELOPEGAMMACONVERSIONMODEL_HH 1

#include "globals.hh"
#include "G4VEmModel.hh"
#include "G4DataVector.hh"
#include "G4ParticleChangeForGamma.hh"

class G4ParticleDefinition;
class G4DynamicParticle;
class G4MaterialCutsCouple;
class G4Material;
class G4PhysicsFreeVector;

class G4PenelopeGammaConversionModel : public G4VEmModel 
{
public:
  explicit G4PenelopeGammaConversionModel(const G4ParticleDefinition* p=nullptr,
					  const G4String& processName ="PenConversion");
  
  virtual ~G4PenelopeGammaConversionModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&) override;
  void InitialiseLocal(const G4ParticleDefinition*, G4VEmModel*) override; 
  G4double ComputeCrossSectionPerAtom(
				      const G4ParticleDefinition*,
				      G4double kinEnergy,
				      G4double Z,
				      G4double A=0,
				      G4double cut=0,
				      G4double emax=DBL_MAX) override;

  void SampleSecondaries(std::vector<G4DynamicParticle*>*,
			 const G4MaterialCutsCouple*,
			 const G4DynamicParticle*,
			 G4double tmin,
			 G4double maxEnergy) override;
  
  void SetVerbosityLevel(G4int lev){fVerboseLevel = lev;};
  G4int GetVerbosityLevel(){return fVerboseLevel;};

  G4PenelopeGammaConversionModel & operator=(const 
					     G4PenelopeGammaConversionModel &right) = delete;
  G4PenelopeGammaConversionModel(const G4PenelopeGammaConversionModel&) = delete;

protected:
  G4ParticleChangeForGamma* fParticleChange;
  const G4ParticleDefinition* fParticle;

private:
  void SetParticle(const G4ParticleDefinition*);
  void ReadDataFile(const G4int Z);
  void InitializeScreeningFunctions(const G4Material*);
  std::pair<G4double,G4double> GetScreeningFunctions(G4double);	

  //Effective (scalar) properties attached to materials:
  // effective charge
  std::map<const G4Material*,G4double> *fEffectiveCharge;
  // 2/Rs (Rs = screening radius), BCB array in Penelope
  std::map<const G4Material*,G4double> *fMaterialInvScreeningRadius;
  // Parameters of screening functions
  std::map<const G4Material*,std::pair<G4double,G4double> > *fScreeningFunction;

  static const G4int fMaxZ =99;
  static G4PhysicsFreeVector* fLogAtomicCrossSection[fMaxZ+1];
  //Z range is 1-99
  static G4double fAtomicScreeningRadius[fMaxZ+1];

  //Intrinsic energy limits of the model: cannot be extended by the parent process
  G4double fIntrinsicLowEnergyLimit;
  G4double fIntrinsicHighEnergyLimit;

  //Use a quicker sampling algorithm if E < smallEnergy
  G4double fSmallEnergy;  
  G4int fVerboseLevel;
  G4bool fIsInitialised;

  //Used only for G4EmCalculator and Unit Tests
  G4bool fLocalTable;
};



#endif
