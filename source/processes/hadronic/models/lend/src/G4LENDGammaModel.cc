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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  File:   G4LENDGammaModel.cc                                               //
//  Date:   30 March 2020                                                     //
//  Author: Dennis H. Wright                                                  //
//                                                                            //
//  Description: model for inelastic scattering of gammas from nuclei         //
//               including gamma-induced fission.  This model is very similar //
//               to G4LENDCombinedModel except that it does not sample        //
//               elastic or capture reactions since there are no such data    //
//               for gammas in GND.                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "G4LENDGammaModel.hh"
#include "G4LENDGammaCrossSection.hh"
#include "G4LENDInelastic.hh"
#include "G4LENDFission.hh"
#include "G4DynamicParticle.hh"

G4LENDGammaModel::G4LENDGammaModel(G4ParticleDefinition* pd)
 :G4LENDModel("LENDGammaModel") {
  proj = pd;
  crossSection = new G4LENDGammaCrossSection(pd);
  inelastic = new G4LENDInelastic(pd);
  fission = new G4LENDFission(pd);
  channels[0] = inelastic;
  channels[1] = fission;
}

void G4LENDGammaModel::BuildPhysicsTable(const G4ParticleDefinition& projectile) {
  crossSection->BuildPhysicsTable(projectile);
  create_used_target_map();
}

G4bool G4LENDGammaModel::HasData(const G4DynamicParticle*, G4int iZ, G4int iA,
                                 G4int iM, const G4Isotope*, const G4Element*,
                                 const G4Material*)
{
  G4bool result = false;
  if (get_target_from_map(lend_manager->GetNucleusEncoding(iZ, iA, iM) ) != nullptr) result = true;
  return result;
}


G4HadFinalState* G4LENDGammaModel::ApplyYourself(const G4HadProjectile& aTrack,
                                                 G4Nucleus& aTarg)
{
   G4LENDModel* channel = nullptr;

   G4int iZ = aTarg.GetZ_asInt();
   G4int iA = aTarg.GetA_asInt();
   //To pass kinetic energy, need to generate dynamic particle  
   G4DynamicParticle* dp = new G4DynamicParticle(proj, G4ThreeVector(0.,0.,1.),
                                                 aTrack.GetKineticEnergy() );
   G4int ichannel = crossSection->SelectChannel(dp, iZ, iA, aTarg.GetIsotope(),
                                                nullptr, aTrack.GetMaterial() );
   delete dp;
   channel = channels[ichannel];
   return channel->ApplyYourself(aTrack, aTarg);
}

