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
// by V. Lara
//

#ifndef G4VPreCompoundEmissionFactory_hh
#define G4VPreCompoundEmissionFactory_hh

#include "G4VPreCompoundFragment.hh"
#include <vector>

class G4VPreCompoundEmissionFactory
{
public:

  G4VPreCompoundEmissionFactory();

  virtual ~G4VPreCompoundEmissionFactory();
  
  inline std::vector<G4VPreCompoundFragment*> * GetFragmentVector();

  G4VPreCompoundEmissionFactory(const G4VPreCompoundEmissionFactory&) = delete;
  const G4VPreCompoundEmissionFactory & operator=
  (const G4VPreCompoundEmissionFactory & val) = delete;
  G4bool operator==(const G4VPreCompoundEmissionFactory & val) const = delete;
  G4bool operator!=(const G4VPreCompoundEmissionFactory & val) const = delete;

protected:

  virtual std::vector<G4VPreCompoundFragment*> * CreateFragmentVector() = 0;

private:

  std::vector<G4VPreCompoundFragment*> * fragvector = nullptr;
};

inline std::vector<G4VPreCompoundFragment*> * 
G4VPreCompoundEmissionFactory::GetFragmentVector()
{
  if (fragvector == nullptr) { fragvector = CreateFragmentVector(); }
  return fragvector;
}

#endif
