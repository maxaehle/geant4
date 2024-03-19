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
// 19.09.21 V. Grichine, first version
//

#include "G4GaussXTRadiator.hh"

#include "G4PhysicalConstants.hh"

////////////////////////////////////////////////////////////////////////////
// Constructor, destructor

G4GaussXTRadiator::G4GaussXTRadiator(
  G4LogicalVolume* anEnvelope,   G4double alphaPlate, G4double alphaGas, G4Material* foilMat, G4Material* gasMat,
  G4double a, G4double b, G4int n, const G4String& processName)
  : G4VXTRenergyLoss(anEnvelope, foilMat, gasMat, a, b, n, processName)
{
  if(verboseLevel > 0)
    G4cout << "Gauss X-ray TR  radiator EM process is called"
           << G4endl;

  fAlphaPlate = alphaPlate;
  fAlphaGas   = alphaGas; //  1000; // 
}

///////////////////////////////////////////////////////////////////////////
G4GaussXTRadiator::~G4GaussXTRadiator() {}

///////////////////////////////////////////////////////////////////////////
void G4GaussXTRadiator::ProcessDescription(std::ostream& out) const
{
  out << "Simulation of forward X-ray transition radiation generated by\n"
         "relativistic charged particles crossing the interface between\n"
         "two materials.\n";
}

///////////////////////////////////////////////////////////////////////////
G4double G4GaussXTRadiator::SpectralXTRdEdx(G4double energy)
{
  G4double result, sum = 0., tmp, cof1, cof2, cofMin, cofPHC, theta2, theta2k;
  G4int k, kMax, kMin;

  cofPHC = 4. * pi * hbarc;
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
  result = 4. * (cof1 + cof2) * (cof1 + cof2) * sum / energy;
  result *= fPlateNumber;

  return result;
}

///////////////////////////////////////////////////////////////////////////
//
// Approximation for radiator interference factor for the case of
// Gauss-distributed regular radiator. The plate and gas gap thicknesses are Gauss distributed with RMS
// sa and sb for plate and gas, respectively.
// The mean values of the plate and gas gap thicknesses
// are supposed to be about XTR formation zones.


G4double G4GaussXTRadiator::GetStackFactor(G4double energy,
                                                    G4double gamma,
                                                    G4double varAngle)
{
  G4double result, Qa, Qb, Q, Qn, aZa, bZb, aMa, bMb;

  G4double Ma, Mb, Za, Zb;

  G4double sa = fPlateThick/fAlphaPlate;
  G4double sb = fGasThick/fAlphaGas;

  Za  = GetPlateFormationZone(energy, gamma, varAngle);
  
  aZa = fPlateThick / Za ;

  Zb  =  GetGasFormationZone(energy, gamma, varAngle);
  
  bZb = fGasThick / Zb ;

  Ma =  GetPlateLinearPhotoAbs(energy);
  
  aMa = fPlateThick * Ma;

  Mb = GetGasLinearPhotoAbs(energy);
  
  bMb   = fGasThick * Mb;

  // Gauss fluctuation of gas gaps according to RMS = sb = b/fAlphaGas
 
  G4double gre, gim, pre, pim;

  pre = -0.5 * aMa - sa * sa * ( 4./ Za / Za - Ma*Ma )/8.;
  
  gre = -0.5 * bMb - sb * sb * ( 4./ Zb / Zb - Mb*Mb )/8.;

  pim = sa * sa * Ma/2./Za - aZa;

  gim = sb * sb * Mb/2./Zb - bZb;
 
  Qa    = std::exp(pre);
  
  Qb    = std::exp(gre);
  
  // Q     = Qa * Qb;

  G4complex Ha( Qa * std::cos(pim), Qa * std::sin(pim) );
  
  G4complex Hb( Qb * std::cos(gim), Qb * std::sin(gim) );

  G4double hre, him, hnre, hnim;

  hre  = pre + gre;
  
  him  = pim + gim;

  G4double nn = G4double(fPlateNumber);
  
  hnre = nn*hre;
  
  hnim = nn*him;

  Q  = std::exp(hre);
  
  Qn = std::exp(hnre);
  
  
  // G4complex H  = Ha * Hb;


  G4complex H( Q * std::cos(him), Q * std::sin(him) );
  G4complex Hn( Qn * std::cos(hnim), Qn * std::sin(hnim) );
  
  // G4complex Hs = conj(H);

  // G4double sigma, D;
  
  // sigma = aMa * fPlateThick + bMb * fGasThick;

  // D = 1.0 / ((1 - Q) * (1 - Q) + 4 * Q * std::sin(0.5 * (aZa + bZb)) * std::sin(0.5 * (aZa + bZb)));
  
  // G4complex F1 = ( 1.0 - Ha ) * ( 1.0 - Hb ) * ( 1.0 - Hs ) * G4double(fPlateNumber) * D;
  
  G4complex F1 = ( 1.0 - Ha ) * ( 1.0 - Hb ) * nn / ( 1. - H );
  
  // G4complex F2 = ( 1.0 - Ha ) * ( 1.0 - Ha ) * Hb * ( 1.0 - Hs ) * ( 1.0 - Hs ) * (1.0 - std::exp( -0.5 * fPlateNumber * sigma) ) * D * D;

  G4complex F2 = ( 1.0 - Ha ) * ( 1.0 - Ha ) * Hb * ( 1. - Hn ) / ( 1. - H ) / ( 1. - H );
  
  G4complex R = (F1 + F2) * OneInterfaceXTRdEdx(energy, gamma, varAngle);
  
  result      = 2.0 * std::real(R);
  
  return result;
}
