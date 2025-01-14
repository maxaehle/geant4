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
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4EmCorrections
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 13.01.2005
//
// Modifications:
// 28.04.2006 General cleanup, add finite size corrections (V.Ivanchenko)
// 13.05.2006 Add corrections for ion stopping (V.Ivanhcenko)
// 20.05.2008 Removed Finite Size correction (V.Ivanchenko)
// 12.09.2008 Added inlined interfaces to effective charge (V.Ivanchenko)
// 19.04.2012 Fix reproducibility problem (A.Ribon)
//
// Class Description:
//
// This class provides calculation of EM corrections to ionisation
//

// -------------------------------------------------------------------
//

#ifndef G4EmCorrections_h
#define G4EmCorrections_h 1

#include <CLHEP/Units/PhysicalConstants.h>

#include "globals.hh"
#include "G4ionEffectiveCharge.hh"
#include "G4Material.hh"
#include "G4ParticleDefinition.hh"
#include "G4Threading.hh"

class G4VEmModel;
class G4PhysicsVector;
class G4IonTable;
class G4MaterialCutsCouple;
class G4PhysicsFreeVector;
class G4Pow;

class G4EmCorrections
{

public:

  explicit G4EmCorrections(G4int verb);

  ~G4EmCorrections();

  G4double HighOrderCorrections(const G4ParticleDefinition*,
                                const G4Material*,
                                G4double kineticEnergy,
                                G4double cutEnergy);

  G4double IonHighOrderCorrections(const G4ParticleDefinition*,
                                   const G4MaterialCutsCouple*,
                                   G4double kineticEnergy);

  G4double ComputeIonCorrections(const G4ParticleDefinition*,
                                 const G4Material*,
                                 G4double kineticEnergy);

  G4double IonBarkasCorrection(const G4ParticleDefinition*,
                               const G4Material*,
                               G4double kineticEnergy);

  G4double Bethe(const G4ParticleDefinition*,
                 const G4Material*,
                 G4double kineticEnergy);

  G4double SpinCorrection(const G4ParticleDefinition*,
                          const G4Material*,
                          G4double kineticEnergy);

  G4double KShellCorrection(const G4ParticleDefinition*,
                            const G4Material*,
                            G4double kineticEnergy);

  G4double LShellCorrection(const G4ParticleDefinition*,
                            const G4Material*,
                            G4double kineticEnergy);

  G4double ShellCorrection(const G4ParticleDefinition*,
                           const G4Material*,
                           G4double kineticEnergy);

  G4double ShellCorrectionSTD(const G4ParticleDefinition*,
                              const G4Material*,
                              G4double kineticEnergy);

  G4double DensityCorrection(const G4ParticleDefinition*,
                             const G4Material*,
                             G4double kineticEnergy);

  G4double BarkasCorrection(const G4ParticleDefinition*,
                            const G4Material*,
                            G4double kineticEnergy);

  G4double BlochCorrection(const G4ParticleDefinition*,
                           const G4Material*,
                           G4double kineticEnergy);

  G4double MottCorrection(const G4ParticleDefinition*,
                          const G4Material*,
                          G4double kineticEnergy);

  void AddStoppingData(G4int Z, G4int A, const G4String& materialName,
                       G4PhysicsVector* dVector);

  void InitialiseForNewRun();

  // effective charge correction using stopping power data
  G4double EffectiveChargeCorrection(const G4ParticleDefinition*,
                                     const G4Material*,
                                     G4double kineticEnergy);

  // effective charge of an ion
  inline G4double GetParticleCharge(const G4ParticleDefinition*,
                                    const G4Material*,
                                    G4double kineticEnergy);

  inline
  G4double EffectiveChargeSquareRatio(const G4ParticleDefinition*,
                                      const G4Material*,
                                      G4double kineticEnergy);

  // ionisation models for ions
  inline void SetIonisationModels(G4VEmModel* mod1 = nullptr, 
                                  G4VEmModel* mod2 = nullptr);

  inline G4int GetNumberOfStoppingVectors() const;

  inline void SetVerbose(G4int verb);

  // hide assignment operator
  G4EmCorrections & operator=(const G4EmCorrections &right) = delete;
  G4EmCorrections(const G4EmCorrections&) = delete;

private:

  void Initialise();

  void BuildCorrectionVector();

  void SetupKinematics(const G4ParticleDefinition*,
                       const G4Material*,
                       G4double kineticEnergy);

  G4double KShell(G4double theta, G4double eta);

  G4double LShell(G4double theta, G4double eta);

  G4int Index(G4double x, const G4double* y, G4int n) const;

  G4double Value(G4double xv, G4double x1, G4double x2, 
                 G4double y1, G4double y2) const;

