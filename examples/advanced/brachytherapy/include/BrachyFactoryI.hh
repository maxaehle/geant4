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
//    *******************************
//    *                             *
//    *    BrachyFactoryI.cc        *
//    *                             *
//    *******************************
//
//
// Code developed by:
//  S.Guatelli
//
#ifndef BrachyFactoryI_h
#define BrachyFactoryI_h 1

#include "BrachyDetectorConstructionI.hh"
#include "BrachyFactory.hh"
#include "G4RunManager.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class BrachyFactory;
class BrachyDetectorConstructionI;
class G4ParticleGun;
class G4Run;
class G4Event;

// This class manages the creation of Bebig Isoseed I-125 source 
// used in interstitial brachytherapy ...
class BrachyFactoryI:public BrachyFactory
{
public:
  explicit BrachyFactoryI();
 ~BrachyFactoryI();

  void CreateSource(G4VPhysicalVolume*)override;
  void CleanSource() override;

private:
  BrachyDetectorConstructionI* fIodineSource;
};
#endif
