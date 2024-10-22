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
//---------------------------------------------------------------------------
//
// ClassName:   G4ChargeExchangePhysics
//
// Author: 19 November 2008 V. Ivanchenko
//
// Modified:
//
//----------------------------------------------------------------------------
//

#include "G4ChargeExchangePhysics.hh"

#include "G4ChargeExchangeProcess.hh"
#include "G4ChargeExchange.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4Neutron.hh"

#include "G4BGGPionElasticXS.hh"
#include "G4BGGNucleonElasticXS.hh"
#include "G4NeutronElasticXS.hh"
#include "G4HadronicParameters.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(G4ChargeExchangePhysics);

G4ChargeExchangePhysics::G4ChargeExchangePhysics(G4int ver)
  : G4VPhysicsConstructor("chargeExchange")
{
  // because it is an addition, the type of this constructor is 0
  G4HadronicParameters::Instance()->SetVerboseLevel(ver);
  if(ver > 1) G4cout << "### ChargeExchangePhysics" << G4endl;
}

G4ChargeExchangePhysics::~G4ChargeExchangePhysics()
{}

void G4ChargeExchangePhysics::ConstructParticle()
{
// G4cout << "G4ChargeExchangePhysics::ConstructParticle" << G4endl;
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();
}

void G4ChargeExchangePhysics::ConstructProcess()
{
  G4ChargeExchange* model = new G4ChargeExchange();

  if(G4HadronicParameters::Instance()->GetVerboseLevel() > 1) {
    G4cout << "### ChargeExchangePhysics Construct Processes with the model <" 
	   << model->GetModelName() << ">" << G4endl;
  }

  auto myParticleIterator=GetParticleIterator();
  myParticleIterator->reset();
  while( (*myParticleIterator)() )
  {
    G4ParticleDefinition* particle = myParticleIterator->value();
    if( particle == G4Neutron::Definition()   || 
        particle == G4PionMinus::Definition() || 
        particle == G4PionPlus::Definition()  || 
        particle == G4Proton::Definition()
       ) { 
      
      G4ProcessManager* pmanager = particle->GetProcessManager();
      G4ChargeExchangeProcess* p = new G4ChargeExchangeProcess();
      p->RegisterMe(model);
      
      if( particle == G4PionMinus::Definition() || particle == G4PionPlus::Definition() ) {
        p->AddDataSet( new G4BGGPionElasticXS( particle ) );
      } else if( particle == G4Proton::Definition() ) {
        p->AddDataSet( new G4BGGNucleonElasticXS( particle ) );
      } else if( particle == G4Neutron::Definition() ) {
        p->AddDataSet( new G4NeutronElasticXS );
      }
  
      pmanager->AddDiscreteProcess(p);

      if(G4HadronicParameters::Instance()->GetVerboseLevel() > 1)
	G4cout << "### ChargeExchangePhysics added for " 
	       << particle->GetParticleName() << G4endl;
    }
  }
}


