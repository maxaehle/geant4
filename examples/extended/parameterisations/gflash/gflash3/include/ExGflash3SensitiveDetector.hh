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
/// \file ExGflash3SensitiveDetector.hh
/// \brief Definition of the ExGflash3SensitiveDetector class
//
#ifndef EX1GFLASHSENSITIVEDETECTOR_H
#define EX1GFLASHSENSITIVEDETECTOR_H

#include "G4VSensitiveDetector.hh"
#include "G4VGFlashSensitiveDetector.hh"
#include "ExGflashHitsCollection.hh"
#include "globals.hh"

class G4GFlashSpot;
class ExGflash3ParallelWorld;

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ExGflash3SensitiveDetector: public G4VSensitiveDetector, 
                                 public G4VGFlashSensitiveDetector {
public:
  ExGflash3SensitiveDetector(G4String, ExGflash3ParallelWorld* det);
  ~ExGflash3SensitiveDetector();
  
  virtual void Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  virtual G4bool ProcessHits(G4GFlashSpot*aSpot,G4TouchableHistory*);
  virtual void EndOfEvent(G4HCofThisEvent*);

private:
  ExGflashHitsCollection* fCaloHitsCollection;
  ExGflash3ParallelWorld* fDetector;
  G4int fHCID;
};

#endif
