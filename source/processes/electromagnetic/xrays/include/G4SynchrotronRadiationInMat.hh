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
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History:
//      21-5-98  1 version , V. Grichine
//      28-05-01, V.Ivanchenko minor changes to provide ANSI -wall compilation
//      19-05-06, V.Ivanchenko rename from G4SynchrotronRadiation
//
// ------------------------------------------------------------

#ifndef G4SynchrotronRadiationInMat_h
#define G4SynchrotronRadiationInMat_h 1

#include "globals.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsTable.hh"
#include "G4Positron.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TransportationManager.hh"
#include "G4VDiscreteProcess.hh"
#include "G4VParticleChange.hh"

class G4ParticleDefinition;
class G4PropagatorInField;

class G4SynchrotronRadiationInMat : public G4VDiscreteProcess
{
 public:
  explicit G4SynchrotronRadiationInMat(
    const G4String& processName = "SynchrotronRadiation",
    G4ProcessType type          = fElectromagnetic);

  ~G4SynchrotronRadiationInMat();

  G4SynchrotronRadiationInMat& operator=(
    const G4SynchrotronRadiationInMat& right) = delete;
  G4SynchrotronRadiationInMat(const G4SynchrotronRadiationInMat&) = delete;

  G4double GetMeanFreePath(const G4Track& track, G4double previousStepSize,
                           G4ForceCondition* condition) override;

  G4VParticleChange* PostStepDoIt(const G4Track& track,
                                  const G4Step& Step) override;

  G4double GetPhotonEnergy(const G4Track& trackData, const G4Step& stepData);

  G4double GetRandomEnergySR(G4double, G4double);

  G4double GetProbSpectrumSRforInt(G4double);
  G4double GetIntProbSR(G4double);

  G4double GetProbSpectrumSRforEnergy(G4double);
  G4double GetEnergyProbSR(G4double);

  G4double GetIntegrandForAngleK(G4double);
  G4double GetAngleK(G4double);
  G4double GetAngleNumberAtGammaKsi(G4double);

  G4bool IsApplicable(const G4ParticleDefinition&) override;

  static G4double GetLambdaConst();
  static G4double GetEnergyConst();

  void SetRootNumber(G4int rn) { fRootNumber = rn; };
  void SetVerboseLevel(G4int v) { fVerboseLevel = v; };
  void SetKsi(G4double ksi) { fKsi = ksi; };
  void SetEta(G4double eta) { fEta = eta; };
  void SetPsiGamma(G4double psg) { fPsiGamma = psg; };
  void SetOrderAngleK(G4double ord)
  {
    fOrderAngleK = ord;
  };  // should be 1/3 or 2/3

 private:
  // Constant for calculation of mean free path
  // sqrt(3.) = 1.73...
  static constexpr G4double fLambdaConst =
    1.73205080756887729352 * CLHEP::electron_mass_c2 /
    (2.5 * CLHEP::fine_structure_const * CLHEP::eplus * ::CLHEP::c_light);

  // Constant for calculation of characterictic energy
  static constexpr G4double fEnergyConst =
    1.5 * CLHEP::c_light * CLHEP::c_light * CLHEP::eplus * CLHEP::hbar_Planck /
    CLHEP::electron_mass_c2;

  // Array of integral probability of synchrotron photons:
  // the corresponding energy = 0.0001*i*i*(characteristic energy)
  static const G4double fIntegralProbabilityOfSR[200]; 

  const G4ParticleDefinition* theGamma;
  const G4ParticleDefinition* theElectron;
  const G4ParticleDefinition* thePositron;

  G4PropagatorInField* fFieldPropagator;

  const G4double
    LowestKineticEnergy;  // low  energy limit of the cross-section formula

  G4double CutInRange;
  G4double GammaCutInKineticEnergyNow;
  G4double ElectronCutInKineticEnergyNow;
  G4double PositronCutInKineticEnergyNow;
  G4double ParticleCutInKineticEnergyNow;
  G4double fAlpha;
  G4double fKsi;          // omega/omega_c
  G4double fPsiGamma;     // Psi-angle*gamma
  G4double fEta;          //
  G4double fOrderAngleK;  // 1/3 or 2/3

  G4int fRootNumber;
  G4int fVerboseLevel;
  
  G4int secID = -1;  // creator modelID
};

#endif  // end of G4SynchrotronRadiationInMat.hh
