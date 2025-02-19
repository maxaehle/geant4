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
// G4VPHitIO implementation
//
// Author: Youhei Morita, 10.08.2001
// --------------------------------------------------------------------

#include "G4VPHitIO.hh"

G4ThreadLocal G4VPHitIO* G4VPHitIO::f_G4VPHitIO = nullptr;

// --------------------------------------------------------------------
G4VPHitIO::G4VPHitIO()
{
  f_catalog = G4HCIOcatalog::GetHCIOcatalog();
}

// --------------------------------------------------------------------
void G4VPHitIO::SetVerboseLevel(G4int v)
{
  m_verbose = v;

  // Loop through the registered Hit I/O managers
  for(std::size_t i = 0; i < f_catalog->NumberOfHCIOmanager(); ++i)
  {
    G4VPHitsCollectionIO* hitIOman = f_catalog->GetHCIOmanager(i);
    hitIOman->SetVerboseLevel(v);
  }
}
