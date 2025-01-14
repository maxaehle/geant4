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
// G4ParticleChangeForLoss class implementation
//
// Author: Hisaya Kurashige, 23 March 1998  
// Revision: Vladimir Ivantchenko, 16 January 2004
// --------------------------------------------------------------------

#include "G4ParticleChangeForLoss.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4DynamicParticle.hh"
#include "G4ExceptionSeverity.hh"

// --------------------------------------------------------------------
G4ParticleChangeForLoss::G4ParticleChangeForLoss()
  : G4VParticleChange()
  , lowEnergyLimit(1.0 * eV)
{
  theSteppingControlFlag = NormalCondition;
  // Disable flag that is enabled in G4VParticleChange if G4VERBOSE.
  debugFlag              = false;
}

// --------------------------------------------------------------------
G4ParticleChangeForLoss::~G4ParticleChangeForLoss()
{
}

// --------------------------------------------------------------------
G4ParticleChangeForLoss::G4ParticleChangeForLoss(
  const G4ParticleChangeForLoss& right)
  : G4VParticleChange(right)
{
  currentTrack              = right.currentTrack;
  proposedKinEnergy         = right.proposedKinEnergy;
  lowEnergyLimit            = right.lowEnergyLimit;
  currentCharge             = right.currentCharge;
  proposedMomentumDirection = right.proposedMomentumDirection;
}

// --------------------------------------------------------------------
G4ParticleChangeForLoss& G4ParticleChangeForLoss::operator=(
  const G4ParticleChangeForLoss& right)
{
  if(this != &right)
  {
    if(theNumberOfSecondaries > 0)
    {
      for(G4int index = 0; index < theNumberOfSecondaries; ++index)
      {
        if((*theListOfSecondaries)[index])
          delete(*theListOfSecondaries)[index];
      }
    }
    delete theListOfSecondaries;
    theListOfSecondaries   = new G4TrackFastVector();
    theNumberOfSecondaries = right.theNumberOfSecondaries;
    for(G4int index = 0; index < theNumberOfSecondaries; ++index)
    {
      G4Track* newTrack = new G4Track(*((*right.theListOfSecondaries)[index]));
      theListOfSecondaries->SetElement(index, newTrack);
    }

    theStatusChange              = right.theStatusChange;
    theLocalEnergyDeposit        = right.theLocalEnergyDeposit;
    theSteppingControlFlag       = right.theSteppingControlFlag;
    theParentWeight              = right.theParentWeight;
    isParentWeightProposed       = right.isParentWeightProposed;
    fSetSecondaryWeightByProcess = right.fSetSecondaryWeightByProcess;

    currentTrack              = right.currentTrack;
    proposedKinEnergy         = right.proposedKinEnergy;
    currentCharge             = right.currentCharge;
    proposedMomentumDirection = right.proposedMomentumDirection;
  }
  return *this;
}

// --------------------------------------------------------------------
void G4ParticleChangeForLoss::DumpInfo() const
{
  // use base-class DumpInfo
  G4VParticleChange::DumpInfo();

  G4int oldprc = G4cout.precision(3);
  G4cout << "        Charge (eplus)   : " << std::setw(20)
         << currentCharge / eplus << G4endl;
  G4cout << "        Kinetic Energy (MeV): " << std::setw(20)
         << proposedKinEnergy / MeV << G4endl;
  G4cout << "        Momentum Direct - x : " << std::setw(20)
         << proposedMomentumDirection.x() << G4endl;
  G4cout << "        Momentum Direct - y : " << std::setw(20)
         << proposedMomentumDirection.y() << G4endl;
  G4cout << "        Momentum Direct - z : " << std::setw(20)
         << proposedMomentumDirection.z() << G4endl;
  G4cout.precision(oldprc);
}

