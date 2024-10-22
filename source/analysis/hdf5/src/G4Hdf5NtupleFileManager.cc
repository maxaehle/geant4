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

// Author: Ivana Hrivnacova, 15/09/2020  (ivana@ipno.in2p3.fr)

#include "G4Hdf5NtupleFileManager.hh"
#include "G4Hdf5NtupleManager.hh"
#include "G4Hdf5FileManager.hh"
#include "G4AnalysisManagerState.hh"
#include "G4AnalysisUtilities.hh"

using namespace G4Analysis;

//
// utility methods
//

//_____________________________________________________________________________
G4Hdf5NtupleFileManager::G4Hdf5NtupleFileManager(const G4AnalysisManagerState& state)
 : G4VNtupleFileManager(state, "hdf5")
{}

//
// private methods
//

//_____________________________________________________________________________
G4bool G4Hdf5NtupleFileManager::CloseNtupleFiles()
{
 // Close ntuple files

  auto result = true;
  auto ntupleVector = fNtupleManager->GetNtupleDescriptionVector();
  for ( auto ntupleDescription : ntupleVector) {
    result &= fFileManager->CloseNtupleFile(ntupleDescription);
  }

  return result;
}

//
// public methods
//

//_____________________________________________________________________________
std::shared_ptr<G4VNtupleManager> G4Hdf5NtupleFileManager::CreateNtupleManager()
{

  // The managers will be deleted by the base class
  fNtupleManager = std::make_shared<G4Hdf5NtupleManager>(fState);
  fNtupleManager->SetFileManager(fFileManager);

  return fNtupleManager;
}

//_____________________________________________________________________________
G4bool G4Hdf5NtupleFileManager::ActionAtOpenFile(const G4String& /*fileName*/)
{
  // Create ntuples if they are booked
  // (The files will be created with creating ntuples)
  fNtupleManager->CreateNtuplesFromBooking(
    fBookingManager->GetNtupleBookingVector());

  return true;
}

//_____________________________________________________________________________
G4bool G4Hdf5NtupleFileManager::ActionAtWrite()
{
  // Nothing to be done
  return true;
}

//_____________________________________________________________________________
G4bool G4Hdf5NtupleFileManager::ActionAtCloseFile(G4bool reset)
{
  auto result = true;

  // Close ntuple files
  result &= CloseNtupleFiles();

  if ( ! reset ) {
    // The ntuples must be always reset when closing file)
    result = Reset();
    if ( ! result ) {
      Warn("Resetting data failed", fkClass, "ActionAtCloseFile");
    }
  }

  return result;
}

//_____________________________________________________________________________
G4bool G4Hdf5NtupleFileManager::Reset()
{
  return fNtupleManager->Reset();
}
