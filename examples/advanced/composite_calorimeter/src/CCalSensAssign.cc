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
///////////////////////////////////////////////////////////////////////////////
// File: CCalSensAssign.cc
// Description: CCalSenAssign creates and assigns the sensitive detetctors 
///////////////////////////////////////////////////////////////////////////////
#include "CCalSensAssign.hh"
#include "CCalSensitiveDetectors.hh"
#include "CCaloSD.hh"

#include "G4SDManager.hh"

//#define debug

G4ThreadLocal CCalSensAssign* CCalSensAssign::theInstance = nullptr;


CCalSensAssign* CCalSensAssign::getInstance() {
  if ( theInstance == nullptr ) {
    static G4ThreadLocalSingleton< CCalSensAssign > inst;
    theInstance = inst.Instance();
  }
  return theInstance;
}


CCalSensAssign::CCalSensAssign() {}


G4bool CCalSensAssign::assign() {
  G4bool result = false;
  CCalSensitiveDetectors* sensDets = CCalSensitiveDetectors::getInstance();
  for (std::map<G4String,G4VSensitiveDetector*>::const_iterator sens_it = sens_.begin();
       sens_it!=sens_.end(); ++sens_it) {
    G4String name = sens_it->first;
    G4VSensitiveDetector* sens = sens_it->second;
    if (sensDets->setSensitive(name, sens)) {
      G4SDManager::GetSDMpointer()->AddNewDetector(sens);
#ifdef debug
      G4cout << "Add " << sens->GetName() << " to the list of Sensitive detectors" << G4endl;
#endif
    }
  }
  return result;
}


G4bool CCalSensAssign::addCaloSD( G4String name, CCalVOrganization* numberingScheme ) {
  sens_[name] = new CCaloSD(name, numberingScheme);
  return true;
}
