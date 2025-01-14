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
// G4TrackingManager
//
// Class description:
//
// This is an interface class between the event, the track and the tracking
// categories. It handles necessary message passing between the upper
// hierarchical object, which is the event manager (G4EventManager), and
// lower hierarchical objects in the tracking category. It receives one track
// in an  event from the event manager and takes care to finish tracking it.
// Used exclusively by the Geant4 kernel.

// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//---------------------------------------------------------------
#ifndef G4TrackingManager_hh
#define G4TrackingManager_hh 1

#include "globals.hh"                  // Include from 'global'
#include "G4SteppingManager.hh"        // Include from 'tracking'
#include "G4Track.hh"                  // Include from 'tracking'
#include "G4TrackingMessenger.hh"
#include "G4TrackVector.hh"            // Include from 'tracking'
#include "G4TrackStatus.hh"            // Include from 'tracking'
#include "G4StepStatus.hh"             // Include from 'tracking'
#include "G4UserTrackingAction.hh"     // Include from 'tracking'
#include "G4UserSteppingAction.hh"     // Include from 'tracking'
#include "G4VTrajectory.hh"            // Include from 'tracking'

class G4VUserTrackInformation;

////////////////////////
class G4TrackingManager 
////////////////////////
{
 public:
  using ProfilerConfig = G4Track::ProfilerConfig;

  public:

    // Constructor/Destructor

    G4TrackingManager();
      // G4TrackingManger should be dynamically allocated, therefore you
      // need to invoke new() when you call this constructor.
      // G4SteppingManger and G4UserTrackingAction will be created
      // in this constructor. "This" pointer will be passed to
      // G4UserTrackingAction. 

    ~G4TrackingManager();

    // Get/Set functions

    G4Track* GetTrack() const;

    G4int GetStoreTrajectory() const;
    void SetStoreTrajectory(G4int value);

    G4SteppingManager* GetSteppingManager() const;

    G4UserTrackingAction* GetUserTrackingAction() const;

    G4VTrajectory* GimmeTrajectory() const;
    void SetTrajectory(G4VTrajectory* aTrajectory);
    
    G4TrackVector* GimmeSecondaries() const;

    void SetUserAction(G4UserTrackingAction* apAction);
    void SetUserAction(G4UserSteppingAction* apAction);

    void SetVerboseLevel(G4int vLevel);
    G4int GetVerboseLevel() const;

    // Other member functions

    void ProcessOneTrack(G4Track* apValueG4Track);
      // Invoking this function, a G4Track given by the argument
      // will be tracked.  

    void EventAborted();
      // Invoking this function, the current tracking will be
      // aborted immediately. The tracking will return the 
      // G4TrackStatus in 'fUserKillTrackAndSecondaries'.
      // By this the EventManager deletes the current track and all 
      // its associated secondaries.

    void SetUserTrackInformation(G4VUserTrackInformation* aValue);
      // This method can be invoked from the user's G4UserTrackingAction
      // implementation to set his/her own G4VUserTrackInformation concrete
      // class object to a G4Track object.

  private:

    void TrackBanner();  // verbose

    // Member data

    G4Track* fpTrack = nullptr;
    G4SteppingManager* fpSteppingManager = nullptr;
    G4UserTrackingAction* fpUserTrackingAction = nullptr;
    G4VTrajectory* fpTrajectory = nullptr;
    G4int StoreTrajectory = 0;
    G4int verboseLevel = 0;
    G4TrackingMessenger* messenger = nullptr;
    G4bool EventIsAborted = false;
};

//*******************************************************************
//
//  Inline function 
//
//*******************************************************************

inline G4Track* G4TrackingManager::GetTrack() const 
{ 
  return fpTrack;
}

inline G4int G4TrackingManager::GetStoreTrajectory() const
{ 
  return StoreTrajectory;
}

inline void G4TrackingManager::SetStoreTrajectory(G4int value)
{ 
  StoreTrajectory = value;
}

inline G4SteppingManager* G4TrackingManager::GetSteppingManager() const
{ 
  return fpSteppingManager; 
}

inline G4UserTrackingAction* G4TrackingManager::GetUserTrackingAction() const
{ 
  return fpUserTrackingAction; 
}

inline G4VTrajectory* G4TrackingManager::GimmeTrajectory() const
{ 
  return fpTrajectory; 
}
    
inline G4TrackVector* G4TrackingManager::GimmeSecondaries() const
{ 
  return fpSteppingManager->GetfSecondary(); 
}

inline void G4TrackingManager::SetUserAction(G4UserTrackingAction* apAction)
{
  fpUserTrackingAction = apAction;
  if(apAction != nullptr)
  {
    apAction->SetTrackingManagerPointer(this);
  }
}

inline void G4TrackingManager::SetUserAction(G4UserSteppingAction* apAction)
{
  fpSteppingManager->SetUserAction(apAction);
  if(apAction != nullptr)
  {
    apAction->SetSteppingManagerPointer(fpSteppingManager);  
  }
}

inline void G4TrackingManager::SetVerboseLevel(G4int vLevel)
{ 
  verboseLevel = vLevel; 
  fpSteppingManager -> SetVerboseLevel( vLevel );
}

inline G4int G4TrackingManager::GetVerboseLevel() const
{ 
  return verboseLevel; 
}

inline void
G4TrackingManager::SetUserTrackInformation(G4VUserTrackInformation* aValue)
{
  if(fpTrack != nullptr) fpTrack->SetUserInformation(aValue);
}

#endif
