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
/*
Author:
M. Omer and R. Hajima  on   17 October 2016
contact:
omer.mohamed@jaea.go.jp and hajima.ryoichi@qst.go.jp
Publication Information:
1- M. Omer, R. Hajima, Including Delbrück scattering in Geant4,
Nucl. Instrum. Methods Phys. Res. Sect. B, vol. 405, 2017, pp. 43-49.,
https://doi.org/10.1016/j.nimb.2017.05.028
2- M. Omer, R. Hajima, Geant4 physics process for elastic scattering of gamma-rays,
JAEA Technical Report 2018-007, 2018.
https://doi.org/10.11484/jaea-data-code-2018-007
*/
//         on base of G4LivermoreRayleighModel
//

#ifndef G4JAEAElasticScattering_h
#define G4JAEAElasticScattering_h 1

#include "G4VEmProcess.hh"


class G4JAEAElasticScattering : public G4VEmProcess
{
public:
  //Constructor
  explicit G4JAEAElasticScattering(const G4String& processName ="JAEA_ES",
				   G4ProcessType type = fElectromagnetic);
  //Destructor
  ~G4JAEAElasticScattering() override;
  
  //true for gamma only
  G4bool IsApplicable(const G4ParticleDefinition&) override;

protected:
  void InitialiseProcess(const G4ParticleDefinition*) override;

private:
  G4bool isInitialised;

};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
#endif
