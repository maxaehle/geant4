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
// G4VMCTruthIO
//
// Class Description:
//
// Abstract base class for storing and retrieving MCTruth events.

// Author: Youhei Morita, 18.11.2001
// --------------------------------------------------------------------
#ifndef G4VMCTRUTHIO_HH
#define G4VMCTRUTHIO_HH 1

#include "G4MCTEvent.hh"

class G4VMCTruthIO
{
  public:

    G4VMCTruthIO();
      // Constructor

    virtual ~G4VMCTruthIO();
      // Destructor

    virtual G4bool Store(G4MCTEvent*) = 0;
      // Pure virtual method for storing MCTruth Event.
      // Each persistency package should implement a concrete method
      // of storing the G4MCTEvent with this signature

    virtual G4bool Retrieve(G4MCTEvent*&) = 0;
      // Pure virtual method for retrieving MCTruth Event.
      // Each persistency package should implement a concrete method
      // of storing the G4MCTEvent with this signature

    void SetVerboseLevel(G4int v) { m_verbose = v; }
      // Set verbose level.

  protected:

    G4int m_verbose = 0;
};

#endif
