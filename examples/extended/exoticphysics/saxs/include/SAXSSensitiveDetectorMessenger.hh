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
/// \file SAXSSensitiveDetectorMessenger.hh
/// \brief Implementation of the SAXSSensitiveDetectorMessenger class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SAXSSensitiveDetectorMessenger_h
#define SAXSSensitiveDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class SAXSSensitiveDetector;
class G4UIdirectory;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// SensitiveDetector messenger.

class SAXSSensitiveDetectorMessenger : public G4UImessenger
{
public:
  SAXSSensitiveDetectorMessenger(SAXSSensitiveDetector*);
  ~SAXSSensitiveDetectorMessenger();
  
  virtual void SetNewValue(G4UIcommand*,G4String);
    
private: 
  SAXSSensitiveDetector* fSenDet;  
  G4UIdirectory* fSenDetDir; 
  G4UIcmdWithABool* fUserStopAndKillCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

