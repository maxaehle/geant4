// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4PHCofThisEvent.ddl,v 1.9 2000/12/15 08:04:14 morita Exp $
// GEANT4 tag $Name: geant4-03-01 $
//

// Class Description:
//   This is a persistent version of container class which stores
// the associations to the hit collection in one event.
//   User should check the existence of the instance of this class
// in his/her sensitive detector class with GetCurrentPHCofThisEvent()
// method of a singleton class G4PersistentHitMan.  If it does
// not exist, use should create a new instance and register the smart
// pointer with SetCurrentPHCofThisEvent() method of G4PersistentHitMan.
//   As user creates a new collection of hit, its smart pointer should
// be registered to this class via AddHitsCollection().
//   To obtain a smart pointer of the i-th collection, use GetHC() method.
//

#ifndef G4PHCofThisEvent_h
#define G4PHCofThisEvent_h 1

#include "G4PersistentSchema.hh"

#include "G4PVHitsCollection.hh"

#include "HepODBMS/odbms/HepODBMS.h"

class G4PHCofThisEvent 
 : public HepPersObj
{
  public: // with description
      G4PHCofThisEvent();
      // Constructor.
      ~G4PHCofThisEvent();
      // Destructor.
      void AddHitsCollection(G4int HCID, HepRef(G4PVHitsCollection) aHC);
      // Store a smart pointer of the collection aHC as HCID-th entry.

  private:
      d_Varray< d_Ref<G4PVHitsCollection> > HC;

  public: // with description
      inline HepRef(G4PVHitsCollection) GetHC(G4int i)
      { return HC[i]; }
      //  Returns a smart pointer of a hit collection at the index i.
      // Null will be returned if the particular collection is not stored
      // in the current event.

  public:
      inline G4int GetCapacity()
      { return HC.size(); }
      inline G4int GetNumberOfCollections()
      {
        G4int n = 0;
        for(int i=0;i<HC.size();i++)
        {
          if( HC[i] != NULL ) n++;
        }
        return n;
      }
};

#endif
