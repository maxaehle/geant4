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
// G4GDMLMessenger
//
// Class description:
//
// GDML UI messenger.

// Author: Witold Pokorski, October 2010
// --------------------------------------------------------------------
#ifndef G4GDMLMessenger_hh
#define G4GDMLMessenger_hh 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4VPhysicalVolume.hh"

class G4GDMLParser;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class G4GDMLMessenger : public G4UImessenger
{
  public:

    G4GDMLMessenger(G4GDMLParser*);
    ~G4GDMLMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:

    G4GDMLParser* myParser = nullptr;
    G4LogicalVolume* topvol = nullptr;

    G4UIdirectory* persistencyDir = nullptr;
    G4UIdirectory* gdmlDir = nullptr;
    G4UIcmdWithAString* ReaderCmd = nullptr;
    G4UIcmdWithAString* WriterCmd = nullptr;
    G4UIcmdWithAString* TopVolCmd = nullptr;
    G4UIcmdWithoutParameter* ClearCmd = nullptr;
    G4UIcmdWithABool* RegionCmd = nullptr;
    G4UIcmdWithABool* EcutsCmd = nullptr;
    G4UIcmdWithABool* SDCmd = nullptr;
    G4UIcmdWithABool* StripCmd = nullptr;
    G4UIcmdWithABool* AppendCmd = nullptr;

    G4bool pFlag = true;  // Append pointers to names flag
};

#endif
