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
// G4VTransitionRadiation  -- header file
//
// Generic process of transition radiation
//
// History:
// 29.02.04, V.Ivanchenko created
// 28.07.05, P.Gumplinger add G4ProcessType to constructor

#ifndef G4VTransitionRadiation_h
#define G4VTransitionRadiation_h

#include "globals.hh"
#include "G4ForceCondition.hh"
#include "G4ParticleDefinition.hh"
#include "G4Region.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VDiscreteProcess.hh"
#include "G4VParticleChange.hh"

#include <vector>

class G4LossTableManager;
class G4Material;
class G4VTRModel;

class G4VTransitionRadiation : public G4VDiscreteProcess
{
 public:
  // Constructors
  explicit G4VTransitionRadiation(const G4String& processName = "TR",
                                  G4ProcessType type = fElectromagnetic);

  // Destructor
  virtual ~G4VTransitionRadiation();

  virtual G4bool IsApplicable(
    const G4ParticleDefinition& aParticleType) override;

  void ProcessDescription(std::ostream&) const override;
  void DumpInfo() const override { ProcessDescription(G4cout); };

  virtual G4double GetMeanFreePath(const G4Track& track, G4double,
                                   G4ForceCondition* condition) override;

  virtual G4VParticleChange* PostStepDoIt(const G4Track& track,
                                          const G4Step& step) override;

  void SetRegion(const G4Region* reg);

  void SetModel(G4VTRModel* m);

  void Clear();

  // hide assignment operator
  G4VTransitionRadiation& operator=(const G4VTransitionRadiation& right) =
    delete;
  G4VTransitionRadiation(const G4VTransitionRadiation&) = delete;

 private:
  G4LossTableManager* theManager;
  const G4Region* region;
  G4VTRModel* model;

  std::vector<const G4Material*> materials;
  std::vector<G4double> steps;
  std::vector<G4ThreeVector> normals;

  G4ThreeVector startingPosition;
  G4ThreeVector startingDirection;

  G4double gammaMin;
  G4double cosDThetaMax;

  G4int nSteps;
};

#endif  // G4VTransitionRadiation_h
