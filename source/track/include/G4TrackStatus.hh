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
// G4TrackStatus
//
// Description:
//
// Enumerator to define the current status of the track which
// is under transportation.

// Authors:
//   Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//   Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
// --------------------------------------------------------------------
#ifndef G4TrackStatus_hh
#define G4TrackStatus_hh 1

enum G4TrackStatus
{
  fAlive,
    // Continue the tracking
  fStopButAlive,
    // Invoke active rest physics processes and kill the current track afterward
  fStopAndKill,
    // Kill the current track
  fKillTrackAndSecondaries,
    // Kill the current track and also associated secondaries
  fSuspend,
    // Suspend the current track
  fPostponeToNextEvent
    // Postpone the tracking of the current track to the next event
};

#endif
