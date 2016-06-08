// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4UserTrackingAction.hh,v 1.7 2001/01/12 05:55:28 tsasaki Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
//
//---------------------------------------------------------------
//
// G4UserTrackingAction.hh
//
// class description:
//   This class represents actions taken place by the user at 
//   the start/end point of processing one track. 
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
//---------------------------------------------------------------

class G4UserTrackingAction;

#ifndef G4UserTrackingAction_h
#define G4UserTrackingAction_h 1

class G4TrackingManager;              // Forward declaration
class G4Track;

///////////////////////////
class G4UserTrackingAction 
///////////////////////////
{

//--------
public: // with description
//--------

// Constructor & Destructor
   G4UserTrackingAction(){;}
   virtual ~G4UserTrackingAction(){;}

// Member functions
   void SetTrackingManagerPointer(G4TrackingManager* pValue);
   virtual void PreUserTrackingAction(const G4Track* aTrack){;}
   virtual void PostUserTrackingAction(const G4Track* aTrack){;}

//----------- 
   protected:
//----------- 

// Member data
   G4TrackingManager* fpTrackingManager;

};

#endif

