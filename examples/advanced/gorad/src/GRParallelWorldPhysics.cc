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
//  Gorad (Geant4 Open-source Radiation Analysis and Design)
//
//  Author : Makoto Asai (SLAC National Accelerator Laboratory)
//
//  Development of Gorad is funded by NASA Johnson Space Center (JSC)
//  under the contract NNJ15HK11B.
//
// ********************************************************************
//
// GRParallelWorldPhysics.cc
//   Utility class that adds GRParallelWorldBiasingProcess to the
//   process managers of particles
//
// History
//   September 8th, 2020 : first implementation
//
// ********************************************************************

#include "GRParallelWorldPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4TransportationManager.hh"
#include "GRParallelWorldBiasingProcess.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(GRParallelWorldPhysics);

GRParallelWorldPhysics::GRParallelWorldPhysics(const G4String& name, G4bool layeredMass)
   :  G4VPhysicsConstructor(name), fLayeredMass(layeredMass)
{;}

GRParallelWorldPhysics::~GRParallelWorldPhysics()
{;}

void GRParallelWorldPhysics::ConstructParticle()
{;}

void GRParallelWorldPhysics::ConstructProcess()
{
  // Make sure the parallel world registered
  G4TransportationManager::GetTransportationManager()
   ->GetParallelWorld(namePhysics);

  // Add parallel world process
  GRParallelWorldBiasingProcess* theParallelWorldProcess
      = new GRParallelWorldBiasingProcess(namePhysics);
  theParallelWorldProcess->SetParallelWorld(namePhysics);
  theParallelWorldProcess->SetLayeredMaterialFlag(fLayeredMass);

  auto myParticleIterator=GetParticleIterator();
  myParticleIterator->reset();
  while( (*myParticleIterator)() ){
    G4ParticleDefinition* particle = myParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    pmanager->AddProcess(theParallelWorldProcess);
    if(theParallelWorldProcess->IsAtRestRequired(particle))
    {pmanager->SetProcessOrdering(theParallelWorldProcess, idxAtRest, 9900);}
    pmanager->SetProcessOrderingToSecond(theParallelWorldProcess, idxAlongStep);
    pmanager->SetProcessOrdering(theParallelWorldProcess, idxPostStep, 9900);
  }
}

