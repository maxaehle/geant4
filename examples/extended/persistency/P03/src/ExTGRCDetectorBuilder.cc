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
/// \file persistency/P03/src/ExTGRCDetectorBuilder.cc
/// \brief Implementation of the ExTGRCDetectorBuilder class

#include "ExTGRCDetectorBuilder.hh"
#include "ExTGRCLineProcessor.hh"
#include "ExTGRCRegionCutsMgr.hh"

#include "G4tgrFileReader.hh"
#include "G4tgrVolumeMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExTGRCDetectorBuilder::ExTGRCDetectorBuilder()
  : G4tgbDetectorBuilder(), fTlproc(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExTGRCDetectorBuilder::~ExTGRCDetectorBuilder()
{
  if (fTlproc)  { delete fTlproc; }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const G4tgrVolume* ExTGRCDetectorBuilder::ReadDetector()
{
  //------------------- construct geometry
  fTlproc = new ExTGRCLineProcessor;
  G4tgrFileReader* tfr = G4tgrFileReader::GetInstance();
  tfr->SetLineProcessor( fTlproc );
  tfr->ReadFiles();

  //---------- find top G4tgrVolume 
  G4tgrVolumeMgr* tgrVolmgr = G4tgrVolumeMgr::GetInstance();
  const G4tgrVolume* tgrVoltop = tgrVolmgr->GetTopVolume();

  return tgrVoltop;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* ExTGRCDetectorBuilder::
ConstructDetector( const G4tgrVolume* tgrVoltop)
{
  G4VPhysicalVolume* topPV =
    G4tgbDetectorBuilder::ConstructDetector( tgrVoltop );

  //--- Create regions
  ExTGRCRegionCutsMgr::GetInstance()->BuildRegions();
  
  //--- Set cuts to regions
    ExTGRCRegionCutsMgr::GetInstance()->BuildProductionCuts();

  return topPV;
}
