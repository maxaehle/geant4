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
// Author:  Dennis Wright (SLAC)
// Date:    6 January 2014
//
// Description: class containing numerically integrated angular distributions
//              in the CM for pi+ p and pi- n elastic scattering.  Below 2.6
//              GeV distributions are taken from SAID phase shift calculations
//              with Coulomb interactions turned off.  Above 2.6 GeV, pp 
//              elastic scattering data are used.
//

#ifndef G4PipP2PipPAngDst_h
#define G4PipP2PPipAngDst_h 1

#include "G4NumIntTwoBodyAngDst.hh"


class G4PipP2PipPAngDst : public G4NumIntTwoBodyAngDst<11,19> {
public:
  G4PipP2PipPAngDst(G4int verbose = 0);
  virtual ~G4PipP2PipPAngDst() {;}
};

#endif
