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
// Authors: Francesco Longo, franzlongo1969@gmail.com
//
// Code based on the hadrontherapy && radioprotection advanced example

#include "GammaRayTelPhysicsList.hh"
#include "GammaRayTelPhysicsListMessenger.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"

// Physic lists (contained inside the Geant4 distribution)
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh" // to treat the new polarised process 
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh" // 

#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4Decay.hh"
#include "G4StepLimiter.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessManager.hh"

#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4RadioactiveDecayPhysics.hh"

/////////////////////////////////////////////////////////////////////////////
GammaRayTelPhysicsList::GammaRayTelPhysicsList() : G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 100*micrometer;
  SetVerboseLevel(1); 

  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 1*GeV);
  SetDefaultCutValue(defaultCutValue);
  DumpCutValuesTable();

  helIsRegisted  = false;
  bicIsRegisted  = false;
  biciIsRegisted = false;
  locIonIonInelasticIsRegistered = false;
  radioactiveDecayIsRegisted = false;

  pMessenger = new GammaRayTelPhysicsListMessenger(this);

  SetVerboseLevel(1);

  // EM physics
  emPhysicsList = new G4EmStandardPhysics_option3(1);
  emName = G4String("emstandard_opt3");

  // Decay physics and all particles
  decPhysicsList = new G4DecayPhysics();
}

/////////////////////////////////////////////////////////////////////////////
GammaRayTelPhysicsList::~GammaRayTelPhysicsList()
{
  delete pMessenger;
  delete emPhysicsList;
  delete decPhysicsList;
  for(size_t i=0; i<hadronPhys.size(); i++) {delete hadronPhys[i];}
}

/////////////////////////////////////////////////////////////////////////////
void GammaRayTelPhysicsList::AddPackage(const G4String& name)
{
  G4PhysListFactory factory;
  G4VModularPhysicsList* phys =factory.GetReferencePhysList(name);
  G4int i=0;
  const G4VPhysicsConstructor* elem= phys->GetPhysics(i);
  G4VPhysicsConstructor* tmp = const_cast<G4VPhysicsConstructor*> (elem);
  while (elem !=0)
	{
	  RegisterPhysics(tmp);
	  elem= phys->GetPhysics(++i) ;
	  tmp = const_cast<G4VPhysicsConstructor*> (elem);
	}
}

/////////////////////////////////////////////////////////////////////////////
void GammaRayTelPhysicsList::ConstructParticle()
{
  decPhysicsList->ConstructParticle();
}

/////////////////////////////////////////////////////////////////////////////
void GammaRayTelPhysicsList::ConstructProcess()
{
  // transportation
  //
  AddTransportation();

  // electromagnetic physics list
  //
  emPhysicsList->ConstructProcess();
  em_config.AddModels();

  // decay physics list
  //
  decPhysicsList->ConstructProcess();

  // hadronic physics lists
  for(size_t i=0; i<hadronPhys.size(); i++) {
    hadronPhys[i]->ConstructProcess();
  }

  // step limitation (as a full process)
  //
 // AddStepMax();
}

/////////////////////////////////////////////////////////////////////////////
void GammaRayTelPhysicsList::AddPhysicsList(const G4String& name)
{

  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }
  if (name == emName) return;

  /////////////////////////////////////////////////////////////////////////////
  //   ELECTROMAGNETIC MODELS
  /////////////////////////////////////////////////////////////////////////////

  if (name == "standard_opt3") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option3();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmStandardPhysics_option3" << G4endl;

  } else if (name == "LowE_Livermore") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmLivermorePhysics();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;
  } else if (name == "LowE_Penelope") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmPenelopePhysics();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;
  } else if (name == "LowE_Polarized") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmLivermorePolarizedPhysics();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;
  } else if (name == "standard_opt4") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option4();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmStandardOption_4" << G4endl;

    /////////////////////////////////////////////////////////////////////////////
    //   HADRONIC MODELS
    /////////////////////////////////////////////////////////////////////////////
  } else if (name == "elastic" && !helIsRegisted) {
    G4cout << "THE FOLLOWING HADRONIC ELASTIC PHYSICS LIST HAS BEEN ACTIVATED: G4HadronElasticPhysics()" << G4endl;
    hadronPhys.push_back( new G4HadronElasticPhysics());
    helIsRegisted = true;

  } else if (name == "DElastic" && !helIsRegisted) {
    hadronPhys.push_back( new G4HadronDElasticPhysics());
    helIsRegisted = true;

  } else if (name == "HPElastic" && !helIsRegisted) {
    hadronPhys.push_back( new G4HadronElasticPhysicsHP());
    helIsRegisted = true;

  } else if (name == "binary" && !bicIsRegisted) {
    hadronPhys.push_back(new G4HadronPhysicsQGSP_BIC_HP());
    bicIsRegisted = true;
    G4cout << "THE FOLLOWING HADRONIC INELASTIC PHYSICS LIST HAS BEEN ACTIVATED: HadronPhysicsQGSP_BIC_HP()" << G4endl;

  } else if (name == "binary_ion" && !biciIsRegisted) {
    hadronPhys.push_back(new G4IonBinaryCascadePhysics());
    biciIsRegisted = true;
    G4cout << "THE FOLLOWING HADRONIC INELASTIC PHYSICS LIST HAS BEEN ACTIVATED: G4IonBinaryCascadePhysics()" << G4endl;
  } else if (name == "radioactive_decay" && !radioactiveDecayIsRegisted ) {
    hadronPhys.push_back(new G4RadioactiveDecayPhysics());
    radioactiveDecayIsRegisted = true;
    G4cout << "THE FOLLOWING HADRONIC INELASTIC PHYSICS LIST HAS BEEN ACTIVATED: G4RadioactiveDecayPhysics()" << G4endl;
  } else {

    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
  }
}

void GammaRayTelPhysicsList::SetCutForGamma(G4double cut)
{
  SetParticleCuts(cut, G4Gamma::Gamma());
}

void GammaRayTelPhysicsList::SetCutForElectron(G4double cut)
{
  SetParticleCuts(cut, G4Electron::Electron());
}

void GammaRayTelPhysicsList::SetCutForPositron(G4double cut)
{
  SetParticleCuts(cut, G4Positron::Positron());
}

