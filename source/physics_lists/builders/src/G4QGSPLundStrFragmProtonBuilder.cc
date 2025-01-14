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
// ClassName:   QGSPStrFragmLundProtonBuilder
//
// Author: Julia Yarba, FNAL/CD (2014)
// 12.04.2017 A.Dotti move to new design with base class
//
//
//----------------------------------------------------------------------------
//
#include "G4QGSPLundStrFragmProtonBuilder.hh"
//
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4BGGNucleonInelasticXS.hh"
#include "G4HadronicParameters.hh"

G4QGSPLundStrFragmProtonBuilder::
G4QGSPLundStrFragmProtonBuilder(G4bool quasiElastic) 
{
  theMin = 100*GeV;
  theModel = new G4TheoFSGenerator("QGSP");

  G4QGSModel< G4QGSParticipants >* theStringModel = 
    new G4QGSModel< G4QGSParticipants >;
  theStringModel->SetFragmentationModel(new G4ExcitedStringDecay());

  G4GeneratorPrecompoundInterface* theCascade = 
    new G4GeneratorPrecompoundInterface();

  theModel->SetTransport(theCascade);
  theModel->SetHighEnergyGenerator(theStringModel);
  if (quasiElastic)
    {
      theModel->SetQuasiElasticChannel(new G4QuasiElasticChannel());
    } 
}

void G4QGSPLundStrFragmProtonBuilder::
Build(G4HadronInelasticProcess * aP)
{
  aP->AddDataSet(new G4BGGNucleonInelasticXS(G4Proton::Proton()));
  theModel->SetMinEnergy(theMin);
  theModel->SetMaxEnergy( G4HadronicParameters::Instance()->GetMaxEnergy() );
  aP->RegisterMe(theModel);
}

G4QGSPLundStrFragmProtonBuilder::~G4QGSPLundStrFragmProtonBuilder() 
{}

