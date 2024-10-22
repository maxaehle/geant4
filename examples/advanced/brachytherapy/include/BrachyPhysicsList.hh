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
//    **********************************
//    *                                *
//    *   BrachyPhysicsList.hh         *
//    *                                *
//    **********************************
/*
Author: Susanna Guatelli
*/
#ifndef BrachyPhysicsList_h
#define BrachyPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"
#include "globals.hh"

class BrachyPhysicsListMessenger;

// Modular physics used in this application
class BrachyPhysicsList: public G4VModularPhysicsList
{
public:
  explicit BrachyPhysicsList();
  ~BrachyPhysicsList();
  void ConstructParticle() override; 
  void AddPhysicsList(const G4String& name);
  void ConstructProcess() override;

private:
  BrachyPhysicsListMessenger* fMessenger;
  G4VPhysicsConstructor* fEmPhysicsList;
  G4VPhysicsConstructor* fDecPhysicsList;
  G4VPhysicsConstructor* fRadDecayPhysicsList;
  G4String fEmName;
};
#endif
