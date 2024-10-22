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
// ********************************************************************
// |                                                                   |
// | History:                                                          |
// | --------                                                          |
// |                                                                   |
// | Feb. 2021 JMCB  - Adapted for polarized gamma ray transport.      |
// |                   See "An electromagnetic physics constructor     |
// |                   for low energy polarised X-/gamma ray transport |
// |                   in Geant4", J. M. C. Brown and M. R. Dimmock,   |
// |                   arXiv:2102.02721 (2021).                        |
// |                   https://arxiv.org/abs/2102.02721                |
// |                                                                   |
// *********************************************************************
//

#ifndef G4EmLowEPPhysics_h
#define G4EmLowEPPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4EmLowEPPhysics : public G4VPhysicsConstructor
{
public:

  explicit G4EmLowEPPhysics(G4int ver=1, const G4String& name= "");

  ~G4EmLowEPPhysics() override;

  void ConstructParticle() override;
  void ConstructProcess() override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif






