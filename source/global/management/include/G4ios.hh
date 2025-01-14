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
// G4ios
//
// Global types for cout/cerr streaming

// Authors: H.Yoshida, M.Nagamatu - November 1998
// --------------------------------------------------------------------
#ifndef G4ios_hh
#define G4ios_hh 1

#include "G4Types.hh"

#include <iostream>

#ifdef G4MULTITHREADED

extern G4GLOB_DLL std::ostream*& _G4cout_p();
extern G4GLOB_DLL std::ostream*& _G4cerr_p();
#  define G4cout (*_G4cout_p())
#  define G4cerr (*_G4cerr_p())

#else  // Sequential

extern G4GLOB_DLL std::ostream G4cout;
extern G4GLOB_DLL std::ostream G4cerr;

#endif

void G4iosInitialization();
void G4iosFinalization();

#define G4cin std::cin
#define G4endl std::endl

#endif
