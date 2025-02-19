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
//
// ----------------------------------------------------------------------
// Class G4TrackTerminator
//
// Class description:
//
// This is an interface for an object which can be called to kill a track.
// The type it provides is needed in case importance biasing and scoring
// is done at the same time. 
// For navigation in the parallel geometry which is done by the importance
// biasing process (it derives from G4ParallelTransport), importance
// biasing has to be done before scoring (to score the correct volume).
// But since scoring would not be called if the importance biasing kills
// a track, it only tells a G4TrackTerminator to kill the track.
// The scoring process implements the interface G4TrackTerminator
// and is now able to kill the track after it has performed the scoring.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4TrackTerminator_hh
#define G4TrackTerminator_hh G4TrackTerminator_hh

#include "G4Types.hh"

class G4TrackTerminator
{

public:  // without description

  G4TrackTerminator()
    : fKillTrack(false) {}
  ~G4TrackTerminator();

  void SetKillTrack(){fKillTrack = true;}
  G4bool GetKillTrack() const {return fKillTrack;} 

private:

  G4bool fKillTrack;
};

#endif
