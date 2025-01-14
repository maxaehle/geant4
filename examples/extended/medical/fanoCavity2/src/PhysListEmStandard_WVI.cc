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
/// \file medical/fanoCavity2/src/PhysListEmStandard_WVI.cc
/// \brief Implementation of the PhysListEmStandard_WVI class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysListEmStandard_WVI.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4WentzelVIModel.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"

#include "G4eIonisation.hh"
#include "MyMollerBhabhaModel.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4hIonisation.hh"
#include "G4hMultipleScattering.hh"

#include "G4EmParameters.hh"
#include "G4MscStepLimitType.hh"

#include "G4BuilderType.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandard_WVI::PhysListEmStandard_WVI(const G4String& name)
: G4VPhysicsConstructor(name)
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(1);
  param->SetMinEnergy(10*eV);
  param->SetMaxEnergy(10*GeV);
  param->SetNumberOfBinsPerDecade(20);
  param->SetLowestElectronEnergy(100*eV);
  param->SetUseMottCorrection(true);
  param->SetStepFunction(0.2, 100*um);
  param->SetBuildCSDARange(true);
  param->SetMaxEnergyForCSDARange(10*GeV);
  param->SetMscThetaLimit(0.15);
  SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandard_WVI::~PhysListEmStandard_WVI()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmStandard_WVI::ConstructProcess()
{
  // Add standard EM Processes
  //

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {
      // gamma
            
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);
      
    } else if (particleName == "e-") {
      //electron

      G4eMultipleScattering* eMsc = new G4eMultipleScattering();
      eMsc->SetEmModel(new G4WentzelVIModel()); 
      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetEmModel(new MyMollerBhabhaModel);

      G4CoulombScattering* cs = new G4CoulombScattering();
      G4eCoulombScatteringModel* single = new G4eCoulombScatteringModel();
      cs->SetEmModel(single);
                         
      pmanager->AddProcess(eMsc,   -1, 1,-1);
      pmanager->AddProcess(eIoni,  -1, 2, 1);
      pmanager->AddProcess(cs,     -1,-1, 2);
                  
    } else if (particleName == "e+") {
      //positron

      G4eMultipleScattering* pMsc = new G4eMultipleScattering();
      pMsc->SetEmModel(new G4WentzelVIModel());
      G4eIonisation* pIoni = new G4eIonisation();
      pIoni->SetEmModel(new MyMollerBhabhaModel);
                               
      pmanager->AddProcess(pMsc,                      -1, 1,-1);
      pmanager->AddProcess(pIoni,                     -1, 2, 1);
      pmanager->AddProcess(new G4eplusAnnihilation,    0,-1, 2);
      pmanager->AddProcess(new G4CoulombScattering,   -1,-1, 3);
                   
    } else if( particleName == "proton" ) {
      //proton
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      msc->SetEmModel(new G4WentzelVIModel());
      pmanager->AddProcess(msc,                       -1, 1,-1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 1);
      pmanager->AddProcess(new G4CoulombScattering,   -1,-1, 2);      
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

