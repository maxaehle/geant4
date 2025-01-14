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
/// \file ExGflashHit.hh
/// \brief Definition of the ExGflashHit class
//
#ifndef ExGflashHit_h
#define ExGflashHit_h 1
 
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4LogicalVolume;

class ExGflashHit : public G4VHit
{
  public:

      ExGflashHit();
      ExGflashHit(G4LogicalVolume* logVol);
      ~ExGflashHit();
      ExGflashHit(const ExGflashHit &right);
      const ExGflashHit& operator=(const ExGflashHit &right);
      G4bool operator==(const ExGflashHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);
      void *operator new(size_t,void*p){return p;}
#ifndef G4NOT_ISO_DELETES
      void operator delete(void *,void*){}
#endif

      virtual void Draw();
      virtual void Print();

  private:
      G4double fEdep;
      G4ThreeVector fPos;
      G4int fCrystalNumber;
      G4ThreeVector fStart;
      G4RotationMatrix fRot;
      const G4LogicalVolume* fLogV;

  public:
      inline void SetEdep(G4double de)
      { fEdep = de; };
      inline void AddEdep(G4double de)
      { fEdep += de; };
      inline G4double GetEdep()
      { return fEdep; };
      inline void SetPos(G4ThreeVector xyz)
      { fPos = xyz; };
      inline G4int GetCrystalNum()
      { return fCrystalNumber; };  
      inline void SetCrystalNum(G4int num)
      { fCrystalNumber=num; };
      inline G4ThreeVector GetPos()
      { return fPos; };
      inline void SetStart(G4ThreeVector xyz)
      { fStart = xyz; };
      inline G4ThreeVector GetStart()
      { return fStart; };

      inline void SetRot(G4RotationMatrix rmat)
      { fRot = rmat; };
      inline G4RotationMatrix GetRot()
      { return fRot; };
      inline const G4LogicalVolume * GetLogV()
      { return fLogV; };
};

typedef G4THitsCollection<ExGflashHit> ExGflashHitsCollection;

extern G4ThreadLocal G4Allocator<ExGflashHit>* ExGflashHitAllocator;

inline void* ExGflashHit::operator new(size_t)
{
  if(!ExGflashHitAllocator) ExGflashHitAllocator = new G4Allocator<ExGflashHit>;
  return (void *) ExGflashHitAllocator->MallocSingle();
}

inline void ExGflashHit::operator delete(void *aHit)
{
  ExGflashHitAllocator->FreeSingle((ExGflashHit*) aHit);
}

#endif





