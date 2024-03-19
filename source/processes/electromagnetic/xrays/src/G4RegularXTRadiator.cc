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

#include "G4RegularXTRadiator.hh"

#include "G4Gamma.hh"
#include "G4PhysicalConstants.hh"

////////////////////////////////////////////////////////////////////////////
// Constructor, destructor
G4RegularXTRadiator::G4RegularXTRadiator(G4LogicalVolume* anEnvelope,
                                         G4Material* foilMat,
                                         G4Material* gasMat, G4double a,
                                         G4double b, G4int n,
                                         const G4String& processName)
  : G4VXTRenergyLoss(anEnvelope, foilMat, gasMat, a, b, n, processName)
{
  G4cout << "Regular X-ray TR radiator EM process is called" << G4endl;

  // Build energy and angular integral spectra of X-ray TR photons from
  // a radiator

  fAlphaPlate = 10000;
  fAlphaGas   = 1000;
  G4cout << "fAlphaPlate = " << fAlphaPlate << " ; fAlphaGas = " << fAlphaGas
         << G4endl;
}

///////////////////////////////////////////////////////////////////////////
G4RegularXTRadiator::~G4RegularXTRadiator() {}

void G4RegularXTRadiator::ProcessDescription(std::ostream& out) const
{
  out << "Simulation of X-ray transition radiation generated by\n"
         "relativistic charged particles crossing the interface between\n"
         "two materials. Thicknesses of plates and gaps are fixed.\n";
}

///////////////////////////////////////////////////////////////////////////
G4double G4RegularXTRadiator::SpectralXTRdEdx(G4double energy)
{
  G4double result, sum = 0., tmp, cof1, cof2, cofMin, cofPHC, theta2, theta2k;
  G4double aMa, bMb, sigma, dump;
  G4int k, kMax, kMin;

  aMa   = fPlateThick * GetPlateLinearPhotoAbs(energy);
  bMb   = fGasThick * GetGasLinearPhotoAbs(energy);
  sigma = 0.5 * (aMa + bMb);
  dump  = std::exp(-fPlateNumber * sigma);
  if(verboseLevel > 2)
    G4cout << " dump = " << dump << G4endl;
  cofPHC = 4 * pi * hbarc;
  tmp    = (fSigma1 - fSigma2) / cofPHC / energy;
  cof1   = fPlateThick * tmp;
  cof2   = fGasThick * tmp;

  cofMin = energy * (fPlateThick + fGasThick) / fGamma / fGamma;
  cofMin += (fPlateThick * fSigma1 + fGasThick * fSigma2) / energy;
  cofMin /= cofPHC;

  theta2 = cofPHC / (energy * (fPlateThick + fGasThick));

  kMin = G4int(cofMin);
  if(cofMin > kMin)
    kMin++;

  kMax = kMin + 49;

  if(verboseLevel > 2)
  {
    G4cout << cof1 << "     " << cof2 << "        " << cofMin << G4endl;
    G4cout << "kMin = " << kMin << ";    kMax = " << kMax << G4endl;
  }
  for(k = kMin; k <= kMax; ++k)
  {
    tmp    = pi * fPlateThick * (k + cof2) / (fPlateThick + fGasThick);
    result = (k - cof1) * (k - cof1) * (k + cof2) * (k + cof2);
    if(k == kMin && kMin == G4int(cofMin))
    {
      sum +=
        0.5 * std::sin(tmp) * std::sin(tmp) * std::abs(k - cofMin) / result;
    }
    else
    {
      sum += std::sin(tmp) * std::sin(tmp) * std::abs(k - cofMin) / result;
    }
    theta2k = std::sqrt(theta2 * std::abs(k - cofMin));

    if(verboseLevel > 2)
    {
      G4cout << k << "   " << theta2k << "     "
             << std::sin(tmp) * std::sin(tmp) * std::abs(k - cofMin) / result
             << "      " << sum << G4endl;
    }
  }
  result = 2 * (cof1 + cof2) * (cof1 + cof2) * sum / energy;
  result *= (1 - dump + 2 * dump * fPlateNumber);

  return result;
}

///////////////////////////////////////////////////////////////////////////
// Approximation for radiator interference factor for the case of
// fully Regular radiator. The plate and gas gap thicknesses are fixed.
// The mean values of the plate and gas gap thicknesses
// are supposed to be about XTR formation zones but much less than
// mean absorption length of XTR photons in corresponding material.

G4double G4RegularXTRadiator::GetStackFactor(G4double energy, G4double gamma,
                                             G4double varAngle)
{
  // some gamma (10000/1000) like algorithm

  G4double result, Za, Zb, Ma, Mb;

  Za = GetPlateFormationZone(energy, gamma, varAngle);
  Zb = GetGasFormationZone(energy, gamma, varAngle);

  Ma = GetPlateLinearPhotoAbs(energy);
  Mb = GetGasLinearPhotoAbs(energy);

  G4complex Ca(1.0 + 0.5 * fPlateThick * Ma / fAlphaPlate,
               fPlateThick / Za / fAlphaPlate);
  G4complex Cb(1.0 + 0.5 * fGasThick * Mb / fAlphaGas,
               fGasThick / Zb / fAlphaGas);

  G4complex Ha = std::pow(Ca, -fAlphaPlate);
  G4complex Hb = std::pow(Cb, -fAlphaGas);
  G4complex H  = Ha * Hb;

  G4complex F1 = (1.0 - Ha) * (1.0 - Hb) / (1.0 - H) * G4double(fPlateNumber);

  G4complex F2 = (1.0 - Ha) * (1.0 - Ha) * Hb / (1.0 - H) / (1.0 - H) *
                 (1.0 - std::pow(H, fPlateNumber));

  G4complex R = (F1 + F2) * OneInterfaceXTRdEdx(energy, gamma, varAngle);

  result = 2.0 * std::real(R);

  return result;
}
