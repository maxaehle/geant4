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
// G4ElectronOccupancy
//
// Class description:
//
// This class has information of occupation of electrons in atomic orbits.
// GetOccupancy(N) gives the number of electron in N-th orbit
//    For example : Carbon atom should be 
//          GetOccupancy(0)      --> 2
//          GetOccupancy(1)      --> 4
//          GetOccupancy(2..7)   --> 0
//
// GetTotalOccupancy() gives the total number of electrons

// Author: Hisaya Kurashige, 17 Aug 1999
// --------------------------------------------------------------------
#ifndef G4ElectronOccupancy_hh
#define G4ElectronOccupancy_hh 1

#include "globals.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"

#include "pwdefs.hh"

class G4ElectronOccupancy 
{
  public:

    enum { MaxSizeOfOrbit = 20};

    G4ElectronOccupancy( G4int sizeOrbit = MaxSizeOfOrbit );
    G4ElectronOccupancy( const G4ElectronOccupancy& right );

    virtual ~G4ElectronOccupancy();

    inline void *operator new(size_t);
    inline void operator delete(void *aElectronOccupancy);
      // new/delete operators are overloaded to use G4Allocator

    G4ElectronOccupancy& operator=(const G4ElectronOccupancy& right);
    G4bool operator==(const G4ElectronOccupancy& right) const;
    G4bool operator!=(const G4ElectronOccupancy& right) const;
      // operators
   
    inline G4int GetTotalOccupancy() const;
    inline G4int GetOccupancy(G4int orbit) const;
      // The following methods returns
      //     0:  if the orbit(atom) is vacant 
      //    >0:  number of electrons in orbit

    G4int AddElectron(G4int orbit, G4int number = 1);
    G4int RemoveElectron(G4int orbit, G4int number = 1);
   
    inline G4int  GetSizeOfOrbit() const;
    void   DumpInfo() const;

  private:

    G4int  theSizeOfOrbit = 0;
    G4int  theTotalOccupancy = 0;
    G4int* theOccupancies = nullptr;
};

extern G4PART_DLL
       G4Allocator<G4ElectronOccupancy>*& aElectronOccupancyAllocator();

// ------------------------
// Inline methods
// ------------------------

inline
void * G4ElectronOccupancy::operator new(size_t)
{
  if (aElectronOccupancyAllocator() == nullptr)
  {
    aElectronOccupancyAllocator() = new G4Allocator<G4ElectronOccupancy>;
  }
  return (void*) aElectronOccupancyAllocator()->MallocSingle();
}

inline
void G4ElectronOccupancy::operator delete(void* aElectronOccupancy)
{
  aElectronOccupancyAllocator()
    ->FreeSingle((G4ElectronOccupancy *) aElectronOccupancy);
}

inline
G4int G4ElectronOccupancy::GetSizeOfOrbit() const
{
  return  theSizeOfOrbit;
}

inline
G4int G4ElectronOccupancy::GetTotalOccupancy() const
{
  return  theTotalOccupancy;
}

inline
G4int G4ElectronOccupancy::GetOccupancy(G4int orbit) const
{
  G4int value = 0;
  if ((orbit >=0)&&(orbit<theSizeOfOrbit))
  {
    value = theOccupancies[orbit];
  }
  return value;  
}

#endif
