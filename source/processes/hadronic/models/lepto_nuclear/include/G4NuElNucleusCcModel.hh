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
// $Id: G4NuElNucleusCcModel.hh 90228 2015-05-21 08:49:57Z gcosmo $
//
// Geant4 Header : G4NuElNucleusCcModel
//
// Author : V.Grichine 12.2.19
//  
// Modified:
//
// Class Description
// Default model for muon neutrino-nucleus charge current scattering; 
// Class Description - End

#ifndef G4NuElNucleusCcModel_h
#define G4NuElNucleusCcModel_h 1
 
#include "globals.hh"
#include "G4NeutrinoNucleusModel.hh"
#include "G4HadProjectile.hh"
#include "G4Nucleus.hh"
#include "G4NucleiProperties.hh"
#include "G4LorentzVector.hh"
#include "G4Threading.hh"

class G4ParticleDefinition;
class G4PreCompoundModel;
class G4Nucleus;
class G4Fragment;

class G4NuElNucleusCcModel : public G4NeutrinoNucleusModel      // G4HadronicInteraction
{
public:

  G4NuElNucleusCcModel(const G4String& name = "NuElNuclCcModel");

  virtual ~G4NuElNucleusCcModel();

  virtual void InitialiseModel();

  virtual G4bool IsApplicable(const G4HadProjectile & aTrack, 
  			      G4Nucleus & targetNucleus);

  virtual G4HadFinalState * ApplyYourself(const G4HadProjectile & aTrack, 
					  G4Nucleus & targetNucleus);


  ////////// KR excitation kinematics ////////////////////

  void SampleLVkr(const G4HadProjectile & aTrack, G4Nucleus & targetNucleus);

  G4double GetMinNuElEnergy(){ return fMel + 0.5*fMel*fMel/fM1 + 0.05*CLHEP::MeV; }; // kinematics + accuracy for sqrts
  
  G4double ThresholdEnergy(G4double mI, G4double mF, G4double mP) // for cluster decay
  { 
    G4double w = std::sqrt(fW2);
    return w + 0.5*( (mP+mF)*(mP+mF)-(w+mI)*(w+mI) )/mI;
  };
 
  virtual void ModelDescription(std::ostream&) const;

private:

  G4ParticleDefinition* theElectron;
 
  G4bool fData, fMaster; // for one initialisation only
  G4double fMel;

#ifdef G4MULTITHREADED
  static G4Mutex numuNucleusModel;
#endif
  
};



#endif
