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

// Author: Ivana Hrivnacova, 18/06/2013  (ivana@ipno.in2p3.fr)

#include "G4RootAnalysisManager.hh"
#include "G4RootFileManager.hh"
#include "G4RootNtupleFileManager.hh"
#include "G4AnalysisManagerState.hh"
#include "G4AnalysisUtilities.hh"
#include "G4ThreadLocalSingleton.hh"
#include "G4Threading.hh"

using namespace G4Analysis;

//_____________________________________________________________________________
G4RootAnalysisManager* G4RootAnalysisManager::Instance()
{
  static G4ThreadLocalSingleton<G4RootAnalysisManager> instance;
  fgIsInstance = true;
  return instance.Instance();
}

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::IsInstance()
{
  return fgIsInstance;
}

//_____________________________________________________________________________
G4RootAnalysisManager::G4RootAnalysisManager()
 : G4ToolsAnalysisManager("Root")
{
  if ( ! G4Threading::IsWorkerThread() ) fgMasterInstance = this;

  // File manager
  fFileManager = std::make_shared<G4RootFileManager>(fState);
  SetFileManager(fFileManager);

  // Ntuple file manager
  fNtupleFileManager = std::make_shared<G4RootNtupleFileManager>(fState);
  fNtupleFileManager->SetFileManager(fFileManager);
  fNtupleFileManager->SetBookingManager(fNtupleBookingManager);
}

//_____________________________________________________________________________
G4RootAnalysisManager::~G4RootAnalysisManager()
{
  if ( fState.GetIsMaster() ) fgMasterInstance = nullptr;
  fgIsInstance = false;
}

//
// protected methods
//

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::OpenFileImpl(const G4String& fileName)
{
  // Create ntuple manager(s)
  // and set it to base class which takes then their ownership
  SetNtupleManager(fNtupleFileManager->CreateNtupleManager());

  auto result = true;

  // Open file
  if ( fNtupleFileManager->GetMergeMode() != G4NtupleMergeMode::kSlave )  {
    result &= fFileManager->OpenFile(fileName);
  }

  // Open ntuple file(s) and create ntuples from bookings
  result &= fNtupleFileManager->ActionAtOpenFile(fFileManager->GetFullFileName());

  return result;
}

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::WriteImpl()
{

  auto result = true;

  Message(kVL4, "write", "files");

  if ( G4Threading::IsWorkerThread() )  {
    result &= G4ToolsAnalysisManager::Merge();
  }
  else {
    // Open all files registered with objects
    fFileManager->OpenFiles();

    // Write all histograms/profile on master
    result &= G4ToolsAnalysisManager::WriteImpl();
  }

  // Ntuples
  result &= fNtupleFileManager->ActionAtWrite();

  // File
  if ( fNtupleFileManager->GetMergeMode() != G4NtupleMergeMode::kSlave )  {
    // write all open files
    result &= fFileManager->WriteFiles();
  }

  // Write ASCII if activated
  if ( IsAscii() ) {
    result &= WriteAscii(fFileManager->GetFileName());
  }

  Message(kVL3, "write", "files", "", result);

  return result;
}

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::CloseFileImpl(G4bool reset)
{
  auto result = true;

  Message(kVL4, "close", "files");

  if ( reset ) {
    if ( ! Reset() ) {
      Warn("Resetting data failed", fkClass, "CloseFileImpl");
      result = false;
    }
  }

  result &= fNtupleFileManager->ActionAtCloseFile(reset);

  if ( fNtupleFileManager->GetMergeMode() != G4NtupleMergeMode::kSlave )  {
    // close all open files
    result &= fFileManager->CloseFiles();
  }

  // No files clean-up in sequential mode
  if ( ! G4Threading::IsMultithreadedApplication() )  return result;

  G4bool isNtupleManagerEmpty = fNtupleBookingManager->IsEmpty();

  // Delete files if empty in MT mode
  if ( ( fState.GetIsMaster() && G4ToolsAnalysisManager::IsEmpty() &&
         isNtupleManagerEmpty ) ||
       ( (! fState.GetIsMaster()) && isNtupleManagerEmpty &&
         fNtupleFileManager->GetMergeMode() == G4NtupleMergeMode::kNone ) ) {

    if ( std::remove(fFileManager->GetFullFileName()) ) {
      //  std::remove returns 0 when success
      Warn( "Removing file " + fFileManager->GetFullFileName() + " failed",
        fkClass, "CloseFileImpl");
      result = false;
    }

    Message(kVL1, "delete", "empty file", fFileManager->GetFullFileName());
  }
  else {
    Message(kVL3, "close", "files");
  }

  return result;
}

//_____________________________________________________________________________
G4bool G4RootAnalysisManager::ResetImpl()
{
// Reset histograms and ntuple

  auto result = true;

  result &= G4ToolsAnalysisManager::ResetImpl();
  if ( fNtupleFileManager != nullptr ) {
    result &= fNtupleFileManager->Reset();
  }

  return result;
}

//
// public methods
//

//_____________________________________________________________________________
void G4RootAnalysisManager::SetNtupleMerging(G4bool mergeNtuples,
                                             G4int  nofNtupleFiles)
{
  fNtupleFileManager->SetNtupleMerging(mergeNtuples, nofNtupleFiles);
}

//_____________________________________________________________________________
void G4RootAnalysisManager::SetNtupleRowWise(G4bool rowWise, G4bool rowMode)
{
  fNtupleFileManager->SetNtupleRowWise(rowWise, rowMode);
}

//_____________________________________________________________________________
void G4RootAnalysisManager::SetBasketSize(unsigned int basketSize)
{
  fFileManager->SetBasketSize(basketSize);
}

//_____________________________________________________________________________
void G4RootAnalysisManager::SetBasketEntries(unsigned int basketEntries)
{
  fFileManager->SetBasketEntries(basketEntries);
}
