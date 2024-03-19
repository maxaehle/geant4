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
///////////////////////////////////////////////////////////////////////////
//
// Rough model describing a gamma function distributed radiator of X-ray
// transition radiation. XTR is considered to flux after radiator!
// Thicknesses of plates and gas gaps are distributed according to gamma
// distribution. x are thicknesses of plates or gas gaps:
//
// p(x) = (alpha/<x>)^alpha * x^(alpha-1) * std::exp(-alpha*x/<x>) / G(alpha)
//
// G(alpha) is Euler's gamma function.
// Plates have mean <x> = fPlateThick > 0 and power alpha = fAlphaPlate > 0 :
// Gas gaps have mean <x> = fGasThick > 0 and power alpha = fAlphaGas > 0 :
// We suppose that:
// formation zone ~ mean thickness << absorption length
// for each material and in the range 1-100 keV. This allows us to simplify
// interference effects in radiator stack (GetStackFactor method).
//
// History:
//
// 03.10.05 V. Grichine, first version
//

#ifndef G4XTRGammaRadModel_h
#define G4XTRGammaRadModel_h 1

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VXTRenergyLoss.hh"

class G4XTRGammaRadModel : public G4VXTRenergyLoss
{
 public:
  explicit G4XTRGammaRadModel(G4LogicalVolume* anEnvelope, G4double, G4double,
                              G4Material*, G4Material*, G4double, G4double,
                              G4int,
                              const G4String& processName = "XTRgammaRadiator");
  ~G4XTRGammaRadModel();

  void ProcessDescription(std::ostream&) const override;
  void DumpInfo() const override { ProcessDescription(G4cout); };

  G4double GetStackFactor(G4double energy, G4double gamma,
                          G4double varAngle) override;
};

#endif
