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
// ClassName:   G4TritonBuilder
//
// Author: 2013 P. Arce
//
// Modified
// 12.04.2017 A.Dotti move to new design with base class
//
//----------------------------------------------------------------------------
//
 #include "G4TritonBuilder.hh"
 #include "G4ParticleDefinition.hh"
 #include "G4ParticleTable.hh"
 #include "G4ProcessManager.hh"

 
 void G4TritonBuilder::Build()
 {
   std::vector<G4VTritonBuilder *>::iterator i;
   for(i=theModelCollections.begin(); i!=theModelCollections.end(); i++)
   {
     (*i)->Build(theTritonInelastic);
   }
   G4ProcessManager * theProcMan = G4Triton::Triton()->GetProcessManager();
   theProcMan->AddDiscreteProcess(theTritonInelastic);
 }

 G4TritonBuilder::
 G4TritonBuilder()
 {
   theTritonInelastic=new G4HadronInelasticProcess( "tInelastic", G4Triton::Definition() );
 }

void G4TritonBuilder::RegisterMe(G4PhysicsBuilderInterface* aB) {
  auto bld = dynamic_cast<G4VTritonBuilder*>(aB);
  if ( bld != nullptr ) {
      theModelCollections.push_back(bld);
  } else {
      G4PhysicsBuilderInterface::RegisterMe(aB);
  }
}

