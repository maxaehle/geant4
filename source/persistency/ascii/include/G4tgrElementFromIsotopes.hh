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
// G4tgrElementFromIsotopes
//
// Class description:
//
// Transient class of a chemical element.

// Author: P.Arce, CIEMAT (November 2007)
// --------------------------------------------------------------------
#ifndef G4tgrElementFromIsotopes_hh
#define G4tgrElementFromIsotopes_hh 1

#include "globals.hh"

#include <vector>

#include "G4tgrElement.hh"

class G4tgrElementFromIsotopes : public G4tgrElement
{
  public:

    G4tgrElementFromIsotopes();
    ~G4tgrElementFromIsotopes();

    G4tgrElementFromIsotopes(const std::vector<G4String>& wl);
      // Construct the G4tgrElementFromIsotopes (fill its data members)
      // interpreting the data in the list of words 'wl'

    G4int GetNumberOfIsotopes() const { return theNoIsotopes; }
    const G4String& GetComponent(G4int n) const { return theComponents[n]; }
    G4double GetAbundance(G4int n) const { return theAbundances[n]; }

    friend std::ostream& operator<<(std::ostream&,
                                    const G4tgrElementFromIsotopes&);

  private:

    G4int theNoIsotopes = 0;
    std::vector<G4String> theComponents;
    std::vector<G4double> theAbundances;
};

#endif
