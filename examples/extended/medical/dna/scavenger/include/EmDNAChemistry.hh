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
/// \file EmDNAChemistry.hh
/// \brief Definition of the EmDNAChemistry class

#ifndef EmDNAChemistry_hh
#define EmDNAChemistry_hh 1

#include "G4VPhysicsConstructor.hh"
#include "G4VUserChemistryList.hh"
#include "globals.hh"

#include "ParserChemReaction.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"

class G4DNAMolecularReactionTable;

class G4MolecularConfiguration;

/// Chemistry constructor with reading of the user file defining chemical
/// reactions and scavengers (reaction with background)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace scavenger
{

class EmDNAChemistry : public G4VUserChemistryList,
                       public G4VPhysicsConstructor,
                       public G4UImessenger {
public:
  EmDNAChemistry();

  ~EmDNAChemistry() override = default;

  // Record name of the chemical reaction table to be used
  void SetNewValue(G4UIcommand *, G4String) override;

  void ConstructParticle() override {
    ConstructMolecule();
  }

  void ConstructMolecule() override;

  void ConstructProcess() override;

  void ConstructDissociationChannels() override;

  void ConstructReactionTable(G4DNAMolecularReactionTable *) override;

  void ConstructTimeStepModel(G4DNAMolecularReactionTable *) override;

private:
  std::unique_ptr<G4UIdirectory> fpParserDir;
  std::unique_ptr<G4UIcmdWithAString> fpReactionTableNameCmd;
  G4String fReactionTableName = "";
};

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