  G4double Value2(G4double xv, G4double yv, G4double x1, G4double x2,
                  G4double y1, G4double y2, G4double z11, G4double z21, 
		  G4double z12, G4double z22) const;

  G4Pow* g4calc;
  G4IonTable* ionTable;

  const G4ParticleDefinition* particle = nullptr;
  const G4ParticleDefinition* curParticle = nullptr;
  const G4Material*           material = nullptr;
  const G4Material*           curMaterial = nullptr;
  const G4ElementVector*      theElementVector = nullptr;
  const G4double*             atomDensity = nullptr;

  G4PhysicsVector*            curVector = nullptr;

  G4VEmModel*  ionLEModel = nullptr;
  G4VEmModel*  ionHEModel = nullptr;

  G4double  kinEnergy = 0.0;
  G4double  mass = 0.0;
  G4double  massFactor = 1.0;
  G4double  eth;
  G4double  tau = 0.0;
  G4double  gamma = 1.0;
  G4double  bg2 = 0.0;
  G4double  beta2 = 0.0;
  G4double  beta = 0.0;
  G4double  ba2 = 0.0;
  G4double  tmax = 0.0;
  G4double  charge = 0.0;
  G4double  q2 = 0.0;
  G4double  eCorrMin;
  G4double  eCorrMax;

  G4bool    isMaster = false;

  size_t    ncouples = 0;
  G4int     verbose;

  G4int     nK;
  G4int     nL;
  G4int     nEtaK;
  G4int     nEtaL;

  G4int     nbinCorr = 52;
  G4int     numberOfElements = 0;

  // Ion stopping data
  G4int     nIons = 0;
  G4int     idx = 0;
  G4int     currentZ = 0;

  std::vector<G4int>          Zion;
  std::vector<G4int>          Aion;
  std::vector<G4String>       materialName;

  std::vector<const G4ParticleDefinition*> ionList;

  std::map< G4int, std::vector<G4double> > thcorr;

  std::vector<const G4Material*> currmat;
  std::vector<const G4Material*> materialList;
  std::vector<G4PhysicsVector*>  stopData;

  G4ionEffectiveCharge  effCharge;

  static const G4double ZD[11];
  static const G4double UK[20];
  static const G4double VK[20];
  static G4double ZK[20];
  static const G4double Eta[29];
  static G4double CK[20][29];
  static G4double CL[26][28];
  static const G4double UL[26];
  static G4double VL[26];

  static G4PhysicsFreeVector* sBarkasCorr;
  static G4PhysicsFreeVector* sThetaK;
  static G4PhysicsFreeVector* sThetaL;

#ifdef G4MULTITHREADED
  static G4Mutex theCorrMutex;
#endif
};

inline G4int 
G4EmCorrections::Index(G4double x, const G4double* y, G4int n) const
{
  G4int iddd = n-1;
  // Loop checking, 03-Aug-2015, Vladimir Ivanchenko
  do {--iddd;} while (iddd>0 && x<y[iddd]);
  return iddd;
}

inline G4double G4EmCorrections::Value(G4double xv, G4double x1, G4double x2,
                                       G4double y1, G4double y2) const
{
  return y1 + (y2 - y1)*(xv - x1)/(x2 - x1);
}

inline G4double G4EmCorrections::Value2(G4double xv, G4double yv, 
                                        G4double x1, G4double x2,
                                        G4double y1, G4double y2,
                                        G4double z11, G4double z21, 
                                        G4double z12, G4double z22) const
{
  return ( z11*(x2-xv)*(y2-yv) + z22*(xv-x1)*(yv-y1) +
	   z12*(x2-xv)*(yv-y1) + z21*(xv-x1)*(y2-yv) )
         / ((x2-x1)*(y2-y1));
}

inline void 
G4EmCorrections::SetIonisationModels(G4VEmModel* mod1, G4VEmModel* mod2)
{
  if(nullptr != mod1) { ionLEModel = mod1; }
  if(nullptr != mod2) { ionHEModel = mod2; }
}

inline G4int G4EmCorrections::GetNumberOfStoppingVectors() const
{
  return nIons;
}

inline G4double 
G4EmCorrections::GetParticleCharge(const G4ParticleDefinition* p,
                                   const G4Material* mat,
                                   G4double kineticEnergy)
{
  return effCharge.EffectiveCharge(p,mat,kineticEnergy);
}

inline G4double 
G4EmCorrections::EffectiveChargeSquareRatio(const G4ParticleDefinition* p,
                                            const G4Material* mat,
                                            G4double kineticEnergy)
{
  return effCharge.EffectiveChargeSquareRatio(p,mat,kineticEnergy);
}

inline void G4EmCorrections::SetVerbose(G4int verb)
{
  verbose = verb;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
