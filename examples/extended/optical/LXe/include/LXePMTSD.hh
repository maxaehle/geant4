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
/// \file optical/LXe/include/LXePMTSD.hh
/// \brief Definition of the LXePMTSD class
//
//
#ifndef LXePMTSD_h
#define LXePMTSD_h 1

#include "LXePMTHit.hh"

#include "G4VSensitiveDetector.hh"

#include <vector>

class G4DataVector;
class G4HCofThisEvent;
class G4Step;

class LXePMTSD : public G4VSensitiveDetector
{
 public:
  LXePMTSD(G4String name);
  ~LXePMTSD();

  void Initialize(G4HCofThisEvent*) override;
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*) override;

  // A version of processHits active on boundary
  G4bool ProcessHits_boundary(const G4Step*, G4TouchableHistory*);

  // Initialize the arrays to store pmt possitions
  inline void InitPMTs()
  {
    if(fPMTPositionsX)
      delete fPMTPositionsX;
    if(fPMTPositionsY)
      delete fPMTPositionsY;
    if(fPMTPositionsZ)
      delete fPMTPositionsZ;
    fPMTPositionsX = new G4DataVector();
    fPMTPositionsY = new G4DataVector();
    fPMTPositionsZ = new G4DataVector();
  }

  // Store a pmt position
  void SetPmtPositions(const std::vector<G4ThreeVector>& positions);

 private:
  LXePMTHitsCollection* fPMTHitCollection;

  G4DataVector* fPMTPositionsX;
  G4DataVector* fPMTPositionsY;
  G4DataVector* fPMTPositionsZ;

  G4int fHitCID;
};

#endif
