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
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999)
//

#include "G4Be7GEMProbability.hh"
#include "G4SystemOfUnits.hh"

G4Be7GEMProbability::G4Be7GEMProbability() :
  G4GEMProbability(7,4,3.0/2.0) // A,Z,Spin
{
  ExcitEnergies.push_back(429.08*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(133.0e-15*s);

  ExcitEnergies.push_back(4570.0*keV);
  ExcitSpins.push_back(7.0/2.0);
  ExcitLifetimes.push_back(fPlanck/(175.0*keV));

  ExcitEnergies.push_back(6.73e3*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(fPlanck/(1.2*MeV));

  ExcitEnergies.push_back(7210.0*keV);
  ExcitSpins.push_back(5.0/2.0);
  ExcitLifetimes.push_back(fPlanck/(0.5*MeV));

  ExcitEnergies.push_back(9900.0*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(fPlanck/(1.8*MeV));

  ExcitEnergies.push_back(11010.0*keV);
  ExcitSpins.push_back(3.0/2.0);
  ExcitLifetimes.push_back(fPlanck/(320.0*keV));

  ExcitEnergies.push_back(17000.0*keV);
  ExcitSpins.push_back(1.0/2.0);
  ExcitLifetimes.push_back(fPlanck/(6.5*MeV));
}

G4Be7GEMProbability::~G4Be7GEMProbability() 
{}
