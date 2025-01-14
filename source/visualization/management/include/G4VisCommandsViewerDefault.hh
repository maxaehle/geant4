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

// /vis/viewer/default commands - John Allison  30th October 2011

#ifndef G4VISCOMMANDSVIEWERDEFAULT_HH
#define G4VISCOMMANDSVIEWERDEFAULT_HH

#include "G4VVisCommand.hh"

class G4UIcmdWithAString;
class G4UIcmdWithABool;

class G4VisCommandViewerDefaultHiddenEdge: public G4VVisCommand {
public:
  G4VisCommandViewerDefaultHiddenEdge();
  virtual ~G4VisCommandViewerDefaultHiddenEdge();
  G4String GetCurrentValue(G4UIcommand* command);
  void SetNewValue(G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerDefaultHiddenEdge(const G4VisCommandViewerDefaultHiddenEdge&);
  G4VisCommandViewerDefaultHiddenEdge& operator=(const G4VisCommandViewerDefaultHiddenEdge&);
  G4UIcmdWithABool* fpCommand;
};

class G4VisCommandViewerDefaultStyle: public G4VVisCommand {
public:
  G4VisCommandViewerDefaultStyle();
  virtual ~G4VisCommandViewerDefaultStyle();
  G4String GetCurrentValue(G4UIcommand* command);
  void SetNewValue(G4UIcommand* command, G4String newValue);
private:
  G4VisCommandViewerDefaultStyle(const G4VisCommandViewerDefaultStyle&);
  G4VisCommandViewerDefaultStyle& operator=(const G4VisCommandViewerDefaultStyle&);
  G4UIcmdWithAString* fpCommand;
};

#endif