// --------------------------------------------------------------------
G4bool G4ParticleChangeForLoss::CheckIt(const G4Track& aTrack)
{
  G4bool itsOK         = true;
  G4bool exitWithError = false;

  G4double accuracy;

  // Energy should not be lager than initial value
  accuracy = (proposedKinEnergy - aTrack.GetKineticEnergy()) / MeV;
  if(accuracy > accuracyForWarning)
  {
    itsOK         = false;
    exitWithError = (accuracy > accuracyForException);
#ifdef G4VERBOSE
    G4cout << "G4ParticleChangeForLoss::CheckIt: ";
    G4cout << "KinEnergy become larger than the initial value!"
           << "  Difference:  " << accuracy << "[MeV] " << G4endl;
    G4cout << aTrack.GetDefinition()->GetParticleName()
           << " E=" << aTrack.GetKineticEnergy() / MeV
           << " pos=" << aTrack.GetPosition().x() / m << ", "
           << aTrack.GetPosition().y() / m << ", "
           << aTrack.GetPosition().z() / m << G4endl;
#endif
  }

  // dump out information of this particle change
#ifdef G4VERBOSE
  if(!itsOK) { DumpInfo(); }
#endif

  // exit with error
  if(exitWithError)
  {
    G4Exception("G4ParticleChangeForLoss::CheckIt()", "TRACK004",
                EventMustBeAborted, "energy was  illegal");
  }

  // correction
  if(!itsOK)
  {
    proposedKinEnergy = aTrack.GetKineticEnergy();
  }

  itsOK = (itsOK) && G4VParticleChange::CheckIt(aTrack);
  return itsOK;
}

// --------------------------------------------------------------------
G4Step* G4ParticleChangeForLoss::UpdateStepForAlongStep(G4Step* pStep)
{
  G4StepPoint* pPostStepPoint = pStep->GetPostStepPoint();
  G4StepPoint* pPreStepPoint  = pStep->GetPreStepPoint();
  G4Track* pTrack             = pStep->GetTrack();

  // accumulate change of the kinetic energy
  G4double preKinEnergy = pPreStepPoint->GetKineticEnergy();
  G4double kinEnergy =
    pPostStepPoint->GetKineticEnergy() + (proposedKinEnergy - preKinEnergy);

  // update kinetic energy and charge
  if(kinEnergy < lowEnergyLimit)
  {
    theLocalEnergyDeposit += kinEnergy;
    kinEnergy = 0.0;
    pPostStepPoint->SetVelocity(0.0);
  }
  else
  {
    pPostStepPoint->SetCharge(currentCharge);
    // calculate velocity
    pTrack->SetKineticEnergy(kinEnergy);
    pPostStepPoint->SetVelocity(pTrack->CalculateVelocity());
    pTrack->SetKineticEnergy(preKinEnergy);
  }
  pPostStepPoint->SetKineticEnergy(kinEnergy);

  if(isParentWeightProposed)
  {
    pPostStepPoint->SetWeight(theParentWeight);
  }

  pStep->AddTotalEnergyDeposit(theLocalEnergyDeposit);
  pStep->AddNonIonizingEnergyDeposit(theNonIonizingEnergyDeposit);
  return pStep;
}

// --------------------------------------------------------------------
G4Step* G4ParticleChangeForLoss::UpdateStepForPostStep(G4Step* pStep)
{
  G4StepPoint* pPostStepPoint = pStep->GetPostStepPoint();
  G4Track* pTrack             = pStep->GetTrack();

  pPostStepPoint->SetCharge(currentCharge);
  pPostStepPoint->SetMomentumDirection(proposedMomentumDirection);
  pPostStepPoint->SetKineticEnergy(proposedKinEnergy);
  pTrack->SetKineticEnergy(proposedKinEnergy);
  if(proposedKinEnergy > 0.0)
  {
    pPostStepPoint->SetVelocity(pTrack->CalculateVelocity());
  }
  else
  {
    pPostStepPoint->SetVelocity(0.0);
  }
  pPostStepPoint->SetPolarization(proposedPolarization);

  if(isParentWeightProposed)
  {
    pPostStepPoint->SetWeight(theParentWeight);
  }

  pStep->AddTotalEnergyDeposit(theLocalEnergyDeposit);
  pStep->AddNonIonizingEnergyDeposit(theNonIonizingEnergyDeposit);
  return pStep;
}
