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
// *********************************************************************
// |                                                                   |
// |             G4LowEPComptonModel-- Geant4 Monash University        |
// |                   low energy Compton scattering model.            |
// |             J. M. C. Brown, Monash University, Australia          |
// |                    ## Unpolarised photons only ##                 |
// |                                                                   |
// |                                                                   |
// *********************************************************************
// |                                                                   |
// | The following is a Geant4 class to simulate the process of        |
// | bound electron Compton scattering. General code structure is      |
// | based on G4LowEnergyCompton.cc and G4LivermoreComptonModel.cc.    |
// | Algorithms for photon energy, and ejected Compton electron        |
// | direction taken from:                                             |
// |                                                                   |
// | J. M. C. Brown, M. R. Dimmock, J. E. Gillam and D. M. Paganin,    |
// | "A low energy bound atomic electron Compton scattering model      |
// |  for Geant4", NIMB, Vol. 338, 77-88, 2014.                        |
// |                                                                   |
// | The author acknowledges the work of the Geant4 collaboration      |
// | in developing the following algorithms that have been employed    |
// | or adapeted for the present software:                             |    
// |                                                                   |
// |  # sampling of photon scattering angle,                           |
// |  # target element selection in composite materials,               |
// |  # target shell selection in element,                             |
// |  # and sampling of bound electron momentum from Compton profiles. |
// |                                                                   |
// *********************************************************************
// |                                                                   |
// | History:                                                          |
// | --------                                                          |
// |                                                                   |
// | Nov. 2011 JMCB       - First version                              |
// | Feb. 2012 JMCB       - Migration to Geant4 9.5                    |
// | Sep. 2012 JMCB       - Final fixes for Geant4 9.6                 |
// | Feb. 2013 JMCB       - Geant4 9.6 FPE fix for bug 1426            |
// | Jan. 2015 JMCB       - Migration to MT (Based on Livermore        |
// |                        implementation)                            |
// | Feb. 2016 JMCB       - Geant4 10.2 FPE fix for bug 1676           |
// |                                                                   |
// *********************************************************************

#ifndef G4LowEPComptonModel_h
#define G4LowEPComptonModel_h 1

#include "G4VEmModel.hh"
#include "G4PhysicsFreeVector.hh"

class G4ParticleChangeForGamma;
class G4VAtomDeexcitation;
class G4ShellData;
class G4DopplerProfile;

class G4LowEPComptonModel : public G4VEmModel
{

public:

  explicit G4LowEPComptonModel(const G4ParticleDefinition* p = nullptr, 
		          const G4String& nam = "LowEPComptonModel");
    virtual ~G4LowEPComptonModel();

  void Initialise(const G4ParticleDefinition*, const G4DataVector&) override;
  void InitialiseLocal(const G4ParticleDefinition*,
                               G4VEmModel* masterModel) override;

  void InitialiseForElement(const G4ParticleDefinition*, G4int Z) override;

  G4double ComputeCrossSectionPerAtom( const G4ParticleDefinition*,
				       G4double kinEnergy,
				       G4double Z,
				       G4double A=0,
				       G4double cut=0,
				       G4double emax=DBL_MAX ) override;

  void SampleSecondaries(std::vector<G4DynamicParticle*>*,
			 const G4MaterialCutsCouple*,
			 const G4DynamicParticle*,
			 G4double tmin,
			 G4double maxEnergy) override;

  G4LowEPComptonModel & operator=(const  G4LowEPComptonModel &right) = delete;
  G4LowEPComptonModel(const  G4LowEPComptonModel&) = delete;

private:
  void ReadData(size_t Z, const char* path = 0);
  G4double ComputeScatteringFunction(G4double x, G4int Z);

  G4ParticleChangeForGamma* fParticleChange;
  G4VAtomDeexcitation*      fAtomDeexcitation;
  static G4ShellData*       shellData;
  static G4DopplerProfile*  profileData;

  static const G4int maxZ = 99;
  static G4PhysicsFreeVector* data[100];
  static const G4double ScatFuncFitParam[101][9];
 
  G4int verboseLevel;
  G4bool isInitialised;
};

//****************************************************************************

#endif
